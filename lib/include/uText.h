#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <numeric>
#include <optional>
#include <print>
#include <ranges>
#include <span>
#include <string>
#include <vector>
#include "unicode.h"

namespace text
{
     enum struct Encoding : std::uint8_t
     {
          UTF8,
          UTF16,
          UTF32
     };

     enum struct Normalisation : std::uint8_t
     {
          NFC,
          NFD,
          NFKC,
          NFKD
     };

     enum struct Comparison : std::uint8_t
     {
          OrdinallyEquivalent,
          OrdinallyEquivalentIgnoreCase,
          CanonicallyEquivalent,
          CompatibilityEquivalent,
          CanonicallyEquivalentIgnoreCase,
          CompatibilityEquivalentIgnoreCase,
          NotEquivalent,
     };

     template <typename Unicode = unicode::UnicodeRoot> struct Text
     {
          struct DecodedEntry
          {
               std::uint8_t run;
               std::uint32_t codepoint;
               std::size_t indexBitPos;
               std::size_t payloadBitPos;
          };

          std::size_t indexSizeInPoints{};
          std::size_t indexSizeInBits{};
          std::unique_ptr<std::byte[]> data{}; // NOLINT
          mutable std::vector<std::size_t> graphemeStarts;

          [[nodiscard]] constexpr std::size_t SizeInCodepoints(void) const noexcept { return this->indexSizeInPoints; }
          [[nodiscard]] constexpr std::size_t SizeInIndexBits(void) const noexcept { return this->indexSizeInBits; }
          [[nodiscard]] constexpr std::size_t SizeInGraphemes(void) const noexcept
          {
               EnsureGraphemeIndex();
               return graphemeStarts.empty() ? 0u : graphemeStarts.size() - 1u;
          }
          [[nodiscard]] constexpr std::size_t Size(void) const noexcept { return SizeInGraphemes(); }

          [[nodiscard]] constexpr bool IsEmpty(void) const noexcept { return this->indexSizeInPoints == 0; }

          [[nodiscard]] constexpr unicode::Direction Direction(void) const
          {
               const auto codePoints = AllCodepoints();
               return Unicode::DetectParagraphDirection(codePoints);
          }

          Text(void) = default;
          Text(const Text& other)
              : indexSizeInPoints{other.indexSizeInPoints}, indexSizeInBits{other.indexSizeInBits},
                graphemeStarts{other.graphemeStarts}
          {
               if (other.data)
               {
                    const std::size_t indexBits = indexSizeInBits;
                    std::size_t payloadBits = 0;
                    other.ForEach(
                        [&](const DecodedEntry& entry)
                        {
                             payloadBits += RunWidth(entry.run);
                             return true;
                        });
                    const std::size_t totalBytes = ByteCount(indexBits, payloadBits);
                    data = std::make_unique_for_overwrite<std::byte[]>(totalBytes); // NOLINT
                    std::copy_n(other.data.get(), totalBytes, data.get());
               }
          }
          Text(Text&& other) noexcept
              : indexSizeInPoints{other.indexSizeInPoints}, indexSizeInBits{other.indexSizeInBits},
                data{std::move(other.data)}, graphemeStarts{std::move(other.graphemeStarts)}
          {
               other.indexSizeInPoints = 0;
               other.indexSizeInBits = 0;
          }

          Text& operator=(const Text& other)
          {
               if (this == &other) return *this;
               Text tmp{other};
               *this = std::move(tmp);
               return *this;
          }
          Text& operator=(Text&& other) noexcept
          {
               if (this == &other) return *this;
               indexSizeInPoints = other.indexSizeInPoints;
               indexSizeInBits = other.indexSizeInBits;
               data = std::move(other.data);
               graphemeStarts = std::move(other.graphemeStarts);
               other.indexSizeInPoints = 0;
               other.indexSizeInBits = 0;
               return *this;
          }
          ~Text(void) = default;

          constexpr explicit Text(const std::span<const std::byte> data, const Encoding sourceEncoding)
          {
               if (data.empty()) return;

               switch (sourceEncoding)
               {
               case Encoding::UTF8: ParseUTF8(data); break;
               default: break;
               }
          }
          bool All(std::invocable<const DecodedEntry&> auto&& visitor) const
          {
               if (!data || indexSizeInPoints == 0) return true;
               bool result = true;
               ForEach(
                   [&](const DecodedEntry& entry)
                   {
                        result = std::invoke(visitor, entry);
                        return result;
                   });
               return result;
          }
          constexpr bool Some(std::invocable<const DecodedEntry&> auto&& visitor) const
          {
               if (!data || indexSizeInPoints == 0) return false;
               bool found = false;
               ForEach(
                   [&](const DecodedEntry& entry)
                   {
                        found = std::invoke(visitor, entry);
                        return !found;
                   });
               return found;
          }
          template <typename TReduced>
          auto Reduce(std::invocable<TReduced, std::uint32_t> auto&& visitor, TReduced initial) const
          {
               ForEach(
                   [&](const DecodedEntry& entry)
                   {
                        initial = std::invoke(visitor, std::move(initial), entry.codepoint);
                        return true;
                   });
               return initial;
          }

     private:
          [[nodiscard]] constexpr bool ReadBit(std::size_t pos) const noexcept
          {
               const std::size_t byteOffset = pos >> 3;
               const std::size_t bitOffset = 7u - (pos & 7u);
               return (std::to_underlying(this->data[byteOffset]) & (1u << bitOffset)) != 0;
          }
          static constexpr std::size_t RunWidth(std::size_t run) noexcept
          { return run == 1u ? 7u : run == 2u ? 11u : run == 3u ? 16u : 21u; }
          static constexpr std::uint8_t CodepointWidth(std::uint32_t codePoint) noexcept
          {
               if (codePoint <= 0x7Fu) return 1u;
               if (codePoint <= 0x7FFu) return 2u;
               if (codePoint <= 0xFFFFu) return 3u;
               return 4u;
          }

          [[nodiscard]] constexpr std::uint32_t ReadBits(std::size_t bitPos, std::size_t count) const noexcept
          {
               std::uint32_t value{};
               for (std::size_t i = 0; i < count; i++)
                    value = (value << 1) | static_cast<std::uint32_t>(ReadBit(bitPos + i));
               return value;
          }

          constexpr static void StaticWriteBit(std::byte* storage, std::size_t& bitPos, bool value) noexcept
          {
               const std::size_t byteOffset = bitPos >> 3;
               const std::size_t bitOffset = 7u - (bitPos & 7u);
               const std::byte mask{static_cast<unsigned char>(1u << bitOffset)};
               if (value) storage[byteOffset] |= mask;
               else
                    storage[byteOffset] &= ~mask;
               bitPos++;
          }

          constexpr static void StaticWriteBits(std::byte* storage, std::size_t& bitPos, std::uint32_t value,
                                                std::size_t count) noexcept
          {
               for (std::size_t i = 0; i < count; i++)
               {
                    const bool bit = static_cast<bool>((value >> (count - 1u - i)) & 1u);
                    StaticWriteBit(storage, bitPos, bit);
               }
          }

          constexpr void ForEach(std::invocable<DecodedEntry> auto&& visitor) const
          {
               if (this->data == nullptr || this->indexSizeInPoints == 0) return;
               std::size_t headerBitCursor = 0;
               std::size_t payloadBitPos = indexSizeInBits;
               std::size_t codePoint = 0;

               while (headerBitCursor < indexSizeInBits)
               {
                    const bool state = ReadBit(headerBitCursor);
                    std::size_t runLen = 0;
                    while (headerBitCursor < indexSizeInBits && ReadBit(headerBitCursor) == state)
                    {
                         runLen++;
                         headerBitCursor++;
                    }

                    const std::size_t width = RunWidth(runLen);
                    DecodedEntry entry{
                        .run = static_cast<std::uint8_t>(runLen),
                        .codepoint = ReadBits(payloadBitPos, width),
                        .indexBitPos = headerBitCursor - runLen,
                        .payloadBitPos = payloadBitPos,
                    };
                    payloadBitPos += width;
                    codePoint += runLen;

                    if (!std::invoke(visitor, entry)) return;
               }
          }

          [[nodiscard]] constexpr std::vector<DecodedEntry> BuildRawIndex(void) const
          {
               std::vector<DecodedEntry> table;
               table.reserve(this->indexSizeInPoints);
               ForEach(
                   [&](const DecodedEntry& entry)
                   {
                        table.push_back(entry);
                        return true;
                   });
               return table;
          }

          struct FromDecoded
          {
          };

          constexpr explicit Text([[maybe_unused]] FromDecoded tag, const std::vector<DecodedEntry>& entries)
          {
               if (entries.empty()) return;

               std::size_t indexBitCount = 0;
               std::size_t payloadBitCount = 0;
               for (const auto& entry : entries)
               {
                    indexBitCount += entry.run;
                    payloadBitCount += RunWidth(entry.run);
               }

               this->indexSizeInPoints = entries.size();
               const std::size_t totalBytes = (indexBitCount + payloadBitCount + 7) / 8;
               this->data = std::make_unique_for_overwrite<std::byte[]>(totalBytes); // NOLINT
               std::fill_n(this->data.get(), totalBytes, std::byte{0});

               std::size_t bitPos = 0;
               bool state = true;
               for (const auto& entry : entries)
               {
                    for (std::uint8_t i = 0; i < entry.run; i++) StaticWriteBit(this->data.get(), bitPos, state);
                    state = !state;
               }
               this->indexSizeInBits = bitPos;

               for (const auto& entry : entries)
               {
                    const std::size_t width = RunWidth(entry.run);
                    StaticWriteBits(this->data.get(), bitPos, entry.codepoint, width);
               }
          }

          constexpr void ParseUTF8(std::span<const std::byte> src)
          {
               constexpr std::uint8_t mask1MSB = 0b10000000u;
               constexpr std::uint8_t mask11MSB = 0b11000000u;
               constexpr std::uint8_t mask111MSB = 0b11100000u;
               constexpr std::uint8_t mask1111MSB = 0b11110000u;
               constexpr std::uint8_t mask11111MSB = 0b11111000u;

               constexpr auto isContinuation = [](std::uint8_t byte) constexpr
               { return (byte & mask11MSB) == mask1MSB; };

               std::vector<std::uint8_t> runs;
               std::vector<std::uint32_t> payload;
               runs.reserve(src.size());
               payload.reserve(src.size());

               std::size_t indexPointCount{};
               std::size_t indexBitCount{};
               std::size_t payloadBitCount{};

               for (auto it = src.begin(); it != src.end();)
               {
                    const auto nextByte = static_cast<std::uint8_t>(*it);

                    if ((nextByte & mask1MSB) == 0)
                    {
                         indexPointCount++;
                         indexBitCount += 1;
                         payloadBitCount += 7;
                         runs.emplace_back(1);
                         payload.emplace_back(nextByte & 0x7Fu);
                         it++;
                         continue;
                    }
                    if ((nextByte & mask111MSB) == mask11MSB)
                    {
                         if (std::next(it) == src.end() || !isContinuation(static_cast<std::uint8_t>(*std::next(it))))
                              break;
                         const auto byte1 = static_cast<std::uint8_t>(*std::next(it));
                         indexPointCount++;
                         indexBitCount += 2;
                         payloadBitCount += 11;
                         runs.emplace_back(2);
                         payload.emplace_back(((nextByte & 0x1Fu) << 6) | (byte1 & 0x3Fu));
                         it += 2;
                         continue;
                    }
                    if ((nextByte & mask1111MSB) == mask111MSB)
                    {
                         if (std::distance(it, src.end()) < 3) break;
                         const auto byte1 = static_cast<std::uint8_t>(*std::next(it, 1));
                         const auto byte2 = static_cast<std::uint8_t>(*std::next(it, 2));
                         if (!isContinuation(byte1) || !isContinuation(byte2)) break;
                         indexPointCount++;
                         indexBitCount += 3;
                         payloadBitCount += 16;
                         runs.emplace_back(3);
                         payload.emplace_back(((nextByte & 0x0Fu) << 12) | ((byte1 & 0x3Fu) << 6) | (byte2 & 0x3Fu));
                         it += 3;
                         continue;
                    }
                    if ((nextByte & mask11111MSB) == mask1111MSB)
                    {
                         if (std::distance(it, src.end()) < 4) break;
                         const auto byte1 = static_cast<std::uint8_t>(*std::next(it, 1));
                         const auto byte2 = static_cast<std::uint8_t>(*std::next(it, 2));
                         const auto byte3 = static_cast<std::uint8_t>(*std::next(it, 3));
                         if (!isContinuation(byte1) || !isContinuation(byte2) || !isContinuation(byte3)) break;
                         indexPointCount++;
                         indexBitCount += 4;
                         payloadBitCount += 21;
                         runs.emplace_back(4);
                         payload.emplace_back(((nextByte & 0x07u) << 18) | ((byte1 & 0x3Fu) << 12) |
                                              ((byte2 & 0x3Fu) << 6) | (byte3 & 0x3Fu));
                         it += 4;
                         continue;
                    }
                    it++;
               }

               this->indexSizeInPoints = indexPointCount;
               const std::size_t totalBytes = (indexBitCount + payloadBitCount + 7) / 8;
               this->data = std::make_unique_for_overwrite<std::byte[]>(totalBytes); // NOLINT
               std::fill_n(this->data.get(), totalBytes, std::byte{0});

               std::size_t bitPos{};
               bool bitState = true;
               for (auto length : runs)
               {
                    for (std::uint8_t i = 0; i < length; i++) StaticWriteBit(this->data.get(), bitPos, bitState);
                    bitState = !bitState;
               }
               this->indexSizeInBits = bitPos;

               std::size_t runIndex{};
               for (auto codePoint : payload)
               {
                    const auto run = runs[runIndex++];
                    const std::size_t width = run == 1 ? 7u : run == 2 ? 11u : run == 3 ? 16u : 21u;
                    StaticWriteBits(this->data.get(), bitPos, codePoint, width);
               }
          }

          constexpr void EnsureGraphemeIndex(void) const
          {
               if (!graphemeStarts.empty()) return;

               std::vector<std::uint32_t> codePoints;
               codePoints.reserve(this->indexSizeInPoints);
               ForEach(
                   [&](const DecodedEntry& entry)
                   {
                        codePoints.push_back(entry.codepoint);
                        return true;
                   });

               graphemeStarts.clear();
               if (codePoints.empty()) return;

               graphemeStarts.push_back(0);

               bool oddRI = false;

               auto IsGB10 = [&](std::size_t index)
               {
                    const auto right = unicode::GraphemeClusterBoundaryPropertyOf(codePoints[index]);

                    if (right != unicode::GraphemeClusterBoundaryProperty::EmojiModifier) return false;

                    for (std::size_t j = index; j > 0; j--)
                    {
                         const auto property = unicode::GraphemeClusterBoundaryPropertyOf(codePoints[j - 1]);

                         if (property == unicode::GraphemeClusterBoundaryProperty::Extend) continue;

                         return property == unicode::GraphemeClusterBoundaryProperty::ExtPic;
                    }

                    return false;
               };
               auto IsGB11 = [&](std::size_t index)
               {
                    const auto left = unicode::GraphemeClusterBoundaryPropertyOf(codePoints[index - 1]);
                    const auto right = unicode::GraphemeClusterBoundaryPropertyOf(codePoints[index]);

                    if (left != unicode::GraphemeClusterBoundaryProperty::ZeroWidthJoiner) return false;

                    if (right != unicode::GraphemeClusterBoundaryProperty::ExtPic) return false;

                    for (std::size_t j = index - 1; j > 0; j--)
                    {
                         const auto property = unicode::GraphemeClusterBoundaryPropertyOf(codePoints[j - 1]);

                         if (property == unicode::GraphemeClusterBoundaryProperty::Extend ||
                             property == unicode::GraphemeClusterBoundaryProperty::EmojiModifier)
                              continue;

                         return property == unicode::GraphemeClusterBoundaryProperty::ExtPic;
                    }

                    return false;
               };

               for (std::size_t i = 1; i < codePoints.size(); i++)
               {
                    const auto left = unicode::GraphemeClusterBoundaryPropertyOf(codePoints[i - 1]);
                    const auto right = unicode::GraphemeClusterBoundaryPropertyOf(codePoints[i]);

                    if (left == unicode::GraphemeClusterBoundaryProperty::RegionalIndicator) oddRI = !oddRI;
                    else
                         oddRI = false;
                    if (IsGB10(i)) continue;
                    if (IsGB11(i)) continue;

                    if (unicode::IsGraphemeBreak(left, right, oddRI, false))
                    {
                         graphemeStarts.push_back(i);
                         oddRI = false;
                    }
               }
               graphemeStarts.push_back(codePoints.size()); // sentinel
          }

          [[nodiscard]] constexpr std::vector<std::uint32_t> AllCodepoints(void) const
          {
               std::vector<std::uint32_t> out;
               out.reserve(this->indexSizeInPoints);
               ForEach(
                   [&](const DecodedEntry& entry)
                   {
                        out.push_back(entry.codepoint);
                        return true;
                   });
               return out;
          }
          [[nodiscard]] static std::size_t ByteCount(std::size_t indexBits, std::size_t payloadBits) noexcept
          { return (indexBits + payloadBits + 7u) / 8u; }

          [[nodiscard]] constexpr std::pair<std::size_t, std::size_t> GraphemeToCodepointRange(
              std::size_t graphemeIndex) const
          {
               EnsureGraphemeIndex();
               if (graphemeIndex + 1 >= graphemeStarts.size())
                    throw std::out_of_range{std::format("Text: grapheme index {} out of range (size {})", graphemeIndex,
                                                        SizeInGraphemes())};
               return {graphemeStarts[graphemeIndex], graphemeStarts[graphemeIndex + 1]};
          }

     public:
          [[nodiscard]] constexpr std::uint32_t operator[](std::size_t graphemeIndex) const
          {
               const auto [cpStart, cpEnd] = GraphemeToCodepointRange(graphemeIndex);
               std::uint32_t result{};
               std::size_t current{};
               ForEach(
                   [&](const DecodedEntry& entry) -> bool
                   {
                        if (current == cpStart)
                        {
                             result = entry.codepoint;
                             return false;
                        }
                        current++;
                        return true;
                   });
               return result;
          }

          [[nodiscard]] constexpr std::vector<std::uint32_t> GraphemeAt(std::size_t graphemeIndex) const
          {
               const auto [cpStart, cpEnd] = GraphemeToCodepointRange(graphemeIndex);
               std::vector<std::uint32_t> out;
               out.reserve(cpEnd - cpStart);
               std::size_t current{};
               ForEach(
                   [&](const DecodedEntry& entry) -> bool
                   {
                        if (current >= cpStart && current < cpEnd) out.push_back(entry.codepoint);
                        current++;
                        return current < cpEnd;
                   });
               return out;
          }

          [[nodiscard]] constexpr std::vector<DecodedEntry> GraphemeAtEx(const std::size_t graphemeIndex) const
          {
               const auto [cpStart, cpEnd] = GraphemeToCodepointRange(graphemeIndex);
               std::vector<DecodedEntry> out;
               out.reserve(cpEnd - cpStart);
               std::size_t current{};
               ForEach(
                   [&](const DecodedEntry& entry) -> bool
                   {
                        if (current >= cpStart && current < cpEnd) out.push_back(entry);
                        current++;
                        return current < cpEnd;
                   });
               return out;
          }

          [[nodiscard]] constexpr Text operator[](decltype(0z) start, decltype(0z) length) const
          {
               const auto originalSize = static_cast<decltype(start)>(SizeInGraphemes());
               if (start >= 0)
               {
                    if (length == 0) return Substring(start);
                    if (length > 0) return Substring(start, length);
                    return Substring(start, originalSize + length);
               }
               const auto startPoint = originalSize + start;
               if (length == 0) return Substring(startPoint);
               if (length > 0) return Substring(startPoint, length);
               return Substring(startPoint, originalSize - length);
          }

          [[nodiscard]] constexpr Text Substring(const std::size_t startGrapheme,
                                                 const std::size_t countGraphemes = std::dynamic_extent) const
          {
               EnsureGraphemeIndex();
               const std::size_t totalG = SizeInGraphemes();
               if (startGrapheme >= totalG || countGraphemes == 0) return Text{};

               const std::size_t clampedCount = (countGraphemes == std::dynamic_extent)
                                                    ? totalG - startGrapheme
                                                    : std::min(countGraphemes, totalG - startGrapheme);

               const std::size_t cpStart = graphemeStarts[startGrapheme];
               const std::size_t cpEnd = graphemeStarts[startGrapheme + clampedCount];

               std::vector<DecodedEntry> slice;
               slice.reserve(cpEnd - cpStart);
               std::size_t current{};
               ForEach(
                   [&](const DecodedEntry& entry) -> bool
                   {
                        if (current >= cpStart && current < cpEnd) slice.push_back(entry);
                        current++;
                        return current < cpEnd;
                   });
               return Text{FromDecoded{}, slice};
          }

          [[nodiscard]] constexpr Text SubstringCodePoints(const std::size_t startCP,
                                                           const std::size_t countCP = std::dynamic_extent) const
          {
               if (startCP >= this->indexSizeInPoints || countCP == 0) return Text{};
               const std::size_t clamped = (countCP == std::dynamic_extent)
                                               ? this->indexSizeInPoints - startCP
                                               : std::min(countCP, this->indexSizeInPoints - startCP);

               std::vector<DecodedEntry> slice;
               slice.reserve(clamped);
               std::size_t current{};
               ForEach(
                   [&](const DecodedEntry& entry) -> bool
                   {
                        if (current >= startCP && current < startCP + clamped) slice.push_back(entry);
                        current++;
                        return current < startCP + clamped;
                   });
               return Text{FromDecoded{}, slice};
          }

          [[nodiscard]] constexpr Text LogicalToVisual(void) const
          {
               EnsureGraphemeIndex();
               const std::size_t sizeInGraphemes = SizeInGraphemes();
               if (sizeInGraphemes == 0) return Text{};

               const auto dir = Direction();
               bool paraRTL = (dir == unicode::Direction::RTL);

               std::vector<std::uint8_t> levels(sizeInGraphemes);
               for (std::size_t i = 0; i < sizeInGraphemes; i++)
               {
                    const auto codePoint = operator[](i);
                    const auto bidiClass = Unicode::BidiClassOf(codePoint);
                    using Class = unicode::BidiClass;
                    const bool isRTL = (bidiClass == Class::R || bidiClass == Class::AL || bidiClass == Class::AN);
                    if (bidiClass == Class::RLO) paraRTL = true;
                    else if (bidiClass == Class::LRO)
                         paraRTL = false;
                    levels[i] = (paraRTL ? 1u : 0u);
                    if (isRTL) levels[i] = 1;
                    else if (bidiClass == Class::L)
                         levels[i] = 0;
               }

               std::vector<std::size_t> order(sizeInGraphemes);
               std::ranges::iota(order, 0u);

               const std::uint8_t maxLevel = *std::ranges::max_element(levels);
               for (std::uint8_t level = maxLevel; level >= 1u; --level)
               {
                    std::size_t runStart = std::numeric_limits<std::size_t>::max();
                    for (std::size_t i = 0; i <= sizeInGraphemes; i++)
                    {
                         const bool inRun = (i < sizeInGraphemes && levels[order[i]] >= level);
                         if (inRun && runStart == std::numeric_limits<std::size_t>::max()) runStart = i;
                         else if (!inRun && runStart != std::numeric_limits<std::size_t>::max())
                         {
                              std::reverse(order.begin() + static_cast<std::ptrdiff_t>(runStart),
                                           order.begin() + static_cast<std::ptrdiff_t>(i));
                              runStart = std::numeric_limits<std::size_t>::max();
                         }
                    }
               }

               const auto rawIndex = BuildRawIndex();
               std::vector<DecodedEntry> reordered;
               reordered.reserve(this->indexSizeInPoints);
               for (const auto graphemeIndex : order)
               {
                    const std::size_t cpStart = graphemeStarts[graphemeIndex];
                    const std::size_t cpEnd = graphemeStarts[graphemeIndex + 1];
                    for (std::size_t ci = cpStart; ci < cpEnd; ci++) reordered.push_back(rawIndex[ci]);
               }
               return Text{FromDecoded{}, reordered};
          }
          [[nodiscard]] constexpr std::u8string ToUTF8(void) const
          {
               std::u8string out;
               out.reserve(this->indexSizeInPoints * 2);
               ForEach(
                   [&](const DecodedEntry& entry) -> bool
                   {
                        const auto codePoint = entry.codepoint;
                        if (entry.run == 1) out.push_back(static_cast<char8_t>(codePoint & 0x7Fu));
                        else if (entry.run == 2)
                        {
                             out.push_back(static_cast<char8_t>(0xC0u | ((codePoint >> 6) & 0x1Fu)));
                             out.push_back(static_cast<char8_t>(0x80u | (codePoint & 0x3Fu)));
                        }
                        else if (entry.run == 3)
                        {
                             out.push_back(static_cast<char8_t>(0xE0u | ((codePoint >> 12) & 0x0Fu)));
                             out.push_back(static_cast<char8_t>(0x80u | ((codePoint >> 6) & 0x3Fu)));
                             out.push_back(static_cast<char8_t>(0x80u | (codePoint & 0x3Fu)));
                        }
                        else
                        {
                             out.push_back(static_cast<char8_t>(0xF0u | ((codePoint >> 18) & 0x07u)));
                             out.push_back(static_cast<char8_t>(0x80u | ((codePoint >> 12) & 0x3Fu)));
                             out.push_back(static_cast<char8_t>(0x80u | ((codePoint >> 6) & 0x3Fu)));
                             out.push_back(static_cast<char8_t>(0x80u | (codePoint & 0x3Fu)));
                        }
                        return true;
                   });
               return out;
          }

          [[nodiscard]] constexpr std::u16string ToUTF16(void) const
          {
               std::u16string out;
               out.reserve(this->indexSizeInPoints);
               ForEach(
                   [&](const DecodedEntry& entry) -> bool
                   {
                        const auto codePoint = entry.codepoint;
                        if (codePoint < 0x10000u) { out.push_back(static_cast<char16_t>(codePoint)); }
                        else
                        {
                             const std::uint32_t value = codePoint - 0x10000u;
                             out.push_back(static_cast<char16_t>(0xD800u | (value >> 10)));
                             out.push_back(static_cast<char16_t>(0xDC00u | (value & 0x3FFu)));
                        }
                        return true;
                   });
               return out;
          }

          [[nodiscard]] constexpr std::u32string ToUTF32(void) const
          {
               std::u32string out;
               out.reserve(this->indexSizeInPoints);
               ForEach(
                   [&](const DecodedEntry& entry) -> bool
                   {
                        out.push_back(static_cast<char32_t>(entry.codepoint));
                        return true;
                   });
               return out;
          }

          static constexpr std::size_t npos = std::numeric_limits<std::size_t>::max();

          [[nodiscard]] constexpr std::size_t Find(std::uint32_t codepoint, std::size_t startGrapheme = 0) const
          {
               EnsureGraphemeIndex();
               const std::size_t sizeInGraphemes = SizeInGraphemes();
               for (std::size_t graphemeIndex = startGrapheme; graphemeIndex < sizeInGraphemes; graphemeIndex++)
                    if (operator[](graphemeIndex) == codepoint) return graphemeIndex;
               return npos;
          }

          [[nodiscard]] constexpr std::size_t Find(const Text& substring, std::size_t startGrapheme = 0) const
          {
               EnsureGraphemeIndex();
               const std::size_t sizeInGraphemes = SizeInGraphemes();
               const std::size_t subSize = substring.SizeInGraphemes();
               if (subSize == 0) return startGrapheme;
               if (subSize > sizeInGraphemes) return npos;
               for (std::size_t graphemeIndex = startGrapheme; graphemeIndex <= sizeInGraphemes - subSize;
                    graphemeIndex++)
               {
                    bool match = true;
                    for (std::size_t sgi = 0; sgi < subSize; sgi++)
                    {
                         if (operator[](graphemeIndex + sgi) != substring[sgi])
                         {
                              match = false;
                              break;
                         }
                    }
                    if (match) return graphemeIndex;
               }
               return npos;
          }

          [[nodiscard]] constexpr bool Contains(std::uint32_t codepoint) const
          {
               return Find(codepoint) != npos; // NOLINT(readability-container-contains)
          }
          [[nodiscard]] constexpr bool Contains(const Text& substring) const
          {
               return Find(substring) != npos; // NOLINT(readability-container-contains)
          }

          [[nodiscard]] constexpr bool StartsWith(std::uint32_t codepoint) const
          {
               if (IsEmpty()) return false;
               return operator[](0) == codepoint;
          }
          [[nodiscard]] constexpr bool StartsWith(const Text& other) const
          {
               EnsureGraphemeIndex();
               const std::size_t subSize = other.SizeInGraphemes();
               if (subSize == 0 || subSize > SizeInGraphemes()) return false;
               for (std::size_t sgi = 0; sgi < subSize; sgi++)
                    if (operator[](sgi) != other[sgi]) return false;
               return true;
          }

          [[nodiscard]] constexpr bool EndsWith(std::uint32_t codepoint) const
          {
               if (IsEmpty()) return false;
               return operator[](SizeInGraphemes() - 1) == codepoint;
          }

          [[nodiscard]] constexpr bool EndsWith(const Text& other) const
          {
               const std::size_t subSize = other.SizeInGraphemes();
               const std::size_t selfSize = SizeInGraphemes();
               if (subSize == 0) return true;
               if (subSize > selfSize) return false;
               const std::size_t offset = selfSize - subSize;
               for (std::size_t sgi = 0; sgi < subSize; sgi++)
                    if (operator[](offset + sgi) != other[sgi]) return false;
               return true;
          }

          [[nodiscard]] constexpr Text Trim(std::predicate<std::uint32_t> auto&& pred) const
          {
               EnsureGraphemeIndex();
               const std::size_t sizeInGraphemes = SizeInGraphemes();

               std::size_t first = 0;
               while (first < sizeInGraphemes && pred(operator[](first))) first++;

               std::size_t last = sizeInGraphemes;
               while (last > first && pred(operator[](last - 1))) last--;

               return Substring(first, last - first);
          }

          [[nodiscard]] constexpr Text TrimWhitespace(void) const
          {
               return Trim([](const std::uint32_t codePoint) { return IsWhitespace(codePoint); });
          }

          [[nodiscard]] constexpr Text Concat(const Text& other) const
          {
               auto left = BuildRawIndex();
               auto right = other.BuildRawIndex();
               left.insert(left.end(), right.begin(), right.end());
               return Text{FromDecoded{}, left};
          }

          Text operator+(const Text& other) const { return Concat(other); }
          Text& operator+=(const Text& other)
          {
               *this = Concat(other);
               return *this;
          }

          [[nodiscard]] constexpr Text ToReversed(void) const
          {
               EnsureGraphemeIndex();
               const std::size_t sizeInGraphemes = SizeInGraphemes();
               if (sizeInGraphemes == 0) return Text{};

               const auto rawIndex = BuildRawIndex();
               std::vector<DecodedEntry> reversed;
               reversed.reserve(this->indexSizeInPoints);

               for (std::size_t graphemeIndex = sizeInGraphemes; graphemeIndex-- > 0;)
               {
                    const std::size_t cpStart = graphemeStarts[graphemeIndex];
                    const std::size_t cpEnd = graphemeStarts[graphemeIndex + 1];
                    for (std::size_t ci = cpStart; ci < cpEnd; ci++) reversed.push_back(rawIndex[ci]);
               }
               return Text{FromDecoded{}, reversed};
          }

          [[nodiscard]] constexpr std::vector<Text> Split(const std::uint32_t delimiter) const
          {
               EnsureGraphemeIndex();
               const std::size_t sizeInGraphemes = SizeInGraphemes();
               std::vector<Text> result;
               std::size_t segStart = 0;

               for (std::size_t graphemeIndex = 0; graphemeIndex <= sizeInGraphemes; graphemeIndex++)
               {
                    if (graphemeIndex == sizeInGraphemes || operator[](graphemeIndex) == delimiter)
                    {
                         result.push_back(Substring(segStart, graphemeIndex - segStart));
                         segStart = graphemeIndex + 1;
                    }
               }
               return result;
          }

          [[nodiscard]] constexpr std::vector<Text> Split(const Text& delimiter) const
          {
               const std::size_t sizeInGraphemes = SizeInGraphemes();
               const std::size_t delimSize = delimiter.SizeInGraphemes();
               std::vector<Text> result;
               if (delimSize == 0)
               {
                    result.push_back(*this);
                    return result;
               }

               std::size_t segStart = 0;
               std::size_t graphemeIndex = 0;
               while (graphemeIndex <= sizeInGraphemes)
               {
                    const std::size_t found = Find(delimiter, graphemeIndex);
                    if (found == npos)
                    {
                         result.push_back(Substring(segStart));
                         break;
                    }
                    result.push_back(Substring(segStart, found - segStart));
                    segStart = found + delimSize;
                    graphemeIndex = segStart;
               }
               return result;
          }

          [[nodiscard]] static Text Join(std::span<const Text> parts, const Text& separator)
          {
               if (parts.empty()) return Text{};
               Text result = parts[0];
               for (std::size_t i = 1; i < parts.size(); i++) result = result.Concat(separator).Concat(parts[i]);
               return result;
          }

          [[nodiscard]] constexpr Text Replace(std::uint32_t from, std::uint32_t with) const
          {
               return Replace([from, with](std::uint32_t codePoint) { return codePoint == from ? with : codePoint; });
          }

          [[nodiscard]] constexpr Text Replace(const Text& from, const Text& with) const
          {
               if (from.IsEmpty()) return *this;
               const std::size_t sizeInGraphemes = SizeInGraphemes();
               const std::size_t fromSize = from.SizeInGraphemes();
               std::vector<Text> parts;
               std::size_t pos = 0;
               while (pos <= sizeInGraphemes)
               {
                    const std::size_t found = Find(from, pos);
                    if (found == npos)
                    {
                         parts.push_back(Substring(pos));
                         break;
                    }
                    parts.push_back(Substring(pos, found - pos));
                    parts.push_back(with);
                    pos = found + fromSize;
               }
               if (parts.empty()) return *this;
               Text result = parts[0];
               for (std::size_t i = 1; i < parts.size(); i++) result = result.Concat(parts[i]);
               return result;
          }

          [[nodiscard]] constexpr std::size_t Count(std::predicate<std::uint32_t> auto&& pred) const
          {
               std::size_t count = 0;
               const std::size_t sizeInGraphemes = SizeInGraphemes();
               for (std::size_t graphemeIndex = 0; graphemeIndex < sizeInGraphemes; graphemeIndex++)
                    if (pred(operator[](graphemeIndex))) count++;
               return count;
          }

          [[nodiscard]] constexpr std::size_t Count(std::uint32_t codepoint) const
          {
               return Count([codepoint](std::uint32_t codePoint) { return codePoint == codepoint; });
          }

          [[nodiscard]] constexpr Text ToLower(void) const { return Replace(Unicode::ToLower); }

          [[nodiscard]] constexpr Text ToUpper(void) const { return Replace(Unicode::ToUpper); }

          [[nodiscard]] constexpr bool operator==(const Text& other) const
          {
               if (this->indexSizeInPoints != other.indexSizeInPoints) return false;
               const auto lhs = BuildRawIndex();
               const auto rhs = other.BuildRawIndex();
               for (std::size_t i = 0; i < lhs.size(); i++)
                    if (lhs[i].codepoint != rhs[i].codepoint) return false;
               return true;
          }

          [[nodiscard]] constexpr Comparison Compare(const Text& other) const
          {
               if (*this == other) return Comparison::OrdinallyEquivalent;
               const auto lLower = ToLower();
               const auto rLower = other.ToLower();
               if (lLower == rLower) return Comparison::OrdinallyEquivalentIgnoreCase;
               const auto lNFC = ToNormalised(Normalisation::NFC);
               const auto rNFC = other.ToNormalised(Normalisation::NFC);
               if (lNFC == rNFC) return Comparison::CanonicallyEquivalent;
               const auto lNFKC = ToNormalised(Normalisation::NFKC);
               const auto rNFKC = other.ToNormalised(Normalisation::NFKC);
               if (lNFKC == rNFKC) return Comparison::CompatibilityEquivalent;
               if (lNFC.ToLower() == rNFC.ToLower()) return Comparison::CanonicallyEquivalentIgnoreCase;
               if (lNFKC.ToLower() == rNFKC.ToLower()) return Comparison::CompatibilityEquivalentIgnoreCase;
               return Comparison::NotEquivalent;
          }
          [[nodiscard]] constexpr Comparison operator<=>(const Text& other) const { return Compare(other); }

          void Dump(std::size_t indent = 0) const
          {
               EnsureGraphemeIndex();
               const auto utf8 = ToUTF8();
               std::println("{}Glyph = '{}'", std::string(indent, ' '),
                            reinterpret_cast<const char*>(utf8.c_str())); // NOLINT
               std::print("{}Codepoints: {}  Graphemes: {}\n{}Header: ", std::string(indent, ' '), SizeInCodepoints(),
                          SizeInGraphemes(), std::string(indent, ' '));

               std::size_t bitPos{};
               bool state = true;
               std::size_t stateSize{};
               std::vector<std::size_t> runs{};
               const std::size_t headerBits = this->indexSizeInBits;

               for (; bitPos < headerBits; bitPos++)
               {
                    stateSize++;
                    const bool cur = ReadBit(bitPos);
                    if (state == cur) continue;
                    std::print("{}, ", stateSize - 1);
                    runs.push_back(stateSize - 1);
                    state = cur;
                    stateSize = 1;
               }
               if (stateSize != 0)
               {
                    std::print("{}", stateSize);
                    runs.push_back(stateSize);
               }
               std::println();

               std::size_t payBitPos = headerBits;
               for (auto run : runs)
               {
                    const std::size_t width = (run == 1) ? 7u : (run == 2) ? 11u : (run == 3) ? 16u : 21u;
                    std::print("{}[{}]: ", std::string(indent, ' '), payBitPos);
                    for (std::size_t i = 0; i < width; i++)
                    {
                         const std::size_t bitPosition = payBitPos + i;
                         std::print("{}", static_cast<int>(ReadBit(bitPosition)));
                    }
                    std::println();
                    payBitPos += width;
               }

               std::println("{}Grapheme clusters:", std::string(indent, ' '));
               const std::size_t sizeInGraphemes = SizeInGraphemes();
               for (std::size_t graphemeIndex = 0; graphemeIndex < sizeInGraphemes; graphemeIndex++)
               {
                    const auto codePoints = GraphemeAt(graphemeIndex);
                    std::print("{}  [{}]: U+{:04X}", std::string(indent, ' '), graphemeIndex, codePoints[0]);
                    for (std::size_t codePoint = 1; codePoint < codePoints.size(); codePoint++)
                         std::print(" + U+{:04X}", codePoints[codePoint]);
                    std::println();
               }
          }

          [[nodiscard]] Text ToPaddedEnd(std::size_t size, std::convertible_to<char32_t> auto symbol) const
          {
               const auto codePoint = static_cast<std::uint32_t>(static_cast<char32_t>(symbol));
               auto decoded = BuildRawIndex();
               std::vector<DecodedEntry> padded;
               padded.reserve(size);
               for (std::size_t i = 0; i < size; i++)
                    padded.push_back(i < decoded.size()
                                         ? decoded[i]
                                         : DecodedEntry{.run = CodepointWidth(codePoint), .codepoint = codePoint});
               return Text{FromDecoded{}, padded};
          }

          [[nodiscard]] Text ToPaddedStart(std::size_t size, std::convertible_to<char32_t> auto symbol) const
          {
               const auto codePoint = static_cast<std::uint32_t>(static_cast<char32_t>(symbol));
               auto decoded = BuildRawIndex();
               std::vector<DecodedEntry> padded;
               padded.reserve(size);
               for (std::size_t i = 0; i < size; i++)
                    padded.push_back(i < size - decoded.size()
                                         ? DecodedEntry{.run = CodepointWidth(codePoint), .codepoint = codePoint}
                                         : decoded[i - (size - decoded.size())]);
               return Text{FromDecoded{}, padded};
          }

          [[nodiscard]] Text ToNormalised(const Normalisation normalisation = Normalisation::NFC) const
          {
               const bool isCompatibility =
                   (normalisation == Normalisation::NFKD || normalisation == Normalisation::NFKC);

               const auto rawCPs = AllCodepoints();
               std::vector<std::uint32_t> decomposed{};
               decomposed.reserve(rawCPs.size() * 2);

               const auto expand = [&](this auto&& self, std::uint32_t codePoint) -> void
               {
                    if (isCompatibility)
                    {
                         std::array<std::uint32_t, 8> kompatibilityBuffer{};
                         const auto compatibileSequence =
                             Unicode::CompatibilityDecompose(codePoint, kompatibilityBuffer);
                         if (!compatibileSequence.empty())
                         {
                              for (const auto decomposed : compatibileSequence) self(decomposed);
                              return;
                         }
                    }

                    std::array<std::uint32_t, 3> buffer{};
                    const auto sequence = Unicode::CanonicalDecompose(codePoint, buffer);
                    if (sequence.empty())
                    {
                         decomposed.push_back(codePoint);
                         return;
                    }
                    for (const auto decomposed : sequence) self(decomposed);
               };
               for (const auto codePoint : rawCPs) expand(codePoint);

               for (std::size_t i = 1; i < decomposed.size(); i++)
               {
                    const auto canonicalCombiningClass = Unicode::CombiningClass(decomposed[i]);
                    if (canonicalCombiningClass == 0) continue;
                    std::size_t decomposedIndex = i;
                    while (decomposedIndex > 0 &&
                           Unicode::CombiningClass(decomposed[decomposedIndex - 1]) > canonicalCombiningClass)
                    {
                         std::swap(decomposed[decomposedIndex], decomposed[decomposedIndex - 1]);
                         decomposedIndex--;
                    }
               }

               if (normalisation == Normalisation::NFD || normalisation == Normalisation::NFKD)
               {
                    std::vector<DecodedEntry> entries;
                    entries.reserve(decomposed.size());
                    for (const auto codePoint : decomposed)
                         entries.push_back(DecodedEntry{.run = CodepointWidth(codePoint), .codepoint = codePoint});
                    return Text{FromDecoded{}, entries};
               }

               std::vector<std::uint32_t> composed;
               composed.reserve(decomposed.size());

               for (const auto codePoint : decomposed)
               {
                    const auto canonicalCombiningClass = Unicode::CombiningClass(codePoint);

                    if (composed.empty())
                    {
                         composed.push_back(codePoint);
                         continue;
                    }

                    bool blocked = false;
                    if (canonicalCombiningClass != 0u)
                    {
                         for (std::size_t codePoint = composed.size(); codePoint-- > 0u;)
                         {
                              const auto kCanonicalCombiningClass = Unicode::CombiningClass(composed[codePoint]);
                              if (kCanonicalCombiningClass == 0u) break;
                              if (kCanonicalCombiningClass >= canonicalCombiningClass)
                              {
                                   blocked = true;
                                   break;
                              }
                         }
                    }

                    bool inComposed = false;
                    if (!blocked)
                    {
                         for (std::size_t codePoint = composed.size(); codePoint-- > 0u;)
                         {
                              if (Unicode::CombiningClass(composed[codePoint]) != 0u) continue;
                              const auto candidate = Unicode::CanonicalCompose(composed[codePoint], codePoint);
                              if (candidate != 0u)
                              {
                                   composed[codePoint] = candidate;
                                   inComposed = true;
                              }
                              break;
                         }
                    }
                    if (!inComposed) composed.push_back(codePoint);
               }

               std::vector<DecodedEntry> entries;
               entries.reserve(composed.size());
               for (const auto codePoint : composed)
                    entries.push_back(DecodedEntry{.run = CodepointWidth(codePoint), .codepoint = codePoint});
               return Text{FromDecoded{}, entries};
          }

          void Normalise(const Normalisation normalisation = Normalisation::NFC)
          { *this = ToNormalised(normalisation); }

          auto FormatTo(std::format_context& context) const
          {
               const auto utf8 = ToUTF8();
               return std::format_to(
                   context.out(), "{}",
                   std::string_view{reinterpret_cast<const char*>(utf8.data()), utf8.size()}); // NOLINT
          }
          [[nodiscard]] constexpr Text Replace(std::invocable<std::uint32_t> auto&& mapper) const
          {
               auto decoded = BuildRawIndex();
               for (auto& entry : decoded)
               {
                    const std::uint32_t mapped = std::invoke(mapper, entry.codepoint);
                    if (mapped != entry.codepoint)
                    {
                         entry.codepoint = mapped;
                         entry.run = CodepointWidth(mapped);
                    }
               }
               return Text{FromDecoded{}, decoded};
          }
          [[nodiscard]] constexpr Text Filter(std::invocable<std::uint32_t> auto&& mapper) const
          {
               std::vector<DecodedEntry> copy{};
               copy.reserve(this->indexSizeInPoints);
               ForEach(
                   [&](const DecodedEntry& entry)
                   {
                        const bool passed = std::invoke(mapper, entry.codepoint);
                        if (passed) copy.push_back(entry);
                        return true;
                   });
               return Text{FromDecoded{}, copy};
          }
          [[nodiscard]] constexpr Text Filter(std::invocable<std::vector<std::uint32_t>> auto&& mapper) const
          {
               const std::size_t sizeInGraphemes = SizeInGraphemes();
               std::vector<DecodedEntry> copy{};
               copy.reserve(sizeInGraphemes);
               for (std::size_t graphemeIndex = 0; graphemeIndex < sizeInGraphemes; graphemeIndex++)
               {
                    const auto codePoints = GraphemeAtEx(graphemeIndex);
                    if (std::invoke(mapper,
                                    std::span<const std::uint32_t>{codePoints |
                                                                   std::views::transform([](const DecodedEntry& entry)
                                                                                         { return entry.codepoint; }) |
                                                                   std::ranges::to<std::vector>()}))
                         copy.append_range(codePoints);
               }
               return Text{FromDecoded{}, copy};
          }

          [[nodiscard]] constexpr std::optional<std::uint32_t> Find(std::invocable<std::uint32_t> auto&& mapper) const
          {
               std::optional<std::uint32_t> found;
               ForEach(
                   [&](const DecodedEntry& entry)
                   {
                        if (!std::invoke(mapper, entry.codepoint)) return true;
                        found = entry.codepoint;
                        return false;
                   });
               return found;
          }
          [[nodiscard]] constexpr std::optional<Text> Find(
              std::invocable<std::span<const std::uint32_t>> auto&& mapper) const
          {
               const std::size_t sizeInGraphemes = SizeInGraphemes();
               for (std::size_t graphemeIndex = 0; graphemeIndex < sizeInGraphemes; graphemeIndex++)
               {
                    const auto codePoints = GraphemeAtEx(graphemeIndex);
                    if (std::invoke(mapper,
                                    std::span<const std::uint32_t>{codePoints |
                                                                   std::views::transform([](const DecodedEntry& entry)
                                                                                         { return entry.codepoint; }) |
                                                                   std::ranges::to<std::vector>()}))
                         return Text{FromDecoded{}, codePoints};
               }
               return std::nullopt;
          }
          [[nodiscard]] constexpr std::optional<std::size_t> IndexOf(std::invocable<std::uint32_t> auto&& mapper) const
          {
               const std::size_t sizeInGraphemes = SizeInGraphemes();
               for (std::size_t graphemeIndex = 0; graphemeIndex < sizeInGraphemes; graphemeIndex++)
               {
                    const auto codePoints = GraphemeAt(graphemeIndex);
                    if (std::any_of(codePoints.begin(), codePoints.end(), [&mapper](const std::uint32_t codePoint)
                                    { return std::invoke(mapper, codePoint); }))
                         return graphemeIndex;
               }
               return std::nullopt;
          }
          [[nodiscard]] constexpr std::optional<std::size_t> IndexOf(
              std::invocable<std::span<const std::uint32_t>> auto&& mapper) const
          {
               const std::size_t sizeInGraphemes = SizeInGraphemes();
               for (std::size_t graphemeIndex = 0; graphemeIndex < sizeInGraphemes; graphemeIndex++)
               {
                    const auto codePoints = GraphemeAt(graphemeIndex);
                    if (std::invoke(mapper, std::span<const std::uint32_t>{codePoints})) return graphemeIndex;
               }
               return std::nullopt;
          }

          [[nodiscard]] static bool IsASCII(std::uint32_t codePoint) { return codePoint <= 0x007Fu; }

          [[nodiscard]] static bool IsASCIIGrapheme(std::span<const std::uint32_t> codePoints) noexcept
          {
               if (codePoints.size() != 1) return false;
               return codePoints[0] <= 0x007Fu;
          }
          [[nodiscard]] static bool IsWhitespace(std::uint32_t codePoint) { return Unicode::IsWhitespace(codePoint); }

          [[nodiscard]] static bool IsEmoji(std::span<const std::uint32_t> codePoints) noexcept
          {
               bool hasEmojiBase = false;
               bool onlyEmojiLike = true;

               for (unsigned int codePoint : codePoints)
               {
                    if (codePoint == 0xFE0F) continue;
                    if (codePoint == 0x200D) continue;
                    const auto graphemeClusterBoundary = unicode::GraphemeClusterBoundaryPropertyOf(codePoint);
                    if (graphemeClusterBoundary == unicode::GraphemeClusterBoundaryProperty::ExtPic)
                         hasEmojiBase = true;
                    else
                         onlyEmojiLike = false;
               }

               return hasEmojiBase && onlyEmojiLike;
          }

          [[nodiscard]] bool IsASCII(void) const
          {
               return All([](const DecodedEntry& entry) { return entry.codepoint <= 0x007Fu; });
          }

          [[nodiscard]] bool IsPalindrome(void) const { return *this == ToReversed(); }
     };

