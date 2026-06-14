#include <uText.h>
#include <iostream>
#include <print>
#include <stdexcept>
#include <string>
#include "unicode.h"

int main(void)
{
     auto english = std::make_shared<text::LocaleAdapter<text::unicode::Unicode<text::unicode::Locale::English>>>();
     auto turkish = std::make_shared<text::LocaleAdapter<text::unicode::Unicode<text::unicode::Locale::Turkish>>>();

     try
     {
          using Text = text::DynamicText;
          using text::DynamicFromString;

          while (true)
          {
               std::string raw{};
               std::print("> ");
               if (!std::getline(std::cin, raw)) break;
               std::span<const char> span{raw};
               Text text = Text{std::as_bytes(span), text::Encoding::UTF8};
               auto spaceIndex = static_cast<decltype(0z)>(text.Find(' '));
               Text command{};
               Text arguments{};
               if (spaceIndex == Text::npos) command = std::move(text);
               else
               {
                    command = text[0, spaceIndex];
                    arguments = text[spaceIndex + 1, 0];
               }

               if (command == DynamicFromString(u8"dump")) { arguments.Dump(); }
               else if (command == DynamicFromString(u8"length"))
               {
                    std::println("length({}) = {}", arguments, arguments.Size());
               }
               else if (command == DynamicFromString(u8"reverse"))
               {
                    std::println("reverse({}) = {}", arguments, arguments.ToReversed());
               }
               else if (command == DynamicFromString(u8"upper"))
               {
                    std::println("upper({}) = {}", arguments, arguments.ToUpper());
               }
               else if (command == DynamicFromString(u8"turkish")) { text::detail::GlobalLocaleRef() = turkish; }
               else if (command == DynamicFromString(u8"english")) { text::detail::GlobalLocaleRef() = english; }
               else if (command == DynamicFromString(u8"lower"))
               {
                    std::println("lower({}) = {}", arguments, arguments.ToLower());
               }
               else if (command == DynamicFromString(u8"normalize") || command == DynamicFromString(u8"normalise"))
               {
                    std::println("NFC:  {}", arguments.ToNormalised(text::Normalisation::NFC));
                    std::println("NFKD: {}", arguments.ToNormalised(text::Normalisation::NFKD));
                    std::println("NFD:  {}", arguments.ToNormalised(text::Normalisation::NFD));
                    std::println("NFKC: {}", arguments.ToNormalised(text::Normalisation::NFKC));
               }
               else if (command == DynamicFromString(u8"contains"))
               {
                    auto delimIdx = static_cast<decltype(0z)>(arguments.Find(' '));
                    if (delimIdx != Text::npos)
                    {
                         const Text haystack = arguments[0, delimIdx];
                         const Text needle = arguments[delimIdx + 1, 0];
                         std::println("contains({}, {}) = {}", haystack, needle,
                                      haystack.Contains(needle) ? "true" : "false");
                    }
                    else
                         std::println("Usage: contains <haystack> <needle>");
               }
               else if (command == DynamicFromString(u8"split"))
               {
                    auto delimIdx = static_cast<decltype(0z)>(arguments.Find(' '));
                    if (delimIdx != Text::npos)
                    {
                         const Text toSplit = arguments[0, delimIdx];
                         const Text delimText = arguments[delimIdx + 1, 0];
                         if (!delimText.IsEmpty())
                         {
                              const auto delim = delimText[0];
                              const auto parts = toSplit.Split(delim);
                              for (std::size_t pi = 0; pi < parts.size(); pi++) std::println("[{}] {}", pi, parts[pi]);
                         }
                    }
                    else
                         std::println("Usage: split <text> <delimiter-char>");
               }
               else if (command == DynamicFromString(u8"compare"))
               {
                    auto delimIdx = static_cast<decltype(0z)>(arguments.Find(' '));
                    if (delimIdx != Text::npos)
                    {
                         const Text lhs = arguments[0, delimIdx];
                         const Text rhs = arguments[delimIdx + 1, 0];
                         std::println("compare({}, {}) = {}", lhs, rhs, ToString(lhs.Compare(rhs)));
                    }
                    else
                         std::println("Usage: compare <lhs> <rhs>");
               }
               else if (command == DynamicFromString(u8"count"))
               {
                    auto delimIdx = static_cast<decltype(0z)>(arguments.Find(' '));
                    if (delimIdx != Text::npos)
                    {
                         const Text haystack = arguments[0, delimIdx];
                         const Text needle = arguments[delimIdx + 1, 0];
                         if (!needle.IsEmpty())
                              std::println("count({}, {}) = {}", haystack, needle, haystack.Count(needle[0]));
                    }
                    else
                         std::println("Usage: count <text> <char>");
               }
               else if (command == DynamicFromString(u8"palindrome"))
               {
                    std::println("{} is{} a palindrome", arguments, arguments.IsPalindrome() ? "" : " not");
               }
               else if (command == DynamicFromString(u8"utf32"))
               {
                    const auto utf32 = arguments.ToUTF32();
                    std::print("UTF-32: ");
                    for (const auto codePoint : utf32) std::print("U+{:04X} ", static_cast<std::uint32_t>(codePoint));
                    std::println();
                    std::println("Length: {} code points/units, {} bytes", utf32.length(), utf32.length() * 4uz);
               }
               else if (command == DynamicFromString(u8"utf16"))
               {
                    const auto utf16 = arguments.ToUTF16();
                    std::print("UTF-16: ");
                    for (const auto codeUnit : utf16) std::print("{:04X} ", static_cast<std::uint32_t>(codeUnit));
                    std::println();
                    std::println("Length: {} code units, {} bytes", utf16.length(), utf16.length() * 2uz);
               }
               else if (command == DynamicFromString(u8"utf8"))
               {
                    const auto utf8 = arguments.ToUTF8();
                    std::print("UTF-8: ");
                    for (const auto codeUnit : utf8) std::print("{:02X} ", static_cast<std::uint32_t>(codeUnit));
                    std::println();
                    std::println("Length: {} code units/bytes", utf8.length());
               }
               else if (command == DynamicFromString(u8"from-utf8"))
               {
                    std::vector<std::byte> utf8{};
                    for (std::size_t i = 0; i < arguments.Size(); i++)
                    {
                         std::uint8_t hex{};
                         auto high = (arguments[i++] | 32U) % 29U % 19U;
                         auto low = (arguments[i++] | 32U) % 29U % 19U;
                         utf8.push_back(std::byte(low | (high << 4)));
                         std::print("{:X}{:X}; ", high, low);
                    }
                    std::println();
                    Text text{std::span{utf8.data(), utf8.size()}, text::Encoding::UTF8};
                    std::println("Result = '{}'", text);
                    // text.Dump();
               }
               else if (command == DynamicFromString(u8"from-utf16"))
               {
                    std::vector<std::byte> utf16{};
                    for (std::size_t i = 0; i < arguments.Size(); i++)
                    {
                         std::uint8_t hex{};
                         auto high = (arguments[i++] | 32U) % 29U % 19U;
                         auto low = (arguments[i++] | 32U) % 29U % 19U;
                         utf16.push_back(std::byte(low | (high << 4)));
                         std::print("{:X}{:X}; ", high, low);
                    }
                    std::println();
                    Text text{std::span{utf16.data(), utf16.size()}, text::Encoding::UTF16};
                    std::println("Result = '{}'", text);
                    // text.Dump();
               }
               else if (command == DynamicFromString(u8"help"))
               {
                    std::println("Commands:");
                    std::println("  dump <text>              - Show internal representation");
                    std::println("  length <text>            - Count grapheme clusters");
                    std::println("  reverse <text>           - Reverse grapheme clusters");
                    std::println("  upper <text>             - To uppercase");
                    std::println("  lower <text>             - To lowercase");
                    std::println("  normalise <text>         - Show NFC/NFD/NFKC/NFKD forms");
                    std::println("  contains <hay> <needle>  - Substring test");
                    std::println("  split <text> <delim>     - Split on delimiter");
                    std::println("  compare <lhs> <rhs>      - Unicode comparison");
                    std::println("  count <text> <char>      - Count occurrences");
                    std::println("  palindrome <text>        - Palindrome check");
                    std::println("  utf32 <text>             - Show UTF-32 code units");
                    std::println("  utf16 <text>             - Show UTF-16 code units");
                    std::println("  utf8 <text>             - Show UTF-9 code units");
               }
               else
               {
                    std::println("Unknown command: {} (type 'help' for a list)", command);
               }
          }
     }
     catch (const std::out_of_range& exception)
     {
          std::println("error: out of range: {}", exception.what());
     }
     catch (const std::runtime_error& exception)
     {
          std::println("error: {}", exception.what());
     }
}