     constexpr Text<unicode::UnicodeRoot> operator""_text(const char8_t* data, std::size_t length)
     {
          std::span<const char8_t> span{data, length};
          return Text<unicode::UnicodeRoot>{std::as_bytes(span), Encoding::UTF8};
     }
     constexpr Text<unicode::UnicodeRoot> operator""_text(const char* data, std::size_t length)
     {
          std::span<const char> span{data, length};
          return Text<unicode::UnicodeRoot>{std::as_bytes(span), Encoding::UTF8};
     }
     template <typename TUnicode, std::size_t N> constexpr Text<TUnicode> FromString(const char8_t (&text)[N]) // NOLINT
     {
          std::span<const char8_t> span{text, N - 1};
          return Text<TUnicode>{std::as_bytes(span), Encoding::UTF8};
     }

     struct ILocale
     {
          virtual ~ILocale() = default;
          ILocale(void) = default;
          ILocale(const ILocale&) = delete;
          ILocale(ILocale&&) noexcept = delete;
          ILocale& operator=(const ILocale&) = delete;
          ILocale& operator=(ILocale&&) noexcept = delete;

          [[nodiscard]] virtual std::uint32_t ToLower(std::uint32_t codePoint) const = 0;
          [[nodiscard]] virtual std::uint32_t ToUpper(std::uint32_t codePoint) const = 0;

          [[nodiscard]] virtual std::span<const std::uint32_t> CanonicalDecompose(
              std::uint32_t codePoint, std::array<std::uint32_t, 3>& buffer) const = 0;
          [[nodiscard]] virtual std::span<const std::uint32_t> CompatibilityDecompose(
              std::uint32_t codePoint, std::array<std::uint32_t, 8>& buffer) const = 0;
          [[nodiscard]] virtual std::uint32_t CanonicalCompose(std::uint32_t lhs, std::uint32_t rhs) const = 0;
          [[nodiscard]] virtual std::uint8_t CombiningClass(std::uint32_t codePoint) const = 0;

          [[nodiscard]] virtual unicode::Direction DetectParagraphDirection(
              std::span<const std::uint32_t> codePoints) const = 0;
          [[nodiscard]] virtual unicode::BidiClass BidiClassOf(std::uint32_t codePoint) const = 0;

          [[nodiscard]] virtual unicode::GraphemeClusterBoundaryProperty GraphemeClusterBoundaryPropertyOf(
              std::uint32_t codePoint) const = 0;
          [[nodiscard]] virtual bool IsGraphemeBreak(unicode::GraphemeClusterBoundaryProperty left,
                                                     unicode::GraphemeClusterBoundaryProperty right, bool oddRI,
                                                     bool extendedPictographic) const = 0;

          [[nodiscard]] virtual bool IsWhitespace(std::uint32_t codePoint) const = 0;
     };

     template <typename TUnicode> class LocaleAdapter final : public ILocale
     {
     public:
          [[nodiscard]] std::uint32_t ToLower(std::uint32_t codePoint) const override
          { return TUnicode::ToLower(codePoint); }

          [[nodiscard]] std::uint32_t ToUpper(std::uint32_t codePoint) const override
          { return TUnicode::ToUpper(codePoint); }

          [[nodiscard]] std::span<const std::uint32_t> CanonicalDecompose(
              std::uint32_t codePoint, std::array<std::uint32_t, 3>& buffer) const override
          { return TUnicode::CanonicalDecompose(codePoint, buffer); }

          [[nodiscard]] std::span<const std::uint32_t> CompatibilityDecompose(
              std::uint32_t codePoint, std::array<std::uint32_t, 8>& buffer) const override
          { return TUnicode::CompatibilityDecompose(codePoint, buffer); }

          [[nodiscard]] std::uint32_t CanonicalCompose(std::uint32_t lhs, std::uint32_t rhs) const override
          { return TUnicode::CanonicalCompose(lhs, rhs); }

          [[nodiscard]] std::uint8_t CombiningClass(std::uint32_t codePoint) const override
          { return TUnicode::CombiningClass(codePoint); }

          [[nodiscard]] unicode::Direction DetectParagraphDirection(
              std::span<const std::uint32_t> codePoints) const override
          { return TUnicode::DetectParagraphDirection(codePoints); }

          [[nodiscard]] unicode::BidiClass BidiClassOf(std::uint32_t codePoint) const override
          { return TUnicode::GetBidiClass(codePoint); }

          [[nodiscard]] unicode::GraphemeClusterBoundaryProperty GraphemeClusterBoundaryPropertyOf(
              std::uint32_t codePoint) const override
          { return unicode::GraphemeClusterBoundaryPropertyOf(codePoint); }

          [[nodiscard]] bool IsGraphemeBreak(unicode::GraphemeClusterBoundaryProperty left,
                                             unicode::GraphemeClusterBoundaryProperty right, bool oddRI,
                                             bool extendedPictographic) const override
          { return unicode::IsGraphemeBreak(left, right, oddRI, extendedPictographic); }

          [[nodiscard]] bool IsWhitespace(std::uint32_t codePoint) const override
          { return TUnicode::IsWhitespace(codePoint); }
     };

     namespace detail
     {
          inline std::shared_ptr<const ILocale>& GlobalLocaleRef() noexcept
          {
               static std::shared_ptr<const ILocale> instance = std::make_shared<LocaleAdapter<unicode::UnicodeRoot>>();
               return instance;
          }
     } // namespace detail

     [[nodiscard]] inline std::shared_ptr<const ILocale> GetGlobalLocale() noexcept
     { return detail::GlobalLocaleRef(); }

     inline void SetGlobalLocale(std::shared_ptr<const ILocale> locale) noexcept
     { detail::GlobalLocaleRef() = std::move(locale); }

     template <typename TUnicode> [[nodiscard]] std::shared_ptr<const ILocale> MakeLocale()
     { return std::make_shared<LocaleAdapter<TUnicode>>(); }

     struct DynamicText
     {
          struct DecodedEntry
          {
               std::uint8_t run;
               std::uint32_t codepoint;
               std::size_t indexBitPos;
               std::size_t payloadBitPos;
          };

          std::size_t indexSizeInPoints{};
          std::size_t indexSizeInBits{};
          std::unique_ptr<std::byte[]> data{}; // NOLINT
          mutable std::vector<std::size_t> graphemeStarts;
          std::shared_ptr<const ILocale> locale;

          [[nodiscard]] const ILocale& Locale() const noexcept { return *locale; }

          void SetLocale(std::shared_ptr<const ILocale> newLocale)
          {
               locale = std::move(newLocale);
               graphemeStarts.clear();
          }

          [[nodiscard]] DynamicText WithLocale(std::shared_ptr<const ILocale> newLocale) const
          {
               DynamicText copy{*this};
               copy.SetLocale(std::move(newLocale));
               return copy;
          }

          template <typename TUnicode> void SetLocale() { SetLocale(MakeLocale<TUnicode>()); }

          template <typename TUnicode> [[nodiscard]] DynamicText WithLocale() const
          { return WithLocale(MakeLocale<TUnicode>()); }

          [[nodiscard]] constexpr std::size_t SizeInCodepoints(void) const noexcept { return indexSizeInPoints; }
          [[nodiscard]] constexpr std::size_t SizeInIndexBits(void) const noexcept { return indexSizeInBits; }
          [[nodiscard]] constexpr std::size_t SizeInGraphemes(void) const noexcept
          {
               EnsureGraphemeIndex();
               return graphemeStarts.empty() ? 0u : graphemeStarts.size() - 1u;
          }
          [[nodiscard]] constexpr std::size_t Size(void) const noexcept { return SizeInGraphemes(); }
          [[nodiscard]] constexpr bool IsEmpty(void) const noexcept { return indexSizeInPoints == 0; }

          [[nodiscard]] unicode::Direction Direction(void) const
          {
               const auto codePoints = AllCodepoints();
               return Locale().DetectParagraphDirection(codePoints);
          }

          DynamicText(void) : locale{GetGlobalLocale()} {}

          DynamicText(const DynamicText& other)
              : indexSizeInPoints{other.indexSizeInPoints}, indexSizeInBits{other.indexSizeInBits},
                graphemeStarts{other.graphemeStarts}, locale{other.locale}
          {
               if (other.data)
               {
                    const std::size_t indexBits = indexSizeInBits;
                    std::size_t payloadBits = 0;
                    other.ForEach(
                        [&](const DecodedEntry& entry)
                        {
                             payloadBits += RunWidth(entry.run);
                             return true;
                        });
                    const std::size_t totalBytes = ByteCount(indexBits, payloadBits);
                    data = std::make_unique_for_overwrite<std::byte[]>(totalBytes); // NOLINT
                    std::copy_n(other.data.get(), totalBytes, data.get());
               }
          }

          DynamicText(DynamicText&& other) noexcept
              : indexSizeInPoints{other.indexSizeInPoints}, indexSizeInBits{other.indexSizeInBits},
                data{std::move(other.data)}, graphemeStarts{std::move(other.graphemeStarts)},
                locale{std::move(other.locale)}
          {
               other.indexSizeInPoints = 0;
               other.indexSizeInBits = 0;
          }

          DynamicText& operator=(const DynamicText& other)
          {
               if (this == &other) return *this;
               DynamicText tmp{other};
               *this = std::move(tmp);
               return *this;
          }

          DynamicText& operator=(DynamicText&& other) noexcept
          {
               if (this == &other) return *this;
               indexSizeInPoints = other.indexSizeInPoints;
               indexSizeInBits = other.indexSizeInBits;
               data = std::move(other.data);
               graphemeStarts = std::move(other.graphemeStarts);
               locale = std::move(other.locale);
               other.indexSizeInPoints = 0;
               other.indexSizeInBits = 0;
               return *this;
          }

          ~DynamicText(void) = default;

          explicit DynamicText(const std::span<const std::byte> bytes, const Encoding sourceEncoding,
                               std::shared_ptr<const ILocale> loc = GetGlobalLocale())
              : locale{std::move(loc)}
          {
               if (bytes.empty()) return;
               switch (sourceEncoding)
               {
               case Encoding::UTF8: ParseUTF8(bytes); break;
               default: break;
               }
          }

          bool All(std::invocable<const DecodedEntry&> auto&& visitor) const
          {
               if (!data || indexSizeInPoints == 0) return true;
               bool result = true;
               ForEach(
                   [&](const DecodedEntry& entry)
                   {
                        result = std::invoke(visitor, entry);
                        return result;
                   });
               return result;
          }

          constexpr bool Some(std::invocable<const DecodedEntry&> auto&& visitor) const
          {
               if (!data || indexSizeInPoints == 0) return false;
               bool found = false;
               ForEach(
                   [&](const DecodedEntry& entry)
                   {
                        found = std::invoke(visitor, entry);
                        return !found;
                   });
               return found;
          }

          template <typename TReduced>
          auto Reduce(std::invocable<TReduced, std::uint32_t> auto&& visitor, TReduced initial) const
          {
               ForEach(
                   [&](const DecodedEntry& entry)
                   {
                        initial = std::invoke(visitor, std::move(initial), entry.codepoint);
                        return true;
                   });
               return initial;
          }

     private:
          [[nodiscard]] constexpr bool ReadBit(std::size_t pos) const noexcept
          {
               const std::size_t byteOffset = pos >> 3;
               const std::size_t bitOffset = 7u - (pos & 7u);
               return (std::to_underlying(data[byteOffset]) & (1u << bitOffset)) != 0;
          }

          static constexpr std::size_t RunWidth(std::size_t run) noexcept
          { return run == 1u ? 7u : run == 2u ? 11u : run == 3u ? 16u : 21u; }

          static constexpr std::uint8_t CodepointWidth(std::uint32_t codePoint) noexcept
          {
               if (codePoint <= 0x7Fu) return 1u;
               if (codePoint <= 0x7FFu) return 2u;
               if (codePoint <= 0xFFFFu) return 3u;
               return 4u;
          }

          [[nodiscard]] constexpr std::uint32_t ReadBits(std::size_t bitPos, std::size_t count) const noexcept
          {
               std::uint32_t value{};
               for (std::size_t i = 0; i < count; i++)
                    value = (value << 1) | static_cast<std::uint32_t>(ReadBit(bitPos + i));
               return value;
          }

          static constexpr void StaticWriteBit(std::byte* storage, std::size_t& bitPos, bool value) noexcept
          {
               const std::size_t byteOffset = bitPos >> 3;
               const std::size_t bitOffset = 7u - (bitPos & 7u);
               const std::byte mask{static_cast<unsigned char>(1u << bitOffset)};
               if (value) storage[byteOffset] |= mask;
               else
                    storage[byteOffset] &= ~mask;
               bitPos++;
          }

          static constexpr void StaticWriteBits(std::byte* storage, std::size_t& bitPos, std::uint32_t value,
                                                std::size_t count) noexcept
          {
               for (std::size_t i = 0; i < count; i++)
               {
                    const bool bit = static_cast<bool>((value >> (count - 1u - i)) & 1u);
                    StaticWriteBit(storage, bitPos, bit);
               }
          }

          constexpr void ForEach(std::invocable<DecodedEntry> auto&& visitor) const
          {
               if (data == nullptr || indexSizeInPoints == 0) return;
               std::size_t headerBitCursor = 0;
               std::size_t payloadBitPos = indexSizeInBits;
               std::size_t codePoint = 0;

               while (headerBitCursor < indexSizeInBits)
               {
                    const bool state = ReadBit(headerBitCursor);
                    std::size_t runLen = 0;
                    while (headerBitCursor < indexSizeInBits && ReadBit(headerBitCursor) == state)
                    {
                         runLen++;
                         headerBitCursor++;
                    }
                    const std::size_t width = RunWidth(runLen);
                    DecodedEntry entry{
                        .run = static_cast<std::uint8_t>(runLen),
                        .codepoint = ReadBits(payloadBitPos, width),
                        .indexBitPos = headerBitCursor - runLen,
                        .payloadBitPos = payloadBitPos,
                    };
                    payloadBitPos += width;
                    codePoint += runLen;
                    if (!std::invoke(visitor, entry)) return;
               }
          }

          [[nodiscard]] constexpr std::vector<DecodedEntry> BuildRawIndex(void) const
          {
               std::vector<DecodedEntry> table;
               table.reserve(indexSizeInPoints);
               ForEach(
                   [&](const DecodedEntry& entry)
                   {
                        table.push_back(entry);
                        return true;
                   });
               return table;
          }

          struct FromDecoded
          {
          };

          explicit DynamicText([[maybe_unused]] FromDecoded tag, const std::vector<DecodedEntry>& entries,
                               std::shared_ptr<const ILocale> loc)
              : locale{std::move(loc)}
          {
               if (entries.empty()) return;

               std::size_t indexBitCount = 0;
               std::size_t payloadBitCount = 0;
               for (const auto& entry : entries)
               {
                    indexBitCount += entry.run;
                    payloadBitCount += RunWidth(entry.run);
               }

               indexSizeInPoints = entries.size();
               const std::size_t totalBytes = (indexBitCount + payloadBitCount + 7) / 8;
               data = std::make_unique_for_overwrite<std::byte[]>(totalBytes); // NOLINT
               std::fill_n(data.get(), totalBytes, std::byte{0});

               std::size_t bitPos = 0;
               bool state = true;
               for (const auto& entry : entries)
               {
                    for (std::uint8_t i = 0; i < entry.run; i++) StaticWriteBit(data.get(), bitPos, state);
                    state = !state;
               }
               indexSizeInBits = bitPos;

               for (const auto& entry : entries)
               {
                    const std::size_t width = RunWidth(entry.run);
                    StaticWriteBits(data.get(), bitPos, entry.codepoint, width);
               }
          }

          [[nodiscard]] DynamicText MakeFromDecoded(const std::vector<DecodedEntry>& entries) const
          { return DynamicText{FromDecoded{}, entries, locale}; }

          void ParseUTF8(std::span<const std::byte> src)
          {
               constexpr std::uint8_t mask1MSB = 0b10000000u;
               constexpr std::uint8_t mask11MSB = 0b11000000u;
               constexpr std::uint8_t mask111MSB = 0b11100000u;
               constexpr std::uint8_t mask1111MSB = 0b11110000u;
               constexpr std::uint8_t mask11111MSB = 0b11111000u;

               constexpr auto isContinuation = [](std::uint8_t byte) constexpr
               { return (byte & mask11MSB) == mask1MSB; };

               std::vector<std::uint8_t> runs;
               std::vector<std::uint32_t> payload;
               runs.reserve(src.size());
               payload.reserve(src.size());

               std::size_t indexPointCount{};
               std::size_t indexBitCount{};
               std::size_t payloadBitCount{};

               for (auto it = src.begin(); it != src.end();)
               {
                    const auto nextByte = static_cast<std::uint8_t>(*it);
                    if ((nextByte & mask1MSB) == 0)
                    {
                         indexPointCount++;
                         indexBitCount += 1;
                         payloadBitCount += 7;
                         runs.emplace_back(1);
                         payload.emplace_back(nextByte & 0x7Fu);
                         it++;
                         continue;
                    }
                    if ((nextByte & mask111MSB) == mask11MSB)
                    {
                         if (std::next(it) == src.end() || !isContinuation(static_cast<std::uint8_t>(*std::next(it))))
                              break;
                         const auto byte1 = static_cast<std::uint8_t>(*std::next(it));
                         indexPointCount++;
                         indexBitCount += 2;
                         payloadBitCount += 11;
                         runs.emplace_back(2);
                         payload.emplace_back(((nextByte & 0x1Fu) << 6) | (byte1 & 0x3Fu));
                         it += 2;
                         continue;
                    }
                    if ((nextByte & mask1111MSB) == mask111MSB)
                    {
                         if (std::distance(it, src.end()) < 3) break;
                         const auto byte1 = static_cast<std::uint8_t>(*std::next(it, 1));
                         const auto byte2 = static_cast<std::uint8_t>(*std::next(it, 2));
                         if (!isContinuation(byte1) || !isContinuation(byte2)) break;
                         indexPointCount++;
                         indexBitCount += 3;
                         payloadBitCount += 16;
                         runs.emplace_back(3);
                         payload.emplace_back(((nextByte & 0x0Fu) << 12) | ((byte1 & 0x3Fu) << 6) | (byte2 & 0x3Fu));
                         it += 3;
                         continue;
                    }
                    if ((nextByte & mask11111MSB) == mask1111MSB)
                    {
                         if (std::distance(it, src.end()) < 4) break;
                         const auto byte1 = static_cast<std::uint8_t>(*std::next(it, 1));
                         const auto byte2 = static_cast<std::uint8_t>(*std::next(it, 2));
                         const auto byte3 = static_cast<std::uint8_t>(*std::next(it, 3));
                         if (!isContinuation(byte1) || !isContinuation(byte2) || !isContinuation(byte3)) break;
                         indexPointCount++;
                         indexBitCount += 4;
                         payloadBitCount += 21;
                         runs.emplace_back(4);
                         payload.emplace_back(((nextByte & 0x07u) << 18) | ((byte1 & 0x3Fu) << 12) |
                                              ((byte2 & 0x3Fu) << 6) | (byte3 & 0x3Fu));
                         it += 4;
                         continue;
                    }
                    it++;
               }

               indexSizeInPoints = indexPointCount;
               const std::size_t totalBytes = (indexBitCount + payloadBitCount + 7) / 8;
               data = std::make_unique_for_overwrite<std::byte[]>(totalBytes); // NOLINT
               std::fill_n(data.get(), totalBytes, std::byte{0});

               std::size_t bitPos{};
               bool bitState = true;
               for (auto length : runs)
               {
                    for (std::uint8_t i = 0; i < length; i++) StaticWriteBit(data.get(), bitPos, bitState);
                    bitState = !bitState;
               }
               indexSizeInBits = bitPos;

               std::size_t runIndex{};
               for (auto codePoint : payload)
               {
                    const auto run = runs[runIndex++];
                    const std::size_t width = run == 1 ? 7u : run == 2 ? 11u : run == 3 ? 16u : 21u;
                    StaticWriteBits(data.get(), bitPos, codePoint, width);
               }
          }

          void EnsureGraphemeIndex(void) const
          {
               if (!graphemeStarts.empty()) return;

               std::vector<std::uint32_t> codePoints;
               codePoints.reserve(indexSizeInPoints);
               ForEach(
                   [&](const DecodedEntry& entry)
                   {
                        codePoints.push_back(entry.codepoint);
                        return true;
                   });

               graphemeStarts.clear();
               if (codePoints.empty()) return;

               graphemeStarts.push_back(0);
               bool oddRI = false;
               const auto& loc = Locale();

               auto IsGB10 = [&](std::size_t index)
               {
                    const auto right = loc.GraphemeClusterBoundaryPropertyOf(codePoints[index]);
                    if (right != unicode::GraphemeClusterBoundaryProperty::EmojiModifier) return false;
                    for (std::size_t j = index; j > 0; j--)
                    {
                         const auto property = loc.GraphemeClusterBoundaryPropertyOf(codePoints[j - 1]);
                         if (property == unicode::GraphemeClusterBoundaryProperty::Extend) continue;
                         return property == unicode::GraphemeClusterBoundaryProperty::ExtPic;
                    }
                    return false;
               };

               auto IsGB11 = [&](std::size_t index)
               {
                    const auto left = loc.GraphemeClusterBoundaryPropertyOf(codePoints[index - 1]);
                    const auto right = loc.GraphemeClusterBoundaryPropertyOf(codePoints[index]);
                    if (left != unicode::GraphemeClusterBoundaryProperty::ZeroWidthJoiner) return false;
                    if (right != unicode::GraphemeClusterBoundaryProperty::ExtPic) return false;
                    for (std::size_t j = index - 1; j > 0; j--)
                    {
                         const auto property = loc.GraphemeClusterBoundaryPropertyOf(codePoints[j - 1]);
                         if (property == unicode::GraphemeClusterBoundaryProperty::Extend ||
                             property == unicode::GraphemeClusterBoundaryProperty::EmojiModifier)
                              continue;
                         return property == unicode::GraphemeClusterBoundaryProperty::ExtPic;
                    }
                    return false;
               };

               for (std::size_t i = 1; i < codePoints.size(); i++)
               {
                    const auto left = loc.GraphemeClusterBoundaryPropertyOf(codePoints[i - 1]);
                    const auto right = loc.GraphemeClusterBoundaryPropertyOf(codePoints[i]);
                    if (left == unicode::GraphemeClusterBoundaryProperty::RegionalIndicator) oddRI = !oddRI;
                    else
                         oddRI = false;
                    if (IsGB10(i)) continue;
                    if (IsGB11(i)) continue;
                    if (loc.IsGraphemeBreak(left, right, oddRI, false))
                    {
                         graphemeStarts.push_back(i);
                         oddRI = false;
                    }
               }
               graphemeStarts.push_back(codePoints.size());
          }

          [[nodiscard]] std::vector<std::uint32_t> AllCodepoints(void) const
          {
               std::vector<std::uint32_t> out;
               out.reserve(indexSizeInPoints);
               ForEach(
                   [&](const DecodedEntry& entry)
                   {
                        out.push_back(entry.codepoint);
                        return true;
                   });
               return out;
          }

          [[nodiscard]] static std::size_t ByteCount(std::size_t indexBits, std::size_t payloadBits) noexcept
          { return (indexBits + payloadBits + 7u) / 8u; }

          [[nodiscard]] std::pair<std::size_t, std::size_t> GraphemeToCodepointRange(std::size_t graphemeIndex) const
          {
               EnsureGraphemeIndex();
               if (graphemeIndex + 1 >= graphemeStarts.size())
                    throw std::out_of_range{std::format("DynamicText: grapheme index {} out of range (size {})",
                                                        graphemeIndex, SizeInGraphemes())};
               return {graphemeStarts[graphemeIndex], graphemeStarts[graphemeIndex + 1]};
          }

     public:
          [[nodiscard]] std::uint32_t operator[](std::size_t graphemeIndex) const
          {
               const auto [cpStart, cpEnd] = GraphemeToCodepointRange(graphemeIndex);
               std::uint32_t result{};
               std::size_t current{};
               ForEach(
                   [&](const DecodedEntry& entry) -> bool
                   {
                        if (current == cpStart)
                        {
                             result = entry.codepoint;
                             return false;
                        }
                        current++;
                        return true;
                   });
               return result;
          }

          [[nodiscard]] std::vector<std::uint32_t> GraphemeAt(std::size_t graphemeIndex) const
          {
               const auto [cpStart, cpEnd] = GraphemeToCodepointRange(graphemeIndex);
               std::vector<std::uint32_t> out;
               out.reserve(cpEnd - cpStart);
               std::size_t current{};
               ForEach(
                   [&](const DecodedEntry& entry) -> bool
                   {
                        if (current >= cpStart && current < cpEnd) out.push_back(entry.codepoint);
                        current++;
                        return current < cpEnd;
                   });
               return out;
          }

          [[nodiscard]] std::vector<DecodedEntry> GraphemeAtEx(const std::size_t graphemeIndex) const
          {
               const auto [cpStart, cpEnd] = GraphemeToCodepointRange(graphemeIndex);
               std::vector<DecodedEntry> out;
               out.reserve(cpEnd - cpStart);
               std::size_t current{};
               ForEach(
                   [&](const DecodedEntry& entry) -> bool
                   {
                        if (current >= cpStart && current < cpEnd) out.push_back(entry);
                        current++;
                        return current < cpEnd;
                   });
               return out;
          }

          [[nodiscard]] DynamicText operator[](decltype(0z) start, decltype(0z) length) const
          {
               const auto originalSize = static_cast<decltype(start)>(SizeInGraphemes());
               if (start >= 0)
               {
                    if (length == 0) return Substring(start);
                    if (length > 0) return Substring(start, length);
                    return Substring(start, originalSize + length);
               }
               const auto startPoint = originalSize + start;
               if (length == 0) return Substring(startPoint);
               if (length > 0) return Substring(startPoint, length);
               return Substring(startPoint, originalSize - length);
          }

          [[nodiscard]] DynamicText Substring(const std::size_t startGrapheme,
                                              const std::size_t countGraphemes = std::dynamic_extent) const
          {
               EnsureGraphemeIndex();
               const std::size_t totalG = SizeInGraphemes();
               if (startGrapheme >= totalG || countGraphemes == 0) return DynamicText{};

               const std::size_t clampedCount = (countGraphemes == std::dynamic_extent)
                                                    ? totalG - startGrapheme
                                                    : std::min(countGraphemes, totalG - startGrapheme);

               const std::size_t cpStart = graphemeStarts[startGrapheme];
               const std::size_t cpEnd = graphemeStarts[startGrapheme + clampedCount];

               std::vector<DecodedEntry> slice;
               slice.reserve(cpEnd - cpStart);
               std::size_t current{};
               ForEach(
                   [&](const DecodedEntry& entry) -> bool
                   {
                        if (current >= cpStart && current < cpEnd) slice.push_back(entry);
                        current++;
                        return current < cpEnd;
                   });
               return MakeFromDecoded(slice);
          }

          [[nodiscard]] DynamicText SubstringCodePoints(const std::size_t startCP,
                                                        const std::size_t countCP = std::dynamic_extent) const
          {
               if (startCP >= indexSizeInPoints || countCP == 0) return DynamicText{};
               const std::size_t clamped = (countCP == std::dynamic_extent)
                                               ? indexSizeInPoints - startCP
                                               : std::min(countCP, indexSizeInPoints - startCP);

               std::vector<DecodedEntry> slice;
               slice.reserve(clamped);
               std::size_t current{};
               ForEach(
                   [&](const DecodedEntry& entry) -> bool
                   {
                        if (current >= startCP && current < startCP + clamped) slice.push_back(entry);
                        current++;
                        return current < startCP + clamped;
                   });
               return MakeFromDecoded(slice);
          }

          [[nodiscard]] DynamicText LogicalToVisual(void) const
          {
               EnsureGraphemeIndex();
               const std::size_t sizeInGraphemes = SizeInGraphemes();
               if (sizeInGraphemes == 0) return DynamicText{};

               const auto& loc = Locale();
               const auto dir = Direction();
               bool paraRTL = (dir == unicode::Direction::RTL);

               std::vector<std::uint8_t> levels(sizeInGraphemes);
               for (std::size_t i = 0; i < sizeInGraphemes; i++)
               {
                    const auto codePoint = operator[](i);
                    const auto bidiClass = loc.BidiClassOf(codePoint);
                    using Class = unicode::BidiClass;
                    const bool isRTL = (bidiClass == Class::R || bidiClass == Class::AL || bidiClass == Class::AN);
                    if (bidiClass == Class::RLO) paraRTL = true;
                    else if (bidiClass == Class::LRO)
                         paraRTL = false;
                    levels[i] = (paraRTL ? 1u : 0u);
                    if (isRTL) levels[i] = 1;
                    else if (bidiClass == Class::L)
                         levels[i] = 0;
               }

               std::vector<std::size_t> order(sizeInGraphemes);
               std::ranges::iota(order, 0u);

               const std::uint8_t maxLevel = *std::ranges::max_element(levels);
               for (std::uint8_t level = maxLevel; level >= 1u; --level)
               {
                    std::size_t runStart = std::numeric_limits<std::size_t>::max();
                    for (std::size_t i = 0; i <= sizeInGraphemes; i++)
                    {
                         const bool inRun = (i < sizeInGraphemes && levels[order[i]] >= level);
                         if (inRun && runStart == std::numeric_limits<std::size_t>::max()) runStart = i;
                         else if (!inRun && runStart != std::numeric_limits<std::size_t>::max())
                         {
                              std::reverse(order.begin() + static_cast<std::ptrdiff_t>(runStart),
                                           order.begin() + static_cast<std::ptrdiff_t>(i));
                              runStart = std::numeric_limits<std::size_t>::max();
                         }
                    }
               }

               const auto rawIndex = BuildRawIndex();
               std::vector<DecodedEntry> reordered;
               reordered.reserve(indexSizeInPoints);
               for (const auto graphemeIndex : order)
               {
                    const std::size_t cpStart = graphemeStarts[graphemeIndex];
                    const std::size_t cpEnd = graphemeStarts[graphemeIndex + 1];
                    for (std::size_t ci = cpStart; ci < cpEnd; ci++) reordered.push_back(rawIndex[ci]);
               }
               return MakeFromDecoded(reordered);
          }

          [[nodiscard]] std::u8string ToUTF8(void) const
          {
               std::u8string out;
               out.reserve(indexSizeInPoints * 2);
               ForEach(
                   [&](const DecodedEntry& entry) -> bool
                   {
                        const auto codePoint = entry.codepoint;
                        if (entry.run == 1) out.push_back(static_cast<char8_t>(codePoint & 0x7Fu));
                        else if (entry.run == 2)
                        {
                             out.push_back(static_cast<char8_t>(0xC0u | ((codePoint >> 6) & 0x1Fu)));
                             out.push_back(static_cast<char8_t>(0x80u | (codePoint & 0x3Fu)));
                        }
                        else if (entry.run == 3)
                        {
                             out.push_back(static_cast<char8_t>(0xE0u | ((codePoint >> 12) & 0x0Fu)));
                             out.push_back(static_cast<char8_t>(0x80u | ((codePoint >> 6) & 0x3Fu)));
                             out.push_back(static_cast<char8_t>(0x80u | (codePoint & 0x3Fu)));
                        }
                        else
                        {
                             out.push_back(static_cast<char8_t>(0xF0u | ((codePoint >> 18) & 0x07u)));
                             out.push_back(static_cast<char8_t>(0x80u | ((codePoint >> 12) & 0x3Fu)));
                             out.push_back(static_cast<char8_t>(0x80u | ((codePoint >> 6) & 0x3Fu)));
                             out.push_back(static_cast<char8_t>(0x80u | (codePoint & 0x3Fu)));
                        }
                        return true;
                   });
               return out;
          }

          [[nodiscard]] std::u16string ToUTF16(void) const
          {
               std::u16string out;
               out.reserve(indexSizeInPoints);
               ForEach(
                   [&](const DecodedEntry& entry) -> bool
                   {
                        const auto codePoint = entry.codepoint;
                        if (codePoint < 0x10000u) { out.push_back(static_cast<char16_t>(codePoint)); }
                        else
                        {
                             const std::uint32_t value = codePoint - 0x10000u;
                             out.push_back(static_cast<char16_t>(0xD800u | (value >> 10)));
                             out.push_back(static_cast<char16_t>(0xDC00u | (value & 0x3FFu)));
                        }
                        return true;
                   });
               return out;
          }

          [[nodiscard]] std::u32string ToUTF32(void) const
          {
               std::u32string out;
               out.reserve(indexSizeInPoints);
               ForEach(
                   [&](const DecodedEntry& entry) -> bool
                   {
                        out.push_back(static_cast<char32_t>(entry.codepoint));
                        return true;
                   });
               return out;
          }

          static constexpr std::size_t npos = std::numeric_limits<std::size_t>::max();

          [[nodiscard]] std::size_t Find(std::uint32_t codepoint, std::size_t startGrapheme = 0) const
          {
               EnsureGraphemeIndex();
               const std::size_t sizeInGraphemes = SizeInGraphemes();
               for (std::size_t graphemeIndex = startGrapheme; graphemeIndex < sizeInGraphemes; graphemeIndex++)
                    if (operator[](graphemeIndex) == codepoint) return graphemeIndex;
               return npos;
          }

          [[nodiscard]] std::size_t Find(const DynamicText& substring, std::size_t startGrapheme = 0) const
          {
               EnsureGraphemeIndex();
               const std::size_t sizeInGraphemes = SizeInGraphemes();
               const std::size_t subSize = substring.SizeInGraphemes();
               if (subSize == 0) return startGrapheme;
               if (subSize > sizeInGraphemes) return npos;
               for (std::size_t graphemeIndex = startGrapheme; graphemeIndex <= sizeInGraphemes - subSize;
                    graphemeIndex++)
               {
                    bool match = true;
                    for (std::size_t sgi = 0; sgi < subSize; sgi++)
                    {
                         if (operator[](graphemeIndex + sgi) != substring[sgi])
                         {
                              match = false;
                              break;
                         }
                    }
                    if (match) return graphemeIndex;
               }
               return npos;
          }

          [[nodiscard]] bool Contains(std::uint32_t codepoint) const
          { return Find(codepoint) != npos; } // NOLINT(readability-container-contains)

          [[nodiscard]] bool Contains(const DynamicText& substring) const
          { return Find(substring) != npos; } // NOLINT(readability-container-contains)

          [[nodiscard]] bool StartsWith(std::uint32_t codepoint) const
          {
               if (IsEmpty()) return false;
               return operator[](0) == codepoint;
          }

          [[nodiscard]] bool StartsWith(const DynamicText& other) const
          {
               EnsureGraphemeIndex();
               const std::size_t subSize = other.SizeInGraphemes();
               if (subSize == 0 || subSize > SizeInGraphemes()) return false;
               for (std::size_t sgi = 0; sgi < subSize; sgi++)
                    if (operator[](sgi) != other[sgi]) return false;
               return true;
          }

          [[nodiscard]] bool EndsWith(std::uint32_t codepoint) const
          {
               if (IsEmpty()) return false;
               return operator[](SizeInGraphemes() - 1) == codepoint;
          }

          [[nodiscard]] bool EndsWith(const DynamicText& other) const
          {
               const std::size_t subSize = other.SizeInGraphemes();
               const std::size_t selfSize = SizeInGraphemes();
               if (subSize == 0) return true;
               if (subSize > selfSize) return false;
               const std::size_t offset = selfSize - subSize;
               for (std::size_t sgi = 0; sgi < subSize; sgi++)
                    if (operator[](offset + sgi) != other[sgi]) return false;
               return true;
          }

          [[nodiscard]] DynamicText Trim(std::predicate<std::uint32_t> auto&& pred) const
          {
               EnsureGraphemeIndex();
               const std::size_t sizeInGraphemes = SizeInGraphemes();
               std::size_t first = 0;
               while (first < sizeInGraphemes && pred(operator[](first))) first++;
               std::size_t last = sizeInGraphemes;
               while (last > first && pred(operator[](last - 1))) last--;
               return Substring(first, last - first);
          }

          [[nodiscard]] DynamicText TrimWhitespace(void) const
          {
               return Trim([this](const std::uint32_t codePoint) { return Locale().IsWhitespace(codePoint); });
          }

          [[nodiscard]] DynamicText Concat(const DynamicText& other) const
          {
               auto left = BuildRawIndex();
               auto right = other.BuildRawIndex();
               left.insert(left.end(), right.begin(), right.end());
               return MakeFromDecoded(left);
          }

          DynamicText operator+(const DynamicText& other) const { return Concat(other); }

          DynamicText& operator+=(const DynamicText& other)
          {
               *this = Concat(other);
               return *this;
          }

          [[nodiscard]] DynamicText ToReversed(void) const
          {
               EnsureGraphemeIndex();
               const std::size_t sizeInGraphemes = SizeInGraphemes();
               if (sizeInGraphemes == 0) return DynamicText{};

               const auto rawIndex = BuildRawIndex();
               std::vector<DecodedEntry> reversed;
               reversed.reserve(indexSizeInPoints);
               for (std::size_t graphemeIndex = sizeInGraphemes; graphemeIndex-- > 0;)
               {
                    const std::size_t cpStart = graphemeStarts[graphemeIndex];
                    const std::size_t cpEnd = graphemeStarts[graphemeIndex + 1];
                    for (std::size_t ci = cpStart; ci < cpEnd; ci++) reversed.push_back(rawIndex[ci]);
               }
               return MakeFromDecoded(reversed);
          }

          [[nodiscard]] std::vector<DynamicText> Split(const std::uint32_t delimiter) const
          {
               EnsureGraphemeIndex();
               const std::size_t sizeInGraphemes = SizeInGraphemes();
               std::vector<DynamicText> result;
               std::size_t segStart = 0;
               for (std::size_t graphemeIndex = 0; graphemeIndex <= sizeInGraphemes; graphemeIndex++)
               {
                    if (graphemeIndex == sizeInGraphemes || operator[](graphemeIndex) == delimiter)
                    {
                         result.push_back(Substring(segStart, graphemeIndex - segStart));
                         segStart = graphemeIndex + 1;
                    }
               }
               return result;
          }

          [[nodiscard]] std::vector<DynamicText> Split(const DynamicText& delimiter) const
          {
               const std::size_t sizeInGraphemes = SizeInGraphemes();
               const std::size_t delimSize = delimiter.SizeInGraphemes();
               std::vector<DynamicText> result;
               if (delimSize == 0)
               {
                    result.push_back(*this);
                    return result;
               }
               std::size_t segStart = 0;
               std::size_t graphemeIndex = 0;
               while (graphemeIndex <= sizeInGraphemes)
               {
                    const std::size_t found = Find(delimiter, graphemeIndex);
                    if (found == npos)
                    {
                         result.push_back(Substring(segStart));
                         break;
                    }
                    result.push_back(Substring(segStart, found - segStart));
                    segStart = found + delimSize;
                    graphemeIndex = segStart;
               }
               return result;
          }

          [[nodiscard]] static DynamicText Join(std::span<const DynamicText> parts, const DynamicText& separator)
          {
               if (parts.empty()) return DynamicText{};
               DynamicText result = parts[0];
               for (std::size_t i = 1; i < parts.size(); i++) result = result.Concat(separator).Concat(parts[i]);
               return result;
          }

          [[nodiscard]] DynamicText Replace(std::uint32_t from, std::uint32_t with) const
          {
               return Replace([from, with](std::uint32_t codePoint) { return codePoint == from ? with : codePoint; });
          }

          [[nodiscard]] DynamicText Replace(const DynamicText& from, const DynamicText& with) const
          {
               if (from.IsEmpty()) return *this;
               const std::size_t sizeInGraphemes = SizeInGraphemes();
               const std::size_t fromSize = from.SizeInGraphemes();
               std::vector<DynamicText> parts;
               std::size_t pos = 0;
               while (pos <= sizeInGraphemes)
               {
                    const std::size_t found = Find(from, pos);
                    if (found == npos)
                    {
                         parts.push_back(Substring(pos));
                         break;
                    }
                    parts.push_back(Substring(pos, found - pos));
                    parts.push_back(with);
                    pos = found + fromSize;
               }
               if (parts.empty()) return *this;
               DynamicText result = parts[0];
               for (std::size_t i = 1; i < parts.size(); i++) result = result.Concat(parts[i]);
               return result;
          }

          [[nodiscard]] std::size_t Count(std::predicate<std::uint32_t> auto&& pred) const
          {
               std::size_t count = 0;
               const std::size_t sizeInGraphemes = SizeInGraphemes();
               for (std::size_t graphemeIndex = 0; graphemeIndex < sizeInGraphemes; graphemeIndex++)
                    if (pred(operator[](graphemeIndex))) count++;
               return count;
          }

          [[nodiscard]] std::size_t Count(std::uint32_t codepoint) const
          {
               return Count([codepoint](std::uint32_t codePoint) { return codePoint == codepoint; });
          }

          [[nodiscard]] DynamicText ToLower(void) const
          {
               return Replace([this](std::uint32_t codePoint) { return Locale().ToLower(codePoint); });
          }

          [[nodiscard]] DynamicText ToUpper(void) const
          {
               return Replace([this](std::uint32_t codePoint) { return Locale().ToUpper(codePoint); });
          }

          [[nodiscard]] bool operator==(const DynamicText& other) const
          {
               if (indexSizeInPoints != other.indexSizeInPoints) return false;
               const auto lhs = BuildRawIndex();
               const auto rhs = other.BuildRawIndex();
               for (std::size_t i = 0; i < lhs.size(); i++)
                    if (lhs[i].codepoint != rhs[i].codepoint) return false;
               return true;
          }

          [[nodiscard]] Comparison Compare(const DynamicText& other) const
          {
               if (*this == other) return Comparison::OrdinallyEquivalent;
               const auto lLower = ToLower();
               const auto rLower = other.ToLower();
               if (lLower == rLower) return Comparison::OrdinallyEquivalentIgnoreCase;
               const auto lNFC = ToNormalised(Normalisation::NFC);
               const auto rNFC = other.ToNormalised(Normalisation::NFC);
               if (lNFC == rNFC) return Comparison::CanonicallyEquivalent;
               const auto lNFKC = ToNormalised(Normalisation::NFKC);
               const auto rNFKC = other.ToNormalised(Normalisation::NFKC);
               if (lNFKC == rNFKC) return Comparison::CompatibilityEquivalent;
               if (lNFC.ToLower() == rNFC.ToLower()) return Comparison::CanonicallyEquivalentIgnoreCase;
               if (lNFKC.ToLower() == rNFKC.ToLower()) return Comparison::CompatibilityEquivalentIgnoreCase;
               return Comparison::NotEquivalent;
          }

          [[nodiscard]] Comparison operator<=>(const DynamicText& other) const { return Compare(other); }

          void Dump(std::size_t indent = 0) const
          {
               EnsureGraphemeIndex();
               const auto utf8 = ToUTF8();
               std::println("{}Glyph = '{}'", std::string(indent, ' '),
                            reinterpret_cast<const char*>(utf8.c_str())); // NOLINT
               std::print("{}Codepoints: {}  Graphemes: {}\n{}Header: ", std::string(indent, ' '), SizeInCodepoints(),
                          SizeInGraphemes(), std::string(indent, ' '));

               std::size_t bitPos{};
               bool state = true;
               std::size_t stateSize{};
               std::vector<std::size_t> runs{};
               const std::size_t headerBits = indexSizeInBits;

               for (; bitPos < headerBits; bitPos++)
               {
                    stateSize++;
                    const bool cur = ReadBit(bitPos);
                    if (state == cur) continue;
                    std::print("{}, ", stateSize - 1);
                    runs.push_back(stateSize - 1);
                    state = cur;
                    stateSize = 1;
               }
               if (stateSize != 0)
               {
                    std::print("{}", stateSize);
                    runs.push_back(stateSize);
               }
               std::println();

               std::size_t payBitPos = headerBits;
               for (auto run : runs)
               {
                    const std::size_t width = (run == 1) ? 7u : (run == 2) ? 11u : (run == 3) ? 16u : 21u;
                    std::print("{}[{}]: ", std::string(indent, ' '), payBitPos);
                    for (std::size_t i = 0; i < width; i++)
                    {
                         const std::size_t bitPosition = payBitPos + i;
                         std::print("{}", static_cast<int>(ReadBit(bitPosition)));
                    }
                    std::println();
                    payBitPos += width;
               }

               std::println("{}Grapheme clusters:", std::string(indent, ' '));
               const std::size_t sizeInGraphemes = SizeInGraphemes();
               for (std::size_t graphemeIndex = 0; graphemeIndex < sizeInGraphemes; graphemeIndex++)
               {
                    const auto codePoints = GraphemeAt(graphemeIndex);
                    std::print("{}  [{}]: U+{:04X}", std::string(indent, ' '), graphemeIndex, codePoints[0]);
                    for (std::size_t codePoint = 1; codePoint < codePoints.size(); codePoint++)
                         std::print(" + U+{:04X}", codePoints[codePoint]);
                    std::println();
               }
          }

          [[nodiscard]] DynamicText ToPaddedEnd(std::size_t size, std::convertible_to<char32_t> auto symbol) const
          {
               const auto codePoint = static_cast<std::uint32_t>(static_cast<char32_t>(symbol));
               auto decoded = BuildRawIndex();
               std::vector<DecodedEntry> padded;
               padded.reserve(size);
               for (std::size_t i = 0; i < size; i++)
                    padded.push_back(i < decoded.size()
                                         ? decoded[i]
                                         : DecodedEntry{.run = CodepointWidth(codePoint), .codepoint = codePoint});
               return MakeFromDecoded(padded);
          }

          [[nodiscard]] DynamicText ToPaddedStart(std::size_t size, std::convertible_to<char32_t> auto symbol) const
          {
               const auto codePoint = static_cast<std::uint32_t>(static_cast<char32_t>(symbol));
               auto decoded = BuildRawIndex();
               std::vector<DecodedEntry> padded;
               padded.reserve(size);
               for (std::size_t i = 0; i < size; i++)
                    padded.push_back(i < size - decoded.size()
                                         ? DecodedEntry{.run = CodepointWidth(codePoint), .codepoint = codePoint}
                                         : decoded[i - (size - decoded.size())]);
               return MakeFromDecoded(padded);
          }

          [[nodiscard]] DynamicText ToNormalised(const Normalisation normalisation = Normalisation::NFC) const
          {
               const bool isCompatibility =
                   (normalisation == Normalisation::NFKD || normalisation == Normalisation::NFKC);
               const auto& loc = Locale();

               const auto rawCPs = AllCodepoints();
               std::vector<std::uint32_t> decomposed{};
               decomposed.reserve(rawCPs.size() * 2);

               const auto expand = [&](this auto&& self, std::uint32_t codePoint) -> void
               {
                    if (isCompatibility)
                    {
                         std::array<std::uint32_t, 8> kompatibilityBuffer{};
                         const auto compatibileSequence = loc.CompatibilityDecompose(codePoint, kompatibilityBuffer);
                         if (!compatibileSequence.empty())
                         {
                              for (const auto codePoint : compatibileSequence) self(codePoint);
                              return;
                         }
                    }
                    std::array<std::uint32_t, 3> buffer{};
                    const auto sequence = loc.CanonicalDecompose(codePoint, buffer);
                    if (sequence.empty())
                    {
                         decomposed.push_back(codePoint);
                         return;
                    }
                    for (const auto codePoint : sequence) self(codePoint);
               };
               for (const auto codePoint : rawCPs) expand(codePoint);

               for (std::size_t i = 1; i < decomposed.size(); i++)
               {
                    const auto canonicalCombiningClass = loc.CombiningClass(decomposed[i]);
                    if (canonicalCombiningClass == 0) continue;
                    std::size_t decomposedIndex = i;
                    while (decomposedIndex > 0 &&
                           loc.CombiningClass(decomposed[decomposedIndex - 1]) > canonicalCombiningClass)
                    {
                         std::swap(decomposed[decomposedIndex], decomposed[decomposedIndex - 1]);
                         decomposedIndex--;
                    }
               }

               if (normalisation == Normalisation::NFD || normalisation == Normalisation::NFKD)
               {
                    std::vector<DecodedEntry> entries;
                    entries.reserve(decomposed.size());
                    for (const auto codePoint : decomposed)
                         entries.push_back(DecodedEntry{.run = CodepointWidth(codePoint), .codepoint = codePoint});
                    return MakeFromDecoded(entries);
               }

               std::vector<std::uint32_t> composed;
               composed.reserve(decomposed.size());
               for (const auto codePoint : decomposed)
               {
                    const auto canonicalCombiningClass = loc.CombiningClass(codePoint);
                    if (composed.empty())
                    {
                         composed.push_back(codePoint);
                         continue;
                    }
                    bool blocked = false;
                    if (canonicalCombiningClass != 0u)
                    {
                         for (std::size_t codePoint = composed.size(); codePoint-- > 0u;)
                         {
                              const auto kCCC = loc.CombiningClass(composed[codePoint]);
                              if (kCCC == 0u) break;
                              if (kCCC >= canonicalCombiningClass)
                              {
                                   blocked = true;
                                   break;
                              }
                         }
                    }
                    bool inComposed = false;
                    if (!blocked)
                    {
                         for (std::size_t codePoint = composed.size(); codePoint-- > 0u;)
                         {
                              if (loc.CombiningClass(composed[codePoint]) != 0u) continue;
                              const auto candidate = loc.CanonicalCompose(composed[codePoint], codePoint);
                              if (candidate != 0u)
                              {
                                   composed[codePoint] = candidate;
                                   inComposed = true;
                              }
                              break;
                         }
                    }
                    if (!inComposed) composed.push_back(codePoint);
               }

               std::vector<DecodedEntry> entries;
               entries.reserve(composed.size());
               for (const auto codePoint : composed)
                    entries.push_back(DecodedEntry{.run = CodepointWidth(codePoint), .codepoint = codePoint});
               return MakeFromDecoded(entries);
          }

          void Normalise(const Normalisation normalisation = Normalisation::NFC)
          { *this = ToNormalised(normalisation); }

          auto FormatTo(std::format_context& context) const
          {
               const auto utf8 = ToUTF8();
               return std::format_to(
                   context.out(), "{}",
                   std::string_view{reinterpret_cast<const char*>(utf8.data()), utf8.size()}); // NOLINT
          }

          [[nodiscard]] DynamicText Replace(std::invocable<std::uint32_t> auto&& mapper) const
          {
               auto decoded = BuildRawIndex();
               for (auto& entry : decoded)
               {
                    const std::uint32_t mapped = std::invoke(mapper, entry.codepoint);
                    if (mapped != entry.codepoint)
                    {
                         entry.codepoint = mapped;
                         entry.run = CodepointWidth(mapped);
                    }
               }
               return MakeFromDecoded(decoded);
          }

          [[nodiscard]] DynamicText Filter(std::invocable<std::uint32_t> auto&& mapper) const
          {
               std::vector<DecodedEntry> copy{};
               copy.reserve(indexSizeInPoints);
               ForEach(
                   [&](const DecodedEntry& entry)
                   {
                        const bool passed = std::invoke(mapper, entry.codepoint);
                        if (passed) copy.push_back(entry);
                        return true;
                   });
               return MakeFromDecoded(copy);
          }

          [[nodiscard]] DynamicText Filter(std::invocable<std::vector<std::uint32_t>> auto&& mapper) const
          {
               const std::size_t sizeInGraphemes = SizeInGraphemes();
               std::vector<DecodedEntry> copy{};
               copy.reserve(sizeInGraphemes);
               for (std::size_t graphemeIndex = 0; graphemeIndex < sizeInGraphemes; graphemeIndex++)
               {
                    const auto codePoints = GraphemeAtEx(graphemeIndex);
                    if (std::invoke(mapper,
                                    std::span<const std::uint32_t>{codePoints |
                                                                   std::views::transform([](const DecodedEntry& entry)
                                                                                         { return entry.codepoint; }) |
                                                                   std::ranges::to<std::vector>()}))
                         copy.append_range(codePoints);
               }
               return MakeFromDecoded(copy);
          }

          [[nodiscard]] std::optional<std::uint32_t> Find(std::invocable<std::uint32_t> auto&& mapper) const
          {
               std::optional<std::uint32_t> found;
               ForEach(
                   [&](const DecodedEntry& entry)
                   {
                        if (!std::invoke(mapper, entry.codepoint)) return true;
                        found = entry.codepoint;
                        return false;
                   });
               return found;
          }

          [[nodiscard]] std::optional<DynamicText> Find(
              std::invocable<std::span<const std::uint32_t>> auto&& mapper) const
          {
               const std::size_t sizeInGraphemes = SizeInGraphemes();
               for (std::size_t graphemeIndex = 0; graphemeIndex < sizeInGraphemes; graphemeIndex++)
               {
                    const auto codePoints = GraphemeAtEx(graphemeIndex);
                    if (std::invoke(mapper,
                                    std::span<const std::uint32_t>{codePoints |
                                                                   std::views::transform([](const DecodedEntry& entry)
                                                                                         { return entry.codepoint; }) |
                                                                   std::ranges::to<std::vector>()}))
                         return MakeFromDecoded(codePoints);
               }
               return std::nullopt;
          }

          [[nodiscard]] std::optional<std::size_t> IndexOf(std::invocable<std::uint32_t> auto&& mapper) const
          {
               const std::size_t sizeInGraphemes = SizeInGraphemes();
               for (std::size_t graphemeIndex = 0; graphemeIndex < sizeInGraphemes; graphemeIndex++)
               {
                    const auto codePoints = GraphemeAt(graphemeIndex);
                    if (std::any_of(codePoints.begin(), codePoints.end(), [&mapper](const std::uint32_t codePoint)
                                    { return std::invoke(mapper, codePoint); }))
                         return graphemeIndex;
               }
               return std::nullopt;
          }

          [[nodiscard]] std::optional<std::size_t> IndexOf(
              std::invocable<std::span<const std::uint32_t>> auto&& mapper) const
          {
               const std::size_t sizeInGraphemes = SizeInGraphemes();
               for (std::size_t graphemeIndex = 0; graphemeIndex < sizeInGraphemes; graphemeIndex++)
               {
                    const auto codePoints = GraphemeAt(graphemeIndex);
                    if (std::invoke(mapper, std::span<const std::uint32_t>{codePoints})) return graphemeIndex;
               }
               return std::nullopt;
          }

          [[nodiscard]] static bool IsASCII(std::uint32_t codePoint) noexcept { return codePoint <= 0x007Fu; }

          [[nodiscard]] static bool IsASCIIGrapheme(std::span<const std::uint32_t> codePoints) noexcept
          {
               if (codePoints.size() != 1) return false;
               return codePoints[0] <= 0x007Fu;
          }

          [[nodiscard]] bool IsWhitespace(std::uint32_t codePoint) const noexcept
          { return Locale().IsWhitespace(codePoint); }

          [[nodiscard]] static bool IsEmoji(std::span<const std::uint32_t> codePoints) noexcept
          {
               bool hasEmojiBase = false;
               bool onlyEmojiLike = true;
               for (unsigned int codePoint : codePoints)
               {
                    if (codePoint == 0xFE0F) continue;
                    if (codePoint == 0x200D) continue;
                    const auto gcb = unicode::GraphemeClusterBoundaryPropertyOf(codePoint);
                    if (gcb == unicode::GraphemeClusterBoundaryProperty::ExtPic) hasEmojiBase = true;
                    else
                         onlyEmojiLike = false;
               }
               return hasEmojiBase && onlyEmojiLike;
          }

          [[nodiscard]] bool IsASCII(void) const
          {
               return All([](const DecodedEntry& entry) { return entry.codepoint <= 0x007Fu; });
          }

          [[nodiscard]] bool IsPalindrome(void) const { return *this == ToReversed(); }
     };
     template <std::size_t N> inline DynamicText DynamicFromString(const char8_t (&text)[N]) // NOLINT
     {
          std::span<const char8_t> span{text, N - 1};
          return DynamicText{std::as_bytes(span), Encoding::UTF8};
     }

     std::string_view ToString(Comparison comp);
} // namespace text

template <typename TUnicode> struct std::formatter<text::Text<TUnicode>>
{
     constexpr auto parse(std::format_parse_context& context) { return context.begin(); } // NOLINT
     auto format(const text::Text<TUnicode>& text, std::format_context& context) const    // NOLINT
     { return text.FormatTo(context); }
};
template <> struct std::formatter<text::DynamicText>
{
     constexpr auto parse(std::format_parse_context& context) { return context.begin(); } // NOLINT
     auto format(const text::DynamicText& text, std::format_context& context) const       // NOLINT
     { return text.FormatTo(context); }
};
