#include <uText.h>
#include "unicode.h"

std::string_view text::ToString(Comparison comp)
{
     switch (comp)
     {
     case Comparison::OrdinallyEquivalent: return "ordinally-equivalent";
     case Comparison::OrdinallyEquivalentIgnoreCase: return "ordinally-equivalent-insensitive";
     case Comparison::CanonicallyEquivalent: return "canonically-equivalent";
     case Comparison::CompatibilityEquivalent: return "compatibility-equivalent";
     case Comparison::CanonicallyEquivalentIgnoreCase: return "canonically-equivalent-insensitive";
     case Comparison::CompatibilityEquivalentIgnoreCase: return "compatibility-equivalent-insensitive";
     default: return "unequal";
     }
}

using namespace text::unicode;

inline namespace graphemeClusterBoundary
{
     template <GraphemeClusterBoundaryProperty> [[nodiscard]] constexpr bool Is(std::uint32_t codePoint) noexcept;
     template <>
     [[nodiscard]] constexpr bool Is<GraphemeClusterBoundaryProperty::CarriageReturn>(std::uint32_t codePoint) noexcept
     { return codePoint == 0x000Du; }
     template <>
     [[nodiscard]] constexpr bool Is<GraphemeClusterBoundaryProperty::LineFeed>(std::uint32_t codePoint) noexcept
     { return codePoint == 0x000Au; }
     template <>
     [[nodiscard]] constexpr bool Is<GraphemeClusterBoundaryProperty::ZeroWidthJoiner>(std::uint32_t codePoint) noexcept
     { return codePoint == 0x200Du; }
     template <>
     [[nodiscard]] constexpr bool Is<GraphemeClusterBoundaryProperty::Control>(std::uint32_t codePoint) noexcept
     {
          return codePoint <= 0x001Fu || (codePoint >= 0x007Fu && codePoint <= 0x009Fu) || codePoint == 0x00ADu ||
                 codePoint == 0x200Bu || codePoint == 0x2028u || codePoint == 0x2029u;
     }
     template <>
     [[nodiscard]] constexpr bool Is<GraphemeClusterBoundaryProperty::HangulL>(std::uint32_t codePoint) noexcept
     { return (codePoint >= 0x1100u && codePoint <= 0x115Fu) || (codePoint >= 0xA960u && codePoint <= 0xA97Cu); }
     template <>
     [[nodiscard]] constexpr bool Is<GraphemeClusterBoundaryProperty::HangulV>(std::uint32_t codePoint) noexcept
     { return (codePoint >= 0x1160u && codePoint <= 0x11A7u) || (codePoint >= 0xD7B0u && codePoint <= 0xD7C6u); }
     template <>
     [[nodiscard]] constexpr bool Is<GraphemeClusterBoundaryProperty::HangulT>(std::uint32_t codePoint) noexcept
     { return (codePoint >= 0x11A8u && codePoint <= 0x11FFu) || (codePoint >= 0xD7CBu && codePoint <= 0xD7FBu); }
     template <>
     [[nodiscard]] constexpr bool Is<GraphemeClusterBoundaryProperty::RegionalIndicator>(
         std::uint32_t codePoint) noexcept
     { return codePoint >= 0x1F1E0u && codePoint <= 0x1F1FFu; }
     template <>
     [[nodiscard]] constexpr bool Is<GraphemeClusterBoundaryProperty::EmojiModifier>(std::uint32_t codePoint) noexcept
     { return codePoint >= 0x1F3FBu && codePoint <= 0x1F3FFu; }
     template <>
     [[nodiscard]] constexpr bool Is<GraphemeClusterBoundaryProperty::ExtPic>(std::uint32_t codePoint) noexcept
     {
          return ((codePoint >= 0x2600u && codePoint <= 0x27BFu) || (codePoint >= 0x1F300u && codePoint <= 0x1F5FFu) ||
                  (codePoint >= 0x1F600u && codePoint <= 0x1F64Fu) ||
                  (codePoint >= 0x1F680u && codePoint <= 0x1F6FFu) ||
                  (codePoint >= 0x1F900u && codePoint <= 0x1F9FFu) ||
                  (codePoint >= 0x1FA00u && codePoint <= 0x1FAFFu)) ||
                 (codePoint == 0x00A9) || (codePoint == 0x00AE) || (codePoint >= 0x203C && codePoint <= 0x203C) ||
                 (codePoint == 0x2049) || (codePoint == 0x2122) || (codePoint == 0x2139) ||
                 (codePoint >= 0x2194 && codePoint <= 0x2199) || (codePoint >= 0x21A9 && codePoint <= 0x21AA) ||
                 (codePoint >= 0x231A && codePoint <= 0x231B) || (codePoint == 0x2328) || (codePoint == 0x23CF) ||
                 (codePoint >= 0x23E9 && codePoint <= 0x23F3) || (codePoint >= 0x23F8 && codePoint <= 0x23FA) ||
                 (codePoint == 0x24C2) || (codePoint >= 0x25AA && codePoint <= 0x25AB) || (codePoint == 0x25B6) ||
                 (codePoint == 0x25C0) || (codePoint >= 0x25FB && codePoint <= 0x25FE) ||
                 (codePoint >= 0x2600 && codePoint <= 0x2604) || (codePoint == 0x260E) || (codePoint == 0x2611) ||
                 (codePoint >= 0x2614 && codePoint <= 0x2615) || (codePoint == 0x2618) || (codePoint == 0x261D) ||
                 (codePoint == 0x2620) || (codePoint >= 0x2622 && codePoint <= 0x2623) || (codePoint == 0x2626) ||
                 (codePoint == 0x262A) || (codePoint >= 0x262E && codePoint <= 0x262F) ||
                 (codePoint >= 0x2638 && codePoint <= 0x263A) || (codePoint == 0x2640) || (codePoint == 0x2642) ||
                 (codePoint >= 0x2648 && codePoint <= 0x2653) || (codePoint >= 0x265F && codePoint <= 0x2660) ||
                 (codePoint == 0x2663) || (codePoint >= 0x2665 && codePoint <= 0x2666) || (codePoint == 0x2668) ||
                 (codePoint == 0x267B) || (codePoint >= 0x267E && codePoint <= 0x267F) ||
                 (codePoint >= 0x2692 && codePoint <= 0x2697) || (codePoint == 0x2699) ||
                 (codePoint >= 0x269B && codePoint <= 0x269C) || (codePoint >= 0x26A0 && codePoint <= 0x26A1) ||
                 (codePoint == 0x26A7) || (codePoint >= 0x26AA && codePoint <= 0x26AB) ||
                 (codePoint >= 0x26B0 && codePoint <= 0x26B1) || (codePoint >= 0x26BD && codePoint <= 0x26BE) ||
                 (codePoint >= 0x26C4 && codePoint <= 0x26C5) || (codePoint >= 0x26CE && codePoint <= 0x26CF) ||
                 (codePoint == 0x26D1) || (codePoint >= 0x26D3 && codePoint <= 0x26D4) ||
                 (codePoint >= 0x26E9 && codePoint <= 0x26EA) || (codePoint >= 0x26F0 && codePoint <= 0x26F5) ||
                 (codePoint >= 0x26F7 && codePoint <= 0x26FA) || (codePoint == 0x26FD) || (codePoint == 0x2702) ||
                 (codePoint == 0x2705) || (codePoint >= 0x2708 && codePoint <= 0x270D) || (codePoint == 0x270F) ||
                 (codePoint == 0x2712) || (codePoint == 0x2714) || (codePoint == 0x2716) || (codePoint == 0x271D) ||
                 (codePoint == 0x2721) || (codePoint == 0x2728) || (codePoint >= 0x2733 && codePoint <= 0x2734) ||
                 (codePoint == 0x2744) || (codePoint == 0x2747) || (codePoint == 0x274C) || (codePoint == 0x274E) ||
                 (codePoint >= 0x2753 && codePoint <= 0x2755) || (codePoint == 0x2757) ||
                 (codePoint >= 0x2763 && codePoint <= 0x2764) || (codePoint >= 0x2795 && codePoint <= 0x2797) ||
                 (codePoint == 0x27A1) || (codePoint == 0x27B0) || (codePoint == 0x27BF) ||
                 (codePoint >= 0x2934 && codePoint <= 0x2935) || (codePoint >= 0x2B05 && codePoint <= 0x2B07) ||
                 (codePoint >= 0x2B1B && codePoint <= 0x2B1C) || (codePoint == 0x2B50) || (codePoint == 0x2B55) ||
                 (codePoint == 0x3030) || (codePoint == 0x303D) || (codePoint == 0x3297) || (codePoint == 0x3299) ||
                 (codePoint == 0x1F004) || (codePoint == 0x1F0CF) || (codePoint >= 0x1F170 && codePoint <= 0x1F171) ||
                 (codePoint >= 0x1F17E && codePoint <= 0x1F17F) || (codePoint == 0x1F18E) ||
                 (codePoint >= 0x1F191 && codePoint <= 0x1F19A) || (codePoint >= 0x1F201 && codePoint <= 0x1F202) ||
                 (codePoint == 0x1F21A) || (codePoint == 0x1F22F) || (codePoint >= 0x1F232 && codePoint <= 0x1F23A) ||
                 (codePoint >= 0x1F250 && codePoint <= 0x1F251) || (codePoint >= 0x1F300 && codePoint <= 0x1F5FF) ||
                 (codePoint >= 0x1F600 && codePoint <= 0x1F64F) || (codePoint >= 0x1F680 && codePoint <= 0x1F6FF) ||
                 (codePoint >= 0x1F7E0 && codePoint <= 0x1F7EB) || (codePoint == 0x1F7F0) ||
                 (codePoint >= 0x1F90C && codePoint <= 0x1F9FF) || (codePoint >= 0x1FA00 && codePoint <= 0x1FA53) ||
                 (codePoint >= 0x1FA60 && codePoint <= 0x1FA6D) || (codePoint >= 0x1FA70 && codePoint <= 0x1FA7C) ||
                 (codePoint >= 0x1FA80 && codePoint <= 0x1FA88) || (codePoint >= 0x1FA90 && codePoint <= 0x1FABD) ||
                 (codePoint >= 0x1FABF && codePoint <= 0x1FAC5) || (codePoint >= 0x1FACE && codePoint <= 0x1FADB) ||
                 (codePoint >= 0x1FAE0 && codePoint <= 0x1FAE8) || (codePoint >= 0x1FAF0 && codePoint <= 0x1FAF8);
     }
     template <>
     [[nodiscard]] constexpr bool Is<GraphemeClusterBoundaryProperty::Extend>(std::uint32_t codePoint) noexcept
     {
          return (codePoint >= 0x0300 && codePoint <= 0x036F) || (codePoint >= 0x0483 && codePoint <= 0x0489) ||
                 (codePoint >= 0x0591 && codePoint <= 0x05BD) || (codePoint == 0x05BF) ||
                 (codePoint >= 0x05C1 && codePoint <= 0x05C2) || (codePoint >= 0x05C4 && codePoint <= 0x05C5) ||
                 (codePoint == 0x05C7) || (codePoint >= 0x0610 && codePoint <= 0x061A) ||
                 (codePoint >= 0x064B && codePoint <= 0x065F) || (codePoint == 0x0670) ||
                 (codePoint >= 0x06D6 && codePoint <= 0x06DC) || (codePoint >= 0x06DF && codePoint <= 0x06E4) ||
                 (codePoint >= 0x06E7 && codePoint <= 0x06E8) || (codePoint >= 0x06EA && codePoint <= 0x06ED) ||
                 (codePoint == 0x0711) || (codePoint >= 0x0730 && codePoint <= 0x074A) ||
                 (codePoint >= 0x07A6 && codePoint <= 0x07B0) || (codePoint >= 0x07EB && codePoint <= 0x07F3) ||
                 (codePoint == 0x07FD) || (codePoint >= 0x0816 && codePoint <= 0x082D) ||
                 (codePoint >= 0x0859 && codePoint <= 0x085B) || (codePoint >= 0x0898 && codePoint <= 0x089F) ||
                 (codePoint >= 0x08CA && codePoint <= 0x08E1) || (codePoint >= 0x08E3 && codePoint <= 0x0902) ||
                 (codePoint == 0x093A) || (codePoint == 0x093C) || (codePoint >= 0x0941 && codePoint <= 0x0948) ||
                 (codePoint == 0x094D) || (codePoint >= 0x0951 && codePoint <= 0x0957) ||
                 (codePoint >= 0x0962 && codePoint <= 0x0963) || (codePoint == 0x0981) || (codePoint == 0x09BC) ||
                 (codePoint == 0x09BE) || (codePoint >= 0x09C1 && codePoint <= 0x09C4) || (codePoint == 0x09CD) ||
                 (codePoint == 0x09D7) || (codePoint >= 0x09E2 && codePoint <= 0x09E3) || (codePoint == 0x09FE) ||
                 (codePoint >= 0x0A01 && codePoint <= 0x0A02) || (codePoint == 0x0A3C) ||
                 (codePoint >= 0x0A41 && codePoint <= 0x0A42) || (codePoint >= 0x0A47 && codePoint <= 0x0A48) ||
                 (codePoint >= 0x0A4B && codePoint <= 0x0A4D) || (codePoint == 0x0A51) ||
                 (codePoint >= 0x0A70 && codePoint <= 0x0A71) || (codePoint == 0x0A75) ||
                 (codePoint >= 0x0A81 && codePoint <= 0x0A82) || (codePoint == 0x0ABC) ||
                 (codePoint >= 0x0AC1 && codePoint <= 0x0AC5) || (codePoint >= 0x0AC7 && codePoint <= 0x0AC8) ||
                 (codePoint == 0x0ACD) || (codePoint >= 0x0AE2 && codePoint <= 0x0AE3) ||
                 (codePoint >= 0x0AFA && codePoint <= 0x0AFF) || (codePoint == 0x0B01) || (codePoint == 0x0B3C) ||
                 (codePoint == 0x0B3F) || (codePoint >= 0x0B41 && codePoint <= 0x0B44) || (codePoint == 0x0B4D) ||
                 (codePoint >= 0x0B55 && codePoint <= 0x0B56) || (codePoint >= 0x0B62 && codePoint <= 0x0B63) ||
                 (codePoint == 0x0B82) || (codePoint == 0x0BC0) || (codePoint == 0x0BCD) || (codePoint == 0x0C00) ||
                 (codePoint == 0x0C04) || (codePoint == 0x0C3C) || (codePoint >= 0x0C3E && codePoint <= 0x0C40) ||
                 (codePoint >= 0x0C46 && codePoint <= 0x0C48) || (codePoint >= 0x0C4A && codePoint <= 0x0C4D) ||
                 (codePoint >= 0x0C55 && codePoint <= 0x0C56) || (codePoint >= 0x0C62 && codePoint <= 0x0C63) ||
                 (codePoint == 0x0C81) || (codePoint == 0x0CBC) || (codePoint == 0x0CBF) || (codePoint == 0x0CC6) ||
                 (codePoint >= 0x0CCC && codePoint <= 0x0CCD) || (codePoint >= 0x0CE2 && codePoint <= 0x0CE3) ||
                 (codePoint >= 0x0D00 && codePoint <= 0x0D01) || (codePoint >= 0x0D3B && codePoint <= 0x0D3C) ||
                 (codePoint >= 0x0D41 && codePoint <= 0x0D44) || (codePoint == 0x0D4D) ||
                 (codePoint >= 0x0D62 && codePoint <= 0x0D63) || (codePoint == 0x0D81) || (codePoint == 0x0DCA) ||
                 (codePoint >= 0x0DD2 && codePoint <= 0x0DD4) || (codePoint == 0x0DD6) || (codePoint == 0x0E31) ||
                 (codePoint >= 0x0E34 && codePoint <= 0x0E3A) || (codePoint >= 0x0E47 && codePoint <= 0x0E4E) ||
                 (codePoint == 0x0EB1) || (codePoint >= 0x0EB4 && codePoint <= 0x0EBC) ||
                 (codePoint >= 0x0EC8 && codePoint <= 0x0ECD) || (codePoint >= 0x0F18 && codePoint <= 0x0F19) ||
                 (codePoint == 0x0F35) || (codePoint == 0x0F37) || (codePoint == 0x0F39) ||
                 (codePoint >= 0x0F71 && codePoint <= 0x0F7E) || (codePoint >= 0x0F80 && codePoint <= 0x0F84) ||
                 (codePoint >= 0x0F86 && codePoint <= 0x0F87) || (codePoint >= 0x0F8D && codePoint <= 0x0F97) ||
                 (codePoint >= 0x0F99 && codePoint <= 0x0FBC) || (codePoint == 0x0FC6) ||
                 (codePoint >= 0x102D && codePoint <= 0x1030) || (codePoint >= 0x1032 && codePoint <= 0x1037) ||
                 (codePoint >= 0x1039 && codePoint <= 0x103A) || (codePoint >= 0x103D && codePoint <= 0x103E) ||
                 (codePoint >= 0x1058 && codePoint <= 0x1059) || (codePoint >= 0x105E && codePoint <= 0x1060) ||
                 (codePoint >= 0x1071 && codePoint <= 0x1074) || (codePoint == 0x1082) ||
                 (codePoint >= 0x1085 && codePoint <= 0x1086) || (codePoint == 0x108D) || (codePoint == 0x109D) ||
                 (codePoint >= 0x135D && codePoint <= 0x135F) || (codePoint >= 0x1712 && codePoint <= 0x1714) ||
                 (codePoint >= 0x1732 && codePoint <= 0x1733) || (codePoint >= 0x1752 && codePoint <= 0x1753) ||
                 (codePoint >= 0x1772 && codePoint <= 0x1773) || (codePoint >= 0x17B4 && codePoint <= 0x17B5) ||
                 (codePoint >= 0x17B7 && codePoint <= 0x17BD) || (codePoint == 0x17C6) ||
                 (codePoint >= 0x17C9 && codePoint <= 0x17D3) || (codePoint == 0x17DD) ||
                 (codePoint >= 0x180B && codePoint <= 0x180D) || (codePoint == 0x180F) ||
                 (codePoint >= 0x1885 && codePoint <= 0x1886) || (codePoint == 0x18A9) ||
                 (codePoint >= 0x1920 && codePoint <= 0x1922) || (codePoint >= 0x1927 && codePoint <= 0x1928) ||
                 (codePoint == 0x1932) || (codePoint >= 0x1939 && codePoint <= 0x193B) ||
                 (codePoint >= 0x1A17 && codePoint <= 0x1A18) || (codePoint == 0x1A1B) || (codePoint == 0x1A56) ||
                 (codePoint >= 0x1A58 && codePoint <= 0x1A5E) || (codePoint == 0x1A60) || (codePoint == 0x1A62) ||
                 (codePoint >= 0x1A65 && codePoint <= 0x1A6C) || (codePoint >= 0x1A73 && codePoint <= 0x1A7C) ||
                 (codePoint == 0x1A7F) || (codePoint >= 0x1AB0 && codePoint <= 0x1ACE) ||
                 (codePoint >= 0x1B00 && codePoint <= 0x1B03) || (codePoint == 0x1B34) ||
                 (codePoint >= 0x1B36 && codePoint <= 0x1B3A) || (codePoint == 0x1B3C) || (codePoint == 0x1B42) ||
                 (codePoint >= 0x1B6B && codePoint <= 0x1B73) || (codePoint >= 0x1B80 && codePoint <= 0x1B81) ||
                 (codePoint >= 0x1BA2 && codePoint <= 0x1BA5) || (codePoint >= 0x1BA8 && codePoint <= 0x1BA9) ||
                 (codePoint >= 0x1BAB && codePoint <= 0x1BAD) || (codePoint == 0x1BE6) ||
                 (codePoint >= 0x1BE8 && codePoint <= 0x1BE9) || (codePoint == 0x1BED) ||
                 (codePoint >= 0x1BEF && codePoint <= 0x1BF1) || (codePoint >= 0x1C2C && codePoint <= 0x1C33) ||
                 (codePoint >= 0x1C36 && codePoint <= 0x1C37) || (codePoint >= 0x1CD0 && codePoint <= 0x1CD2) ||
                 (codePoint >= 0x1CD4 && codePoint <= 0x1CE0) || (codePoint >= 0x1CE2 && codePoint <= 0x1CE8) ||
                 (codePoint == 0x1CED) || (codePoint == 0x1CF4) || (codePoint >= 0x1CF8 && codePoint <= 0x1CF9) ||
                 (codePoint >= 0x1DC0 && codePoint <= 0x1DFF) || (codePoint == 0x200C) ||
                 (codePoint >= 0x20D0 && codePoint <= 0x20F0) || (codePoint >= 0xFE00 && codePoint <= 0xFE0Fu) ||
                 (codePoint >= 0xFE20 && codePoint <= 0xFE2F) || (codePoint >= 0xE0000u && codePoint <= 0xE00FFu) ||
                 (codePoint >= 0xE0100u && codePoint <= 0xE01EFu);
     }
     template <>
     [[nodiscard]] constexpr bool Is<GraphemeClusterBoundaryProperty::SpacingMark>(std::uint32_t codePoint) noexcept
     {
          return (codePoint == 0x0903) || (codePoint == 0x093B) || (codePoint >= 0x093E && codePoint <= 0x0940) ||
                 (codePoint >= 0x0949 && codePoint <= 0x094C) || (codePoint >= 0x094E && codePoint <= 0x094F) ||
                 (codePoint >= 0x0982 && codePoint <= 0x0983) || (codePoint >= 0x09BF && codePoint <= 0x09C0) ||
                 (codePoint >= 0x09C7 && codePoint <= 0x09C8) || (codePoint >= 0x09CB && codePoint <= 0x09CC) ||
                 (codePoint == 0x0A03) || (codePoint >= 0x0A3E && codePoint <= 0x0A40) || (codePoint == 0x0A83) ||
                 (codePoint >= 0x0ABE && codePoint <= 0x0AC0) || (codePoint == 0x0AC9) ||
                 (codePoint >= 0x0ACB && codePoint <= 0x0ACC) || (codePoint >= 0x0B02 && codePoint <= 0x0B03) ||
                 (codePoint == 0x0B3E) || (codePoint == 0x0B40) || (codePoint >= 0x0B47 && codePoint <= 0x0B48) ||
                 (codePoint >= 0x0B4B && codePoint <= 0x0B4C) || (codePoint == 0x0B57) ||
                 (codePoint >= 0x0BBE && codePoint <= 0x0BBF) || (codePoint >= 0x0BC1 && codePoint <= 0x0BC2) ||
                 (codePoint >= 0x0BC6 && codePoint <= 0x0BC8) || (codePoint >= 0x0BCA && codePoint <= 0x0BCC) ||
                 (codePoint == 0x0BD7) || (codePoint >= 0x0C01 && codePoint <= 0x0C03) ||
                 (codePoint >= 0x0C41 && codePoint <= 0x0C44) || (codePoint >= 0x0C82 && codePoint <= 0x0C83) ||
                 (codePoint == 0x0CBE) || (codePoint >= 0x0CC0 && codePoint <= 0x0CC4) ||
                 (codePoint >= 0x0CC7 && codePoint <= 0x0CC8) || (codePoint >= 0x0CCA && codePoint <= 0x0CCB) ||
                 (codePoint >= 0x0CD5 && codePoint <= 0x0CD6) || (codePoint >= 0x0D02 && codePoint <= 0x0D03) ||
                 (codePoint >= 0x0D3E && codePoint <= 0x0D40) || (codePoint >= 0x0D46 && codePoint <= 0x0D48) ||
                 (codePoint >= 0x0D4A && codePoint <= 0x0D4C) || (codePoint == 0x0D57) ||
                 (codePoint >= 0x0D82 && codePoint <= 0x0D83) || (codePoint >= 0x0DCF && codePoint <= 0x0DD1) ||
                 (codePoint >= 0x0DD8 && codePoint <= 0x0DDF) || (codePoint >= 0x0DF2 && codePoint <= 0x0DF3) ||
                 (codePoint >= 0x0F3E && codePoint <= 0x0F3F) || (codePoint == 0x0F7F) ||
                 (codePoint >= 0x102B && codePoint <= 0x102C) || (codePoint == 0x1031) || (codePoint == 0x1038) ||
                 (codePoint >= 0x103B && codePoint <= 0x103C) || (codePoint >= 0x1056 && codePoint <= 0x1057) ||
                 (codePoint == 0x1062) || (codePoint >= 0x1067 && codePoint <= 0x1068) ||
                 (codePoint >= 0x1083 && codePoint <= 0x1084) || (codePoint >= 0x1087 && codePoint <= 0x108C) ||
                 (codePoint == 0x108F) || (codePoint >= 0x109A && codePoint <= 0x109C) || (codePoint == 0x1A57) ||
                 (codePoint == 0x1A61) || (codePoint >= 0x1A63 && codePoint <= 0x1A64) || (codePoint == 0x1B04) ||
                 (codePoint == 0x1B35) || (codePoint == 0x1B3B) || (codePoint >= 0x1B3D && codePoint <= 0x1B41) ||
                 (codePoint >= 0x1B43 && codePoint <= 0x1B44) || (codePoint == 0x1B82) || (codePoint == 0x1BA1) ||
                 (codePoint >= 0x1BA6 && codePoint <= 0x1BA7) || (codePoint == 0x1BAA) || (codePoint == 0x1BE7) ||
                 (codePoint >= 0x1BEA && codePoint <= 0x1BEC) || (codePoint == 0x1BEE) ||
                 (codePoint >= 0x1BF2 && codePoint <= 0x1BF3) || (codePoint >= 0x1C24 && codePoint <= 0x1C2B) ||
                 (codePoint >= 0x1C34 && codePoint <= 0x1C35) || (codePoint == 0x1CE1) || (codePoint == 0x1CF7) ||
                 (codePoint >= 0x302E && codePoint <= 0x302F) || (codePoint >= 0xA823 && codePoint <= 0xA824) ||
                 (codePoint == 0xA827) || (codePoint >= 0xA880 && codePoint <= 0xA881) ||
                 (codePoint >= 0xA8B4 && codePoint <= 0xA8C3) || (codePoint >= 0xA952 && codePoint <= 0xA953) ||
                 (codePoint == 0xA983) || (codePoint >= 0xA9B4 && codePoint <= 0xA9B5) ||
                 (codePoint >= 0xA9BA && codePoint <= 0xA9BB) || (codePoint >= 0xA9BD && codePoint <= 0xA9C0) ||
                 (codePoint >= 0xAA2F && codePoint <= 0xAA30) || (codePoint >= 0xAA33 && codePoint <= 0xAA34) ||
                 (codePoint == 0xAA4D) || (codePoint == 0xAA7B) || (codePoint == 0xAA7D) || (codePoint == 0xAAEB) ||
                 (codePoint >= 0xAAEE && codePoint <= 0xAAEF) || (codePoint == 0xAAF5) ||
                 (codePoint >= 0xABE3 && codePoint <= 0xABE4) || (codePoint >= 0xABE6 && codePoint <= 0xABE7) ||
                 (codePoint >= 0xABE9 && codePoint <= 0xABEA) || (codePoint == 0xABEC);
     }
     template <>
     [[nodiscard]] constexpr bool Is<GraphemeClusterBoundaryProperty::Prepend>(std::uint32_t codePoint) noexcept
     {
          return codePoint == 0x0600 || codePoint == 0x0601 || codePoint == 0x0602 || codePoint == 0x0603 ||
                 codePoint == 0x0604 || codePoint == 0x0605 || codePoint == 0x06DD || codePoint == 0x070F ||
                 codePoint == 0x0890 || codePoint == 0x0891 || codePoint == 0x08E2 || codePoint == 0x110BD ||
                 codePoint == 0x110CD || (codePoint >= 0x111C2 && codePoint <= 0x111C3) || codePoint == 0x11A3A ||
                 (codePoint >= 0x11A86 && codePoint <= 0x11A89) || codePoint == 0x11D46;
     }
} // namespace graphemeClusterBoundary

GraphemeClusterBoundaryProperty UnicodeDatabase::GraphemeProperty(std::uint32_t codePoint) noexcept
{
     if (Is<GraphemeClusterBoundaryProperty::CarriageReturn>(codePoint))
          return GraphemeClusterBoundaryProperty::CarriageReturn;
     if (Is<GraphemeClusterBoundaryProperty::LineFeed>(codePoint)) return GraphemeClusterBoundaryProperty::LineFeed;
     if (Is<GraphemeClusterBoundaryProperty::Control>(codePoint)) return GraphemeClusterBoundaryProperty::Control;
     if (Is<GraphemeClusterBoundaryProperty::ZeroWidthJoiner>(codePoint))
          return GraphemeClusterBoundaryProperty::ZeroWidthJoiner;
     if (Is<GraphemeClusterBoundaryProperty::HangulL>(codePoint)) return GraphemeClusterBoundaryProperty::HangulL;
     if (Is<GraphemeClusterBoundaryProperty::HangulV>(codePoint)) return GraphemeClusterBoundaryProperty::HangulV;
     if (Is<GraphemeClusterBoundaryProperty::HangulT>(codePoint)) return GraphemeClusterBoundaryProperty::HangulT;

     if (codePoint >= 0xAC00u && codePoint <= 0xD7A3u)
     {
          const std::uint32_t offset = codePoint - 0xAC00;
          return (offset % 28 == 0) ? GraphemeClusterBoundaryProperty::HangulLV
                                    : GraphemeClusterBoundaryProperty::HangulLVT;
     }

     if (Is<GraphemeClusterBoundaryProperty::RegionalIndicator>(codePoint))
          return GraphemeClusterBoundaryProperty::RegionalIndicator;
     if (Is<GraphemeClusterBoundaryProperty::EmojiModifier>(codePoint))
          return GraphemeClusterBoundaryProperty::EmojiModifier;
     if (Is<GraphemeClusterBoundaryProperty::ExtPic>(codePoint)) return GraphemeClusterBoundaryProperty::ExtPic;
     if (Is<GraphemeClusterBoundaryProperty::Extend>(codePoint)) return GraphemeClusterBoundaryProperty::Extend;
     if (Is<GraphemeClusterBoundaryProperty::SpacingMark>(codePoint))
          return GraphemeClusterBoundaryProperty::SpacingMark;
     if (Is<GraphemeClusterBoundaryProperty::Prepend>(codePoint)) return GraphemeClusterBoundaryProperty::Prepend;
     return GraphemeClusterBoundaryProperty::Other;
}

std::uint8_t UnicodeDatabase::CombiningClass(std::uint32_t codePoint) noexcept
{
     struct CanonicalCombiningClassRange
     {
          std::uint32_t low{};
          std::uint32_t high{};
          std::uint8_t canonicalCombiningClass;
     };
     static constexpr std::array canonicalCombiningClassRanges = std::to_array<CanonicalCombiningClassRange>({
         {.low = 0x0300, .high = 0x0314, .canonicalCombiningClass = 230},
         {.low = 0x0315, .high = 0x0315, .canonicalCombiningClass = 232},
         {.low = 0x0316, .high = 0x0319, .canonicalCombiningClass = 220},
         {.low = 0x031A, .high = 0x031A, .canonicalCombiningClass = 232},
         {.low = 0x031B, .high = 0x031B, .canonicalCombiningClass = 216},
         {.low = 0x031C, .high = 0x0320, .canonicalCombiningClass = 220},
         {.low = 0x0321, .high = 0x0322, .canonicalCombiningClass = 202},
         {.low = 0x0323, .high = 0x0326, .canonicalCombiningClass = 220},
         {.low = 0x0327, .high = 0x0328, .canonicalCombiningClass = 202},
         {.low = 0x0329, .high = 0x0333, .canonicalCombiningClass = 220},
         {.low = 0x0334, .high = 0x0338, .canonicalCombiningClass = 1},
         {.low = 0x0339, .high = 0x033C, .canonicalCombiningClass = 220},
         {.low = 0x033D, .high = 0x0344, .canonicalCombiningClass = 230},
         {.low = 0x0345, .high = 0x0345, .canonicalCombiningClass = 240},
         {.low = 0x0346, .high = 0x0346, .canonicalCombiningClass = 230},
         {.low = 0x0347, .high = 0x0349, .canonicalCombiningClass = 220},
         {.low = 0x034A, .high = 0x034C, .canonicalCombiningClass = 230},
         {.low = 0x034D, .high = 0x034E, .canonicalCombiningClass = 220},
         {.low = 0x0350, .high = 0x0352, .canonicalCombiningClass = 230},
         {.low = 0x0353, .high = 0x0356, .canonicalCombiningClass = 220},
         {.low = 0x0357, .high = 0x0357, .canonicalCombiningClass = 230},
         {.low = 0x0358, .high = 0x0358, .canonicalCombiningClass = 232},
         {.low = 0x0359, .high = 0x035A, .canonicalCombiningClass = 220},
         {.low = 0x035B, .high = 0x035B, .canonicalCombiningClass = 230},
         {.low = 0x035C, .high = 0x035C, .canonicalCombiningClass = 233},
         {.low = 0x035D, .high = 0x035E, .canonicalCombiningClass = 234},
         {.low = 0x035F, .high = 0x035F, .canonicalCombiningClass = 233},
         {.low = 0x0360, .high = 0x0361, .canonicalCombiningClass = 234},
         {.low = 0x0362, .high = 0x0362, .canonicalCombiningClass = 233},
         {.low = 0x0483, .high = 0x0487, .canonicalCombiningClass = 230},
         {.low = 0x0591, .high = 0x0595, .canonicalCombiningClass = 220},
         {.low = 0x0596, .high = 0x0596, .canonicalCombiningClass = 230},
         {.low = 0x0597, .high = 0x0599, .canonicalCombiningClass = 220},
         {.low = 0x059A, .high = 0x059A, .canonicalCombiningClass = 222},
         {.low = 0x059B, .high = 0x059B, .canonicalCombiningClass = 220},
         {.low = 0x059C, .high = 0x05A1, .canonicalCombiningClass = 230},
         {.low = 0x05A2, .high = 0x05A7, .canonicalCombiningClass = 220},
         {.low = 0x05A8, .high = 0x05A9, .canonicalCombiningClass = 230},
         {.low = 0x05AA, .high = 0x05AA, .canonicalCombiningClass = 220},
         {.low = 0x05AB, .high = 0x05AC, .canonicalCombiningClass = 230},
         {.low = 0x05AD, .high = 0x05AD, .canonicalCombiningClass = 222},
         {.low = 0x05AE, .high = 0x05AE, .canonicalCombiningClass = 228},
         {.low = 0x05AF, .high = 0x05AF, .canonicalCombiningClass = 230},
         {.low = 0x05B0, .high = 0x05B0, .canonicalCombiningClass = 10},
         {.low = 0x05B1, .high = 0x05B1, .canonicalCombiningClass = 11},
         {.low = 0x05B2, .high = 0x05B2, .canonicalCombiningClass = 12},
         {.low = 0x05B3, .high = 0x05B3, .canonicalCombiningClass = 13},
         {.low = 0x05B4, .high = 0x05B4, .canonicalCombiningClass = 14},
         {.low = 0x05B5, .high = 0x05B5, .canonicalCombiningClass = 15},
         {.low = 0x05B6, .high = 0x05B6, .canonicalCombiningClass = 16},
         {.low = 0x05B7, .high = 0x05B7, .canonicalCombiningClass = 17},
         {.low = 0x05B8, .high = 0x05B8, .canonicalCombiningClass = 18},
         {.low = 0x05B9, .high = 0x05BA, .canonicalCombiningClass = 19},
         {.low = 0x05BB, .high = 0x05BB, .canonicalCombiningClass = 20},
         {.low = 0x05BC, .high = 0x05BC, .canonicalCombiningClass = 21},
         {.low = 0x05BD, .high = 0x05BD, .canonicalCombiningClass = 22},
         {.low = 0x05BF, .high = 0x05BF, .canonicalCombiningClass = 23},
         {.low = 0x05C1, .high = 0x05C1, .canonicalCombiningClass = 24},
         {.low = 0x05C2, .high = 0x05C2, .canonicalCombiningClass = 25},
         {.low = 0x05C4, .high = 0x05C4, .canonicalCombiningClass = 230},
         {.low = 0x05C5, .high = 0x05C5, .canonicalCombiningClass = 220},
         {.low = 0x05C7, .high = 0x05C7, .canonicalCombiningClass = 18},
         {.low = 0x0610, .high = 0x0617, .canonicalCombiningClass = 230},
         {.low = 0x0618, .high = 0x0618, .canonicalCombiningClass = 30},
         {.low = 0x0619, .high = 0x0619, .canonicalCombiningClass = 31},
         {.low = 0x061A, .high = 0x061A, .canonicalCombiningClass = 32},
         {.low = 0x064B, .high = 0x064B, .canonicalCombiningClass = 27},
         {.low = 0x064C, .high = 0x064C, .canonicalCombiningClass = 28},
         {.low = 0x064D, .high = 0x064D, .canonicalCombiningClass = 29},
         {.low = 0x064E, .high = 0x064E, .canonicalCombiningClass = 30},
         {.low = 0x064F, .high = 0x064F, .canonicalCombiningClass = 31},
         {.low = 0x0650, .high = 0x0650, .canonicalCombiningClass = 32},
         {.low = 0x0651, .high = 0x0651, .canonicalCombiningClass = 33},
         {.low = 0x0652, .high = 0x0652, .canonicalCombiningClass = 34},
         {.low = 0x0653, .high = 0x0654, .canonicalCombiningClass = 230},
         {.low = 0x0655, .high = 0x0656, .canonicalCombiningClass = 220},
         {.low = 0x0657, .high = 0x065B, .canonicalCombiningClass = 230},
         {.low = 0x065C, .high = 0x065C, .canonicalCombiningClass = 220},
         {.low = 0x065D, .high = 0x065E, .canonicalCombiningClass = 230},
         {.low = 0x065F, .high = 0x065F, .canonicalCombiningClass = 220},
         {.low = 0x0670, .high = 0x0670, .canonicalCombiningClass = 35},
         {.low = 0x06D6, .high = 0x06DC, .canonicalCombiningClass = 230},
         {.low = 0x06DF, .high = 0x06E2, .canonicalCombiningClass = 230},
         {.low = 0x06E3, .high = 0x06E3, .canonicalCombiningClass = 220},
         {.low = 0x06E4, .high = 0x06E4, .canonicalCombiningClass = 230},
         {.low = 0x06E7, .high = 0x06E8, .canonicalCombiningClass = 230},
         {.low = 0x06EA, .high = 0x06EB, .canonicalCombiningClass = 220},
         {.low = 0x06EC, .high = 0x06ED, .canonicalCombiningClass = 230},
         {.low = 0x0711, .high = 0x0711, .canonicalCombiningClass = 36},
         {.low = 0x0730, .high = 0x0730, .canonicalCombiningClass = 230},
         {.low = 0x0731, .high = 0x0731, .canonicalCombiningClass = 220},
         {.low = 0x0732, .high = 0x0733, .canonicalCombiningClass = 230},
         {.low = 0x0734, .high = 0x0734, .canonicalCombiningClass = 220},
         {.low = 0x0735, .high = 0x0736, .canonicalCombiningClass = 230},
         {.low = 0x0737, .high = 0x0739, .canonicalCombiningClass = 220},
         {.low = 0x073A, .high = 0x073A, .canonicalCombiningClass = 230},
         {.low = 0x073B, .high = 0x073C, .canonicalCombiningClass = 220},
         {.low = 0x073D, .high = 0x073D, .canonicalCombiningClass = 230},
         {.low = 0x073E, .high = 0x073E, .canonicalCombiningClass = 220},
         {.low = 0x073F, .high = 0x0741, .canonicalCombiningClass = 230},
         {.low = 0x0742, .high = 0x0742, .canonicalCombiningClass = 220},
         {.low = 0x0743, .high = 0x0743, .canonicalCombiningClass = 230},
         {.low = 0x0744, .high = 0x0744, .canonicalCombiningClass = 220},
         {.low = 0x0745, .high = 0x0745, .canonicalCombiningClass = 230},
         {.low = 0x0746, .high = 0x0746, .canonicalCombiningClass = 220},
         {.low = 0x0747, .high = 0x0747, .canonicalCombiningClass = 230},
         {.low = 0x0748, .high = 0x0748, .canonicalCombiningClass = 220},
         {.low = 0x0749, .high = 0x074A, .canonicalCombiningClass = 230},
         {.low = 0x07EB, .high = 0x07F1, .canonicalCombiningClass = 230},
         {.low = 0x07F2, .high = 0x07F2, .canonicalCombiningClass = 220},
         {.low = 0x07F3, .high = 0x07F3, .canonicalCombiningClass = 230},
         {.low = 0x07FD, .high = 0x07FD, .canonicalCombiningClass = 220},
         {.low = 0x0816, .high = 0x0819, .canonicalCombiningClass = 230},
         {.low = 0x081B, .high = 0x0823, .canonicalCombiningClass = 230},
         {.low = 0x0825, .high = 0x0827, .canonicalCombiningClass = 230},
         {.low = 0x0829, .high = 0x082D, .canonicalCombiningClass = 230},
         {.low = 0x0859, .high = 0x085B, .canonicalCombiningClass = 220},
         {.low = 0x08CA, .high = 0x08CF, .canonicalCombiningClass = 230},
         {.low = 0x08D0, .high = 0x08D1, .canonicalCombiningClass = 220},
         {.low = 0x08D2, .high = 0x08D2, .canonicalCombiningClass = 230},
         {.low = 0x08D3, .high = 0x08D3, .canonicalCombiningClass = 220},
         {.low = 0x08D4, .high = 0x08D8, .canonicalCombiningClass = 230},
         {.low = 0x08D9, .high = 0x08D9, .canonicalCombiningClass = 220},
         {.low = 0x08DA, .high = 0x08DF, .canonicalCombiningClass = 230},
         {.low = 0x08E0, .high = 0x08E1, .canonicalCombiningClass = 220},
         {.low = 0x08E3, .high = 0x08E8, .canonicalCombiningClass = 230},
         {.low = 0x08E9, .high = 0x08E9, .canonicalCombiningClass = 220},
         {.low = 0x08EA, .high = 0x08EC, .canonicalCombiningClass = 230},
         {.low = 0x08ED, .high = 0x08EF, .canonicalCombiningClass = 220},
         {.low = 0x08F0, .high = 0x08F1, .canonicalCombiningClass = 230},
         {.low = 0x08F2, .high = 0x08F2, .canonicalCombiningClass = 220},
         {.low = 0x08F3, .high = 0x08FA, .canonicalCombiningClass = 230},
         {.low = 0x08FB, .high = 0x08FF, .canonicalCombiningClass = 230},
         {.low = 0x093C, .high = 0x093C, .canonicalCombiningClass = 7},
         {.low = 0x094D, .high = 0x094D, .canonicalCombiningClass = 9},
         {.low = 0x0951, .high = 0x0951, .canonicalCombiningClass = 230},
         {.low = 0x0952, .high = 0x0952, .canonicalCombiningClass = 220},
         {.low = 0x09BC, .high = 0x09BC, .canonicalCombiningClass = 7},
         {.low = 0x09CD, .high = 0x09CD, .canonicalCombiningClass = 9},
         {.low = 0x09FE, .high = 0x09FE, .canonicalCombiningClass = 230},
         {.low = 0x0A3C, .high = 0x0A3C, .canonicalCombiningClass = 7},
         {.low = 0x0A4D, .high = 0x0A4D, .canonicalCombiningClass = 9},
         {.low = 0x0ABC, .high = 0x0ABC, .canonicalCombiningClass = 7},
         {.low = 0x0ACD, .high = 0x0ACD, .canonicalCombiningClass = 9},
         {.low = 0x0B3C, .high = 0x0B3C, .canonicalCombiningClass = 7},
         {.low = 0x0B4D, .high = 0x0B4D, .canonicalCombiningClass = 9},
         {.low = 0x0BCD, .high = 0x0BCD, .canonicalCombiningClass = 9},
         {.low = 0x0C4D, .high = 0x0C4D, .canonicalCombiningClass = 9},
         {.low = 0x0CBC, .high = 0x0CBC, .canonicalCombiningClass = 7},
         {.low = 0x0CCD, .high = 0x0CCD, .canonicalCombiningClass = 9},
         {.low = 0x0D4D, .high = 0x0D4D, .canonicalCombiningClass = 9},
         {.low = 0x0DCA, .high = 0x0DCA, .canonicalCombiningClass = 9},
         {.low = 0x0E38, .high = 0x0E39, .canonicalCombiningClass = 103},
         {.low = 0x0E3A, .high = 0x0E3A, .canonicalCombiningClass = 9},
         {.low = 0x0E48, .high = 0x0E4B, .canonicalCombiningClass = 107},
         {.low = 0x0EB8, .high = 0x0EB9, .canonicalCombiningClass = 118},
         {.low = 0x0EBA, .high = 0x0EBA, .canonicalCombiningClass = 9},
         {.low = 0x0EC8, .high = 0x0ECB, .canonicalCombiningClass = 122},
         {.low = 0x0F18, .high = 0x0F19, .canonicalCombiningClass = 220},
         {.low = 0x0F35, .high = 0x0F35, .canonicalCombiningClass = 220},
         {.low = 0x0F37, .high = 0x0F37, .canonicalCombiningClass = 220},
         {.low = 0x0F39, .high = 0x0F39, .canonicalCombiningClass = 216},
         {.low = 0x0F71, .high = 0x0F71, .canonicalCombiningClass = 129},
         {.low = 0x0F72, .high = 0x0F72, .canonicalCombiningClass = 130},
         {.low = 0x0F74, .high = 0x0F74, .canonicalCombiningClass = 132},
         {.low = 0x0F7E, .high = 0x0F7E, .canonicalCombiningClass = 130},
         {.low = 0x0F80, .high = 0x0F80, .canonicalCombiningClass = 130},
         {.low = 0x0F82, .high = 0x0F83, .canonicalCombiningClass = 230},
         {.low = 0x0F84, .high = 0x0F84, .canonicalCombiningClass = 9},
         {.low = 0x0F86, .high = 0x0F87, .canonicalCombiningClass = 230},
         {.low = 0x0FC6, .high = 0x0FC6, .canonicalCombiningClass = 220},
         {.low = 0x1037, .high = 0x1037, .canonicalCombiningClass = 7},
         {.low = 0x1039, .high = 0x103A, .canonicalCombiningClass = 9},
         {.low = 0x108D, .high = 0x108D, .canonicalCombiningClass = 220},
         {.low = 0x135D, .high = 0x135F, .canonicalCombiningClass = 230},
         {.low = 0x1714, .high = 0x1714, .canonicalCombiningClass = 9},
         {.low = 0x1734, .high = 0x1734, .canonicalCombiningClass = 9},
         {.low = 0x17D2, .high = 0x17D2, .canonicalCombiningClass = 9},
         {.low = 0x17DD, .high = 0x17DD, .canonicalCombiningClass = 230},
         {.low = 0x18A9, .high = 0x18A9, .canonicalCombiningClass = 228},
         {.low = 0x1939, .high = 0x1939, .canonicalCombiningClass = 222},
         {.low = 0x193A, .high = 0x193A, .canonicalCombiningClass = 230},
         {.low = 0x193B, .high = 0x193B, .canonicalCombiningClass = 220},
         {.low = 0x1A60, .high = 0x1A60, .canonicalCombiningClass = 9},
         {.low = 0x1A75, .high = 0x1A7C, .canonicalCombiningClass = 230},
         {.low = 0x1A7F, .high = 0x1A7F, .canonicalCombiningClass = 220},
         {.low = 0x1AB0, .high = 0x1ABE, .canonicalCombiningClass = 230},
         {.low = 0x1B34, .high = 0x1B34, .canonicalCombiningClass = 7},
         {.low = 0x1B44, .high = 0x1B44, .canonicalCombiningClass = 9},
         {.low = 0x1B6B, .high = 0x1B6B, .canonicalCombiningClass = 230},
         {.low = 0x1B6C, .high = 0x1B6C, .canonicalCombiningClass = 220},
         {.low = 0x1B6D, .high = 0x1B73, .canonicalCombiningClass = 230},
         {.low = 0x1BAA, .high = 0x1BAA, .canonicalCombiningClass = 9},
         {.low = 0x1BAB, .high = 0x1BAB, .canonicalCombiningClass = 9},
         {.low = 0x1BE6, .high = 0x1BE6, .canonicalCombiningClass = 7},
         {.low = 0x1BED, .high = 0x1BED, .canonicalCombiningClass = 220},
         {.low = 0x1BEF, .high = 0x1BF1, .canonicalCombiningClass = 220},
         {.low = 0x1C37, .high = 0x1C37, .canonicalCombiningClass = 7},
         {.low = 0x1CD0, .high = 0x1CD0, .canonicalCombiningClass = 230},
         {.low = 0x1CD1, .high = 0x1CD1, .canonicalCombiningClass = 220},
         {.low = 0x1CD2, .high = 0x1CD2, .canonicalCombiningClass = 230},
         {.low = 0x1CDA, .high = 0x1CDB, .canonicalCombiningClass = 230},
         {.low = 0x1CDC, .high = 0x1CDF, .canonicalCombiningClass = 220},
         {.low = 0x1CE0, .high = 0x1CE0, .canonicalCombiningClass = 230},
         {.low = 0x1CE2, .high = 0x1CE8, .canonicalCombiningClass = 1},
         {.low = 0x1CED, .high = 0x1CED, .canonicalCombiningClass = 220},
         {.low = 0x1CF4, .high = 0x1CF4, .canonicalCombiningClass = 230},
         {.low = 0x1CF8, .high = 0x1CF9, .canonicalCombiningClass = 230},
         {.low = 0x1DC0, .high = 0x1DCF, .canonicalCombiningClass = 230},
         {.low = 0x1DD0, .high = 0x1DD0, .canonicalCombiningClass = 220},
         {.low = 0x1DD1, .high = 0x1DFF, .canonicalCombiningClass = 230},
         {.low = 0x20D0, .high = 0x20D1, .canonicalCombiningClass = 230},
         {.low = 0x20D2, .high = 0x20D3, .canonicalCombiningClass = 1},
         {.low = 0x20D4, .high = 0x20D7, .canonicalCombiningClass = 230},
         {.low = 0x20D8, .high = 0x20DA, .canonicalCombiningClass = 1},
         {.low = 0x20DB, .high = 0x20DC, .canonicalCombiningClass = 230},
         {.low = 0x20E1, .high = 0x20E1, .canonicalCombiningClass = 230},
         {.low = 0x20E5, .high = 0x20E6, .canonicalCombiningClass = 1},
         {.low = 0x20E7, .high = 0x20E7, .canonicalCombiningClass = 230},
         {.low = 0x20E8, .high = 0x20E8, .canonicalCombiningClass = 220},
         {.low = 0x20E9, .high = 0x20E9, .canonicalCombiningClass = 230},
         {.low = 0x20EA, .high = 0x20EB, .canonicalCombiningClass = 1},
         {.low = 0x20EC, .high = 0x20EF, .canonicalCombiningClass = 220},
         {.low = 0x20F0, .high = 0x20F0, .canonicalCombiningClass = 230},
         {.low = 0xFE20, .high = 0xFE26, .canonicalCombiningClass = 230},
         {.low = 0xFE27, .high = 0xFE2D, .canonicalCombiningClass = 220},
         {.low = 0xFE2E, .high = 0xFE2F, .canonicalCombiningClass = 230},
     });

     std::size_t low = 0;
     std::size_t high = canonicalCombiningClassRanges.size();
     while (low < high)
     {
          const std::size_t mid = low + ((high - low) / 2);
          if (canonicalCombiningClassRanges[mid].high < codePoint) low = mid + 1;
          else if (canonicalCombiningClassRanges[mid].low > codePoint)
               high = mid;
          else
               return canonicalCombiningClassRanges[mid].canonicalCombiningClass;
     }
     return 0;
}
std::uint32_t UnicodeDatabase::CanonicalCompose(std::uint32_t starter, std::uint32_t combining) noexcept
{
     constexpr std::uint32_t LeadingBase = 0x1100u;
     constexpr std::uint32_t VowelBase = 0x1161u;
     constexpr std::uint32_t TrailingBase = 0x11A7u;
     constexpr std::uint32_t SyllableBase = 0xAC00u;
     constexpr std::uint32_t LeadingCount = 19u;
     constexpr std::uint32_t VowelCount = 21u;
     constexpr std::uint32_t TrailingCount = 28u;
     constexpr std::uint32_t VowelTrailingCount = VowelCount * TrailingCount;
     constexpr std::uint32_t SyllableCount = LeadingCount * VowelTrailingCount;

     if (starter >= LeadingBase && starter < LeadingBase + LeadingCount && combining >= VowelBase &&
         combining < VowelBase + VowelCount)
          return SyllableBase + ((((starter - LeadingBase) * VowelCount) + (combining - VowelBase)) * TrailingCount);

     if (starter >= SyllableBase && starter < SyllableBase + SyllableCount &&
         (starter - SyllableBase) % TrailingCount == 0 && combining > TrailingBase &&
         combining < TrailingBase + TrailingCount)
          return starter + (combining - TrailingBase);

     struct CanonicalCompositionTwoFields
     {
          std::uint32_t baseCharacter{};
          std::uint32_t combiningMark{};
          std::uint32_t combinedCharacter{};
     };
     static constexpr auto canonicalCompositionTwoFields = std::to_array<CanonicalCompositionTwoFields>({
         {.baseCharacter = 0x0041u, .combiningMark = 0x0300u, .combinedCharacter = 0x00C0u},
         {.baseCharacter = 0x0061u, .combiningMark = 0x0300u, .combinedCharacter = 0x00E0u},
         {.baseCharacter = 0x0041u, .combiningMark = 0x0301u, .combinedCharacter = 0x00C1u},
         {.baseCharacter = 0x0061u, .combiningMark = 0x0301u, .combinedCharacter = 0x00E1u},
         {.baseCharacter = 0x0041u, .combiningMark = 0x0302u, .combinedCharacter = 0x00C2u},
         {.baseCharacter = 0x0061u, .combiningMark = 0x0302u, .combinedCharacter = 0x00E2u},
         {.baseCharacter = 0x0041u, .combiningMark = 0x0303u, .combinedCharacter = 0x00C3u},
         {.baseCharacter = 0x0061u, .combiningMark = 0x0303u, .combinedCharacter = 0x00E3u},
         {.baseCharacter = 0x0041u, .combiningMark = 0x0308u, .combinedCharacter = 0x00C4u},
         {.baseCharacter = 0x0061u, .combiningMark = 0x0308u, .combinedCharacter = 0x00E4u},
         {.baseCharacter = 0x0041u, .combiningMark = 0x030Au, .combinedCharacter = 0x00C5u},
         {.baseCharacter = 0x0061u, .combiningMark = 0x030Au, .combinedCharacter = 0x00E5u},
         {.baseCharacter = 0x0043u, .combiningMark = 0x0327u, .combinedCharacter = 0x00C7u},
         {.baseCharacter = 0x0063u, .combiningMark = 0x0327u, .combinedCharacter = 0x00E7u},
         {.baseCharacter = 0x0045u, .combiningMark = 0x0300u, .combinedCharacter = 0x00C8u},
         {.baseCharacter = 0x0065u, .combiningMark = 0x0300u, .combinedCharacter = 0x00E8u},
         {.baseCharacter = 0x0045u, .combiningMark = 0x0301u, .combinedCharacter = 0x00C9u},
         {.baseCharacter = 0x0065u, .combiningMark = 0x0301u, .combinedCharacter = 0x00E9u},
         {.baseCharacter = 0x0045u, .combiningMark = 0x0302u, .combinedCharacter = 0x00CAu},
         {.baseCharacter = 0x0065u, .combiningMark = 0x0302u, .combinedCharacter = 0x00EAu},
         {.baseCharacter = 0x0045u, .combiningMark = 0x0308u, .combinedCharacter = 0x00CBu},
         {.baseCharacter = 0x0065u, .combiningMark = 0x0308u, .combinedCharacter = 0x00EBu},
         {.baseCharacter = 0x0049u, .combiningMark = 0x0300u, .combinedCharacter = 0x00CCu},
         {.baseCharacter = 0x0069u, .combiningMark = 0x0300u, .combinedCharacter = 0x00ECu},
         {.baseCharacter = 0x0049u, .combiningMark = 0x0301u, .combinedCharacter = 0x00CDu},
         {.baseCharacter = 0x0069u, .combiningMark = 0x0301u, .combinedCharacter = 0x00EDu},
         {.baseCharacter = 0x0049u, .combiningMark = 0x0302u, .combinedCharacter = 0x00CEu},
         {.baseCharacter = 0x0069u, .combiningMark = 0x0302u, .combinedCharacter = 0x00EEu},
         {.baseCharacter = 0x0049u, .combiningMark = 0x0308u, .combinedCharacter = 0x00CFu},
         {.baseCharacter = 0x0069u, .combiningMark = 0x0308u, .combinedCharacter = 0x00EFu},
         {.baseCharacter = 0x004Eu, .combiningMark = 0x0303u, .combinedCharacter = 0x00D1u},
         {.baseCharacter = 0x006Eu, .combiningMark = 0x0303u, .combinedCharacter = 0x00F1u},
         {.baseCharacter = 0x004Fu, .combiningMark = 0x0300u, .combinedCharacter = 0x00D2u},
         {.baseCharacter = 0x006Fu, .combiningMark = 0x0300u, .combinedCharacter = 0x00F2u},
         {.baseCharacter = 0x004Fu, .combiningMark = 0x0301u, .combinedCharacter = 0x00D3u},
         {.baseCharacter = 0x006Fu, .combiningMark = 0x0301u, .combinedCharacter = 0x00F3u},
         {.baseCharacter = 0x004Fu, .combiningMark = 0x0302u, .combinedCharacter = 0x00D4u},
         {.baseCharacter = 0x006Fu, .combiningMark = 0x0302u, .combinedCharacter = 0x00F4u},
         {.baseCharacter = 0x004Fu, .combiningMark = 0x0303u, .combinedCharacter = 0x00D5u},
         {.baseCharacter = 0x006Fu, .combiningMark = 0x0303u, .combinedCharacter = 0x00F5u},
         {.baseCharacter = 0x004Fu, .combiningMark = 0x0308u, .combinedCharacter = 0x00D6u},
         {.baseCharacter = 0x006Fu, .combiningMark = 0x0308u, .combinedCharacter = 0x00F6u},
         {.baseCharacter = 0x0055u, .combiningMark = 0x0300u, .combinedCharacter = 0x00D9u},
         {.baseCharacter = 0x0075u, .combiningMark = 0x0300u, .combinedCharacter = 0x00F9u},
         {.baseCharacter = 0x0055u, .combiningMark = 0x0301u, .combinedCharacter = 0x00DAu},
         {.baseCharacter = 0x0075u, .combiningMark = 0x0301u, .combinedCharacter = 0x00FAu},
         {.baseCharacter = 0x0055u, .combiningMark = 0x0302u, .combinedCharacter = 0x00DBu},
         {.baseCharacter = 0x0075u, .combiningMark = 0x0302u, .combinedCharacter = 0x00FBu},
         {.baseCharacter = 0x0055u, .combiningMark = 0x0308u, .combinedCharacter = 0x00DCu},
         {.baseCharacter = 0x0075u, .combiningMark = 0x0308u, .combinedCharacter = 0x00FCu},
         {.baseCharacter = 0x0059u, .combiningMark = 0x0301u, .combinedCharacter = 0x00DDu},
         {.baseCharacter = 0x0079u, .combiningMark = 0x0301u, .combinedCharacter = 0x00FDu},
         {.baseCharacter = 0x0059u, .combiningMark = 0x0308u, .combinedCharacter = 0x0178u},
         {.baseCharacter = 0x0079u, .combiningMark = 0x0308u, .combinedCharacter = 0x00FFu},
     });
     for (const auto& [a, b, c] : canonicalCompositionTwoFields)
          if (starter == a && combining == b) return c;
     return 0u;
}
struct DecomposedEntry
{
     std::uint32_t codePoint;
     std::array<std::uint32_t, 3> decomposed;
     std::uint8_t length;
};

std::span<const std::uint32_t> UnicodeDatabase::CanonicalDecompose(std::uint32_t codePoint,
                                                                   std::array<std::uint32_t, 3>& buffer) noexcept
{
     constexpr std::uint32_t SyllableBase = 0xAC00u;
     constexpr std::uint32_t LeadingBase = 0x1100u;
     constexpr std::uint32_t VowelBase = 0x1161u;
     constexpr std::uint32_t TrailingBase = 0x11A7u;
     constexpr std::uint32_t VowelCount = 21u;
     constexpr std::uint32_t TrailingCount = 28u;
     constexpr std::uint32_t VowelTrailingCount = VowelCount * TrailingCount;

     if (codePoint >= SyllableBase && codePoint <= 0xD7A3u)
     {
          const std::uint32_t syllableIndex = codePoint - SyllableBase;

          buffer[0] = LeadingBase + (syllableIndex / VowelTrailingCount);
          buffer[1] = VowelBase + ((syllableIndex % VowelTrailingCount) / TrailingCount);

          const std::uint32_t trailingCharacter = TrailingBase + (syllableIndex % TrailingCount);
          if (trailingCharacter != TrailingBase)
          {
               buffer[2] = trailingCharacter;
               return {buffer.data(), 3};
          }
          return {buffer.data(), 2};
     }

     struct CanonicalDecompositionTwoFields
     {
          std::uint32_t combinedCharacter;
          std::uint32_t baseCharacter;
          std::uint32_t combiningMark;
     };
     static constexpr auto canonicalDecompositionTwoFields = std::to_array<CanonicalDecompositionTwoFields>({
         {.combinedCharacter = 0x00C1u, .baseCharacter = 0x0041u, .combiningMark = 0x0301u},
         {.combinedCharacter = 0x00E1u, .baseCharacter = 0x0061u, .combiningMark = 0x0301u},
         {.combinedCharacter = 0x00C9u, .baseCharacter = 0x0045u, .combiningMark = 0x0301u},
         {.combinedCharacter = 0x00E9u, .baseCharacter = 0x0065u, .combiningMark = 0x0301u},
         {.combinedCharacter = 0x00CDu, .baseCharacter = 0x0049u, .combiningMark = 0x0301u},
         {.combinedCharacter = 0x00EDu, .baseCharacter = 0x0069u, .combiningMark = 0x0301u},
         {.combinedCharacter = 0x00D3u, .baseCharacter = 0x004Fu, .combiningMark = 0x0301u},
         {.combinedCharacter = 0x00F3u, .baseCharacter = 0x006Fu, .combiningMark = 0x0301u},
         {.combinedCharacter = 0x00DAu, .baseCharacter = 0x0055u, .combiningMark = 0x0301u},
         {.combinedCharacter = 0x00FAu, .baseCharacter = 0x0075u, .combiningMark = 0x0301u},
         {.combinedCharacter = 0x00DDu, .baseCharacter = 0x0059u, .combiningMark = 0x0301u},
         {.combinedCharacter = 0x00FDu, .baseCharacter = 0x0079u, .combiningMark = 0x0301u},
         {.combinedCharacter = 0x00C0u, .baseCharacter = 0x0041u, .combiningMark = 0x0300u},
         {.combinedCharacter = 0x00E0u, .baseCharacter = 0x0061u, .combiningMark = 0x0300u},
         {.combinedCharacter = 0x00C8u, .baseCharacter = 0x0045u, .combiningMark = 0x0300u},
         {.combinedCharacter = 0x00E8u, .baseCharacter = 0x0065u, .combiningMark = 0x0300u},
         {.combinedCharacter = 0x00CCu, .baseCharacter = 0x0049u, .combiningMark = 0x0300u},
         {.combinedCharacter = 0x00ECu, .baseCharacter = 0x0069u, .combiningMark = 0x0300u},
         {.combinedCharacter = 0x00D2u, .baseCharacter = 0x004Fu, .combiningMark = 0x0300u},
         {.combinedCharacter = 0x00F2u, .baseCharacter = 0x006Fu, .combiningMark = 0x0300u},
         {.combinedCharacter = 0x00D9u, .baseCharacter = 0x0055u, .combiningMark = 0x0300u},
         {.combinedCharacter = 0x00F9u, .baseCharacter = 0x0075u, .combiningMark = 0x0300u},
         {.combinedCharacter = 0x00C2u, .baseCharacter = 0x0041u, .combiningMark = 0x0302u},
         {.combinedCharacter = 0x00E2u, .baseCharacter = 0x0061u, .combiningMark = 0x0302u},
         {.combinedCharacter = 0x00CAu, .baseCharacter = 0x0045u, .combiningMark = 0x0302u},
         {.combinedCharacter = 0x00EAu, .baseCharacter = 0x0065u, .combiningMark = 0x0302u},
         {.combinedCharacter = 0x00CEu, .baseCharacter = 0x0049u, .combiningMark = 0x0302u},
         {.combinedCharacter = 0x00EEu, .baseCharacter = 0x0069u, .combiningMark = 0x0302u},
         {.combinedCharacter = 0x00D4u, .baseCharacter = 0x004Fu, .combiningMark = 0x0302u},
         {.combinedCharacter = 0x00F4u, .baseCharacter = 0x006Fu, .combiningMark = 0x0302u},
         {.combinedCharacter = 0x00DBu, .baseCharacter = 0x0055u, .combiningMark = 0x0302u},
         {.combinedCharacter = 0x00FBu, .baseCharacter = 0x0075u, .combiningMark = 0x0302u},
         {.combinedCharacter = 0x00C4u, .baseCharacter = 0x0041u, .combiningMark = 0x0308u},
         {.combinedCharacter = 0x00E4u, .baseCharacter = 0x0061u, .combiningMark = 0x0308u},
         {.combinedCharacter = 0x00CBu, .baseCharacter = 0x0045u, .combiningMark = 0x0308u},
         {.combinedCharacter = 0x00EBu, .baseCharacter = 0x0065u, .combiningMark = 0x0308u},
         {.combinedCharacter = 0x00CFu, .baseCharacter = 0x0049u, .combiningMark = 0x0308u},
         {.combinedCharacter = 0x00EFu, .baseCharacter = 0x0069u, .combiningMark = 0x0308u},
         {.combinedCharacter = 0x00D6u, .baseCharacter = 0x004Fu, .combiningMark = 0x0308u},
         {.combinedCharacter = 0x00F6u, .baseCharacter = 0x006Fu, .combiningMark = 0x0308u},
         {.combinedCharacter = 0x00DCu, .baseCharacter = 0x0055u, .combiningMark = 0x0308u},
         {.combinedCharacter = 0x00FCu, .baseCharacter = 0x0075u, .combiningMark = 0x0308u},
         {.combinedCharacter = 0x0178u, .baseCharacter = 0x0059u, .combiningMark = 0x0308u},
         {.combinedCharacter = 0x00FFu, .baseCharacter = 0x0079u, .combiningMark = 0x0308u},
         {.combinedCharacter = 0x00C3u, .baseCharacter = 0x0041u, .combiningMark = 0x0303u},
         {.combinedCharacter = 0x00E3u, .baseCharacter = 0x0061u, .combiningMark = 0x0303u},
         {.combinedCharacter = 0x00D1u, .baseCharacter = 0x004Eu, .combiningMark = 0x0303u},
         {.combinedCharacter = 0x00F1u, .baseCharacter = 0x006Eu, .combiningMark = 0x0303u},
         {.combinedCharacter = 0x00D5u, .baseCharacter = 0x004Fu, .combiningMark = 0x0303u},
         {.combinedCharacter = 0x00F5u, .baseCharacter = 0x006Fu, .combiningMark = 0x0303u},
         {.combinedCharacter = 0x00C5u, .baseCharacter = 0x0041u, .combiningMark = 0x030Au},
         {.combinedCharacter = 0x00E5u, .baseCharacter = 0x0061u, .combiningMark = 0x030Au},
         {.combinedCharacter = 0x00C7u, .baseCharacter = 0x0043u, .combiningMark = 0x0327u},
         {.combinedCharacter = 0x00E7u, .baseCharacter = 0x0063u, .combiningMark = 0x0327u},
     });
     for (const auto& [c, a, b] : canonicalDecompositionTwoFields)
     {
          if (codePoint != c) continue;
          buffer[0] = a;
          buffer[1] = b;
          return {buffer.data(), 2};
     }

     return {};
}
struct ArabicMapping
{
     std::uint32_t low{};
     std::uint32_t high{};
     std::uint32_t base{};
};

std::span<const std::uint32_t> UnicodeDatabase::CompatibilityDecompose(std::uint32_t codePoint,
                                                                       std::array<std::uint32_t, 8>& buffer) noexcept
{
     if (codePoint >= 0xFF01u && codePoint <= 0xFF5Eu)
     {
          buffer[0] = codePoint - 0xFF01u + 0x0021u;
          return {buffer.data(), 1};
     }

     static constexpr auto hwKata = std::to_array<std::uint32_t>({
         0xFF65, 0xFF66, 0xFF67, 0xFF68, 0xFF69, 0xFF6A, 0xFF6B, 0xFF6C, 0xFF6D, 0xFF6E, 0xFF6F, 0xFF70,
         0x30FB, 0x30F2, 0x30A1, 0x30A3, 0x30A5, 0x30A7, 0x30A9, 0x30E3, 0x30E5, 0x30E7, 0x30C3, 0x30FC,
         0x30A2, 0x30A4, 0x30A6, 0x30A8, 0x30AA, 0x30AB, 0x30AD, 0x30AF, 0x30B1, 0x30B3, 0x30B5, 0x30B7,
         0x30B9, 0x30BB, 0x30BD, 0x30BF, 0x30C1, 0x30C4, 0x30C6, 0x30C8, 0x30CA, 0x30CB, 0x30CC, 0x30CD,
         0x30CE, 0x30CF, 0x30D2, 0x30D5, 0x30D8, 0x30DB, 0x30DE, 0x30DF, 0x30E0, 0x30E1, 0x30E2, 0x30E4,
         0x30E6, 0x30E8, 0x30E9, 0x30EA, 0x30EB, 0x30EC, 0x30ED, 0x30EF, 0x30F3, 0x309B, 0x309C,
     });
     if (codePoint >= 0xFF65u && codePoint <= 0xFF9Fu)
     {
          buffer[0] = hwKata[codePoint - 0xFF65u];
          return {buffer.data(), 1};
     }

     struct Mangul
     {
          std::uint32_t codePoint, base;
     };
     static constexpr auto hwHangulMap = std::to_array<Mangul>({
         {.codePoint = 0xFFA0, .base = 0x3164}, {.codePoint = 0xFFA1, .base = 0x3131},
         {.codePoint = 0xFFA2, .base = 0x3132}, {.codePoint = 0xFFA3, .base = 0x3133},
         {.codePoint = 0xFFA4, .base = 0x3134}, {.codePoint = 0xFFA5, .base = 0x3135},
         {.codePoint = 0xFFA6, .base = 0x3136}, {.codePoint = 0xFFA7, .base = 0x3137},
         {.codePoint = 0xFFA9, .base = 0x3138}, {.codePoint = 0xFFAA, .base = 0x3139},
         {.codePoint = 0xFFAB, .base = 0x313A}, {.codePoint = 0xFFAC, .base = 0x313B},
         {.codePoint = 0xFFAD, .base = 0x313C}, {.codePoint = 0xFFAE, .base = 0x313D},
         {.codePoint = 0xFFAF, .base = 0x313E}, {.codePoint = 0xFFB0, .base = 0x313F},
         {.codePoint = 0xFFB1, .base = 0x3140}, {.codePoint = 0xFFB2, .base = 0x3141},
         {.codePoint = 0xFFB3, .base = 0x3142}, {.codePoint = 0xFFB4, .base = 0x3143},
         {.codePoint = 0xFFB5, .base = 0x3144}, {.codePoint = 0xFFB6, .base = 0x3145},
         {.codePoint = 0xFFB7, .base = 0x3146}, {.codePoint = 0xFFB8, .base = 0x3147},
         {.codePoint = 0xFFB9, .base = 0x3148}, {.codePoint = 0xFFBA, .base = 0x3149},
         {.codePoint = 0xFFBB, .base = 0x314A}, {.codePoint = 0xFFBC, .base = 0x314B},
         {.codePoint = 0xFFBD, .base = 0x314C}, {.codePoint = 0xFFBE, .base = 0x314D},
         {.codePoint = 0xFFC2, .base = 0x314F}, {.codePoint = 0xFFC3, .base = 0x3150},
         {.codePoint = 0xFFC4, .base = 0x3151}, {.codePoint = 0xFFC5, .base = 0x3152},
         {.codePoint = 0xFFC6, .base = 0x3153}, {.codePoint = 0xFFC7, .base = 0x3154},
         {.codePoint = 0xFFCA, .base = 0x3155}, {.codePoint = 0xFFCB, .base = 0x3156},
         {.codePoint = 0xFFCC, .base = 0x3157}, {.codePoint = 0xFFCD, .base = 0x3158},
         {.codePoint = 0xFFCE, .base = 0x3159}, {.codePoint = 0xFFCF, .base = 0x315A},
         {.codePoint = 0xFFD2, .base = 0x315B}, {.codePoint = 0xFFD3, .base = 0x315C},
         {.codePoint = 0xFFD4, .base = 0x315D}, {.codePoint = 0xFFD5, .base = 0x315E},
         {.codePoint = 0xFFD6, .base = 0x315F}, {.codePoint = 0xFFD7, .base = 0x3160},
         {.codePoint = 0xFFDA, .base = 0x3161}, {.codePoint = 0xFFDB, .base = 0x3162},
         {.codePoint = 0xFFDC, .base = 0x3163},
     });

     for (const auto& [c, b] : hwHangulMap)
          if (codePoint == c)
          {
               buffer[0] = b;
               return {buffer.data(), 1};
          }
     static constexpr auto arabicMappingB = std::to_array<ArabicMapping>({
         {.low = 0xFE70, .high = 0xFE70, .base = 0x064Bu}, {.low = 0xFE71, .high = 0xFE71, .base = 0x064Bu},
         {.low = 0xFE72, .high = 0xFE72, .base = 0x064Cu}, {.low = 0xFE74, .high = 0xFE74, .base = 0x064Du},
         {.low = 0xFE76, .high = 0xFE76, .base = 0x064Eu}, {.low = 0xFE77, .high = 0xFE77, .base = 0x064Eu},
         {.low = 0xFE78, .high = 0xFE78, .base = 0x064Fu}, {.low = 0xFE79, .high = 0xFE79, .base = 0x064Fu},
         {.low = 0xFE7A, .high = 0xFE7A, .base = 0x0650u}, {.low = 0xFE7B, .high = 0xFE7B, .base = 0x0650u},
         {.low = 0xFE7C, .high = 0xFE7C, .base = 0x0651u}, {.low = 0xFE7D, .high = 0xFE7D, .base = 0x0651u},
         {.low = 0xFE7E, .high = 0xFE7E, .base = 0x0652u}, {.low = 0xFE7F, .high = 0xFE7F, .base = 0x0652u},
         {.low = 0xFE80, .high = 0xFE80, .base = 0x0621u}, {.low = 0xFE81, .high = 0xFE82, .base = 0x0622u},
         {.low = 0xFE83, .high = 0xFE84, .base = 0x0623u}, {.low = 0xFE85, .high = 0xFE86, .base = 0x0624u},
         {.low = 0xFE87, .high = 0xFE88, .base = 0x0625u}, {.low = 0xFE89, .high = 0xFE8C, .base = 0x0626u},
         {.low = 0xFE8D, .high = 0xFE8E, .base = 0x0627u}, {.low = 0xFE8F, .high = 0xFE92, .base = 0x0628u},
         {.low = 0xFE93, .high = 0xFE94, .base = 0x0629u}, {.low = 0xFE95, .high = 0xFE98, .base = 0x062Au},
         {.low = 0xFE99, .high = 0xFE9C, .base = 0x062Bu}, {.low = 0xFE9D, .high = 0xFEA0, .base = 0x062Cu},
         {.low = 0xFEA1, .high = 0xFEA4, .base = 0x062Du}, {.low = 0xFEA5, .high = 0xFEA8, .base = 0x062Eu},
         {.low = 0xFEA9, .high = 0xFEAA, .base = 0x062Fu}, {.low = 0xFEAB, .high = 0xFEAC, .base = 0x0630u},
         {.low = 0xFEAD, .high = 0xFEAE, .base = 0x0631u}, {.low = 0xFEAF, .high = 0xFEB0, .base = 0x0632u},
         {.low = 0xFEB1, .high = 0xFEB4, .base = 0x0633u}, {.low = 0xFEB5, .high = 0xFEB8, .base = 0x0634u},
         {.low = 0xFEB9, .high = 0xFEBC, .base = 0x0635u}, {.low = 0xFEBD, .high = 0xFEC0, .base = 0x0636u},
         {.low = 0xFEC1, .high = 0xFEC4, .base = 0x0637u}, {.low = 0xFEC5, .high = 0xFEC8, .base = 0x0638u},
         {.low = 0xFEC9, .high = 0xFECC, .base = 0x0639u}, {.low = 0xFECD, .high = 0xFED0, .base = 0x063Au},
         {.low = 0xFED1, .high = 0xFED4, .base = 0x0641u}, {.low = 0xFED5, .high = 0xFED8, .base = 0x0642u},
         {.low = 0xFED9, .high = 0xFEDC, .base = 0x0643u}, {.low = 0xFEDD, .high = 0xFEE0, .base = 0x0644u},
         {.low = 0xFEE1, .high = 0xFEE4, .base = 0x0645u}, {.low = 0xFEE5, .high = 0xFEE8, .base = 0x0646u},
         {.low = 0xFEE9, .high = 0xFEEC, .base = 0x0647u}, {.low = 0xFEED, .high = 0xFEEE, .base = 0x0648u},
         {.low = 0xFEEF, .high = 0xFEF0, .base = 0x0649u}, {.low = 0xFEF1, .high = 0xFEF4, .base = 0x064Au},
     });
     for (const auto& [low, high, base] : arabicMappingB)
          if (codePoint >= low && codePoint <= high)
          {
               buffer[0] = base;
               return {buffer.data(), 1};
          }

     struct LamAlefCombination
     {
          std::uint32_t character;
          std::uint32_t alef;
     };
     static constexpr auto lamAlefCombination = std::to_array<LamAlefCombination>({
         {.character = 0xFEF5u, .alef = 0x0622u},
         {.character = 0xFEF6u, .alef = 0x0622u},
         {.character = 0xFEF7u, .alef = 0x0623u},
         {.character = 0xFEF8u, .alef = 0x0623u},
         {.character = 0xFEF9u, .alef = 0x0625u},
         {.character = 0xFEFAu, .alef = 0x0625u},
         {.character = 0xFEFBu, .alef = 0x0627u},
         {.character = 0xFEFCu, .alef = 0x0627u},
     });
     for (const auto& [nestedCodePoint, alef] : lamAlefCombination)
          if (codePoint == nestedCodePoint)
          {
               buffer[0] = 0x0644u;
               buffer[1] = alef;
               return {buffer.data(), 2};
          }

     static constexpr auto arabicMappingA = std::to_array<ArabicMapping>({
         {.low = 0xFB50u, .high = 0xFB51u, .base = 0x0671u}, {.low = 0xFB52u, .high = 0xFB55u, .base = 0x067Bu},
         {.low = 0xFB56u, .high = 0xFB59u, .base = 0x067Eu}, {.low = 0xFB5Au, .high = 0xFB5Du, .base = 0x0680u},
         {.low = 0xFB5Eu, .high = 0xFB61u, .base = 0x067Au}, {.low = 0xFB62u, .high = 0xFB65u, .base = 0x067Fu},
         {.low = 0xFB66u, .high = 0xFB69u, .base = 0x0679u}, {.low = 0xFB6Au, .high = 0xFB6Du, .base = 0x06A4u},
         {.low = 0xFB6Eu, .high = 0xFB71u, .base = 0x06A6u}, {.low = 0xFB72u, .high = 0xFB75u, .base = 0x0684u},
         {.low = 0xFB76u, .high = 0xFB79u, .base = 0x0683u}, {.low = 0xFB7Au, .high = 0xFB7Du, .base = 0x0686u},
         {.low = 0xFB7Eu, .high = 0xFB81u, .base = 0x0687u}, {.low = 0xFB82u, .high = 0xFB83u, .base = 0x068Du},
         {.low = 0xFB84u, .high = 0xFB85u, .base = 0x068Cu}, {.low = 0xFB86u, .high = 0xFB87u, .base = 0x068Eu},
         {.low = 0xFB88u, .high = 0xFB89u, .base = 0x0688u}, {.low = 0xFB8Au, .high = 0xFB8Bu, .base = 0x0698u},
         {.low = 0xFB8Cu, .high = 0xFB8Du, .base = 0x0691u}, {.low = 0xFB8Eu, .high = 0xFB91u, .base = 0x06A9u},
         {.low = 0xFB92u, .high = 0xFB95u, .base = 0x06AFu}, {.low = 0xFB96u, .high = 0xFB99u, .base = 0x06B3u},
         {.low = 0xFB9Au, .high = 0xFB9Du, .base = 0x06B1u}, {.low = 0xFB9Eu, .high = 0xFB9Fu, .base = 0x06BAu},
         {.low = 0xFBA0u, .high = 0xFBA3u, .base = 0x06BBu}, {.low = 0xFBA4u, .high = 0xFBA5u, .base = 0x06C0u},
         {.low = 0xFBA6u, .high = 0xFBA9u, .base = 0x06C1u}, {.low = 0xFBAAu, .high = 0xFBABu, .base = 0x06BEu},
         {.low = 0xFBACu, .high = 0xFBADu, .base = 0x06C3u}, {.low = 0xFBAEu, .high = 0xFBAFu, .base = 0x06D2u},
         {.low = 0xFBB0u, .high = 0xFBB1u, .base = 0x06D3u}, {.low = 0xFBD3u, .high = 0xFBD6u, .base = 0x06ADu},
         {.low = 0xFBD7u, .high = 0xFBD8u, .base = 0x06C7u}, {.low = 0xFBD9u, .high = 0xFBDAu, .base = 0x06C6u},
         {.low = 0xFBDBu, .high = 0xFBDCu, .base = 0x06C8u}, {.low = 0xFBDDu, .high = 0xFBDDu, .base = 0x06C7u},
         {.low = 0xFBDEu, .high = 0xFBDFu, .base = 0x06CBu}, {.low = 0xFBE0u, .high = 0xFBE1u, .base = 0x06C5u},
         {.low = 0xFBE2u, .high = 0xFBE3u, .base = 0x06C9u}, {.low = 0xFBE4u, .high = 0xFBE7u, .base = 0x06D0u},
         {.low = 0xFBE8u, .high = 0xFBE9u, .base = 0x0649u}, {.low = 0xFBEAu, .high = 0xFBEBu, .base = 0x064Au},
         {.low = 0xFBECu, .high = 0xFBEDu, .base = 0x064Au}, {.low = 0xFBEEu, .high = 0xFBEFu, .base = 0x064Au},
         {.low = 0xFBF0u, .high = 0xFBF1u, .base = 0x064Au}, {.low = 0xFBF2u, .high = 0xFBF3u, .base = 0x064Au},
         {.low = 0xFBF4u, .high = 0xFBF5u, .base = 0x064Au}, {.low = 0xFBF6u, .high = 0xFBF8u, .base = 0x064Au},
         {.low = 0xFBF9u, .high = 0xFBFBu, .base = 0x064Au}, {.low = 0xFBFCu, .high = 0xFBFFu, .base = 0x064Au},
     });
     for (const auto& [low, high, base] : arabicMappingA)
          if (codePoint >= low && codePoint <= high)
          {
               buffer[0] = base;
               return {buffer.data(), 1};
          }

     switch (codePoint)
     {
     case 0xFB00u:
          buffer[0] = 0x0066u;
          buffer[1] = 0x0066u;
          return {buffer.data(), 2};
     case 0xFB01u:
          buffer[0] = 0x0066u;
          buffer[1] = 0x0069u;
          return {buffer.data(), 2};
     case 0xFB02u:
          buffer[0] = 0x0066u;
          buffer[1] = 0x006Cu;
          return {buffer.data(), 2};
     case 0xFB03u:
          buffer[0] = 0x0066u;
          buffer[1] = 0x0066u;
          buffer[2] = 0x0069u;
          return {buffer.data(), 3};
     case 0xFB04u:
          buffer[0] = 0x0066u;
          buffer[1] = 0x0066u;
          buffer[2] = 0x006Cu;
          return {buffer.data(), 3};
     case 0xFB05u:
     case 0xFB06u:
          buffer[0] = 0x0073u;
          buffer[1] = 0x0074u;
          return {buffer.data(), 2};
     case 0x0132u:
          buffer[0] = 0x0049u;
          buffer[1] = 0x004Au;
          return {buffer.data(), 2};
     case 0x0133u:
          buffer[0] = 0x0069u;
          buffer[1] = 0x006Au;
          return {buffer.data(), 2};
     case 0x01F1u:
          buffer[0] = 0x0044u;
          buffer[1] = 0x005Au;
          return {buffer.data(), 2};
     case 0x01F2u:
          buffer[0] = 0x0044u;
          buffer[1] = 0x007Au;
          return {buffer.data(), 2};
     case 0x01F3u:
          buffer[0] = 0x0064u;
          buffer[1] = 0x007Au;
          return {buffer.data(), 2};
     case 0x01C7u:
          buffer[0] = 0x004Cu;
          buffer[1] = 0x004Au;
          return {buffer.data(), 2};
     case 0x01C8u:
          buffer[0] = 0x004Cu;
          buffer[1] = 0x006Au;
          return {buffer.data(), 2};
     case 0x01C9u:
          buffer[0] = 0x006Cu;
          buffer[1] = 0x006Au;
          return {buffer.data(), 2};
     case 0x01CAu:
          buffer[0] = 0x004Eu;
          buffer[1] = 0x004Au;
          return {buffer.data(), 2};
     case 0x01CBu:
          buffer[0] = 0x004Eu;
          buffer[1] = 0x006Au;
          return {buffer.data(), 2};
     case 0x01CCu:
          buffer[0] = 0x006Eu;
          buffer[1] = 0x006Au;
          return {buffer.data(), 2};
     default: break;
     }

     switch (codePoint)
     {
     case 0x00AAu: buffer[0] = 0x0061u; return {buffer.data(), 1};
     case 0x00BAu: buffer[0] = 0x006Fu; return {buffer.data(), 1};
     case 0x00B2u: buffer[0] = 0x0032u; return {buffer.data(), 1};
     case 0x00B3u: buffer[0] = 0x0033u; return {buffer.data(), 1};
     case 0x00B9u: buffer[0] = 0x0031u; return {buffer.data(), 1};
     default: break;
     }

     switch (codePoint)
     {
     case 0x00BCu:
          buffer[0] = 0x0031u;
          buffer[1] = 0x2044u;
          buffer[2] = 0x0034u;
          return {buffer.data(), 3};
     case 0x00BDu:
          buffer[0] = 0x0031u;
          buffer[1] = 0x2044u;
          buffer[2] = 0x0032u;
          return {buffer.data(), 3};
     case 0x00BEu:
          buffer[0] = 0x0033u;
          buffer[1] = 0x2044u;
          buffer[2] = 0x0034u;
          return {buffer.data(), 3};
     case 0x2150u:
          buffer[0] = 0x0031u;
          buffer[1] = 0x2044u;
          buffer[2] = 0x0037u;
          return {buffer.data(), 3};
     case 0x2151u:
          buffer[0] = 0x0031u;
          buffer[1] = 0x2044u;
          buffer[2] = 0x0039u;
          return {buffer.data(), 3};
     case 0x2152u:
          buffer[0] = 0x0031u;
          buffer[1] = 0x2044u;
          buffer[2] = 0x0031u;
          return {buffer.data(), 3};
     case 0x2153u:
          buffer[0] = 0x0031u;
          buffer[1] = 0x2044u;
          buffer[2] = 0x0033u;
          return {buffer.data(), 3};
     case 0x2154u:
          buffer[0] = 0x0032u;
          buffer[1] = 0x2044u;
          buffer[2] = 0x0033u;
          return {buffer.data(), 3};
     case 0x2155u:
          buffer[0] = 0x0031u;
          buffer[1] = 0x2044u;
          buffer[2] = 0x0035u;
          return {buffer.data(), 3};
     case 0x2156u:
          buffer[0] = 0x0032u;
          buffer[1] = 0x2044u;
          buffer[2] = 0x0035u;
          return {buffer.data(), 3};
     case 0x2157u:
          buffer[0] = 0x0033u;
          buffer[1] = 0x2044u;
          buffer[2] = 0x0035u;
          return {buffer.data(), 3};
     case 0x2158u:
          buffer[0] = 0x0034u;
          buffer[1] = 0x2044u;
          buffer[2] = 0x0035u;
          return {buffer.data(), 3};
     case 0x2159u:
          buffer[0] = 0x0031u;
          buffer[1] = 0x2044u;
          buffer[2] = 0x0036u;
          return {buffer.data(), 3};
     case 0x215Au:
          buffer[0] = 0x0035u;
          buffer[1] = 0x2044u;
          buffer[2] = 0x0036u;
          return {buffer.data(), 3};
     case 0x215Bu:
          buffer[0] = 0x0031u;
          buffer[1] = 0x2044u;
          buffer[2] = 0x0038u;
          return {buffer.data(), 3};
     case 0x215Cu:
          buffer[0] = 0x0033u;
          buffer[1] = 0x2044u;
          buffer[2] = 0x0038u;
          return {buffer.data(), 3};
     case 0x215Du:
          buffer[0] = 0x0035u;
          buffer[1] = 0x2044u;
          buffer[2] = 0x0038u;
          return {buffer.data(), 3};
     case 0x215Eu:
          buffer[0] = 0x0037u;
          buffer[1] = 0x2044u;
          buffer[2] = 0x0038u;
          return {buffer.data(), 3};
     default: break;
     }

     struct Superscript
     {
          std::uint32_t c;
          std::uint32_t b;
     };
     static constexpr auto superscript = std::to_array<Superscript>({
         {.c = 0x2070u, .b = 0x0030u}, {.c = 0x2071u, .b = 0x0069u}, {.c = 0x2074u, .b = 0x0034u},
         {.c = 0x2075u, .b = 0x0035u}, {.c = 0x2076u, .b = 0x0036u}, {.c = 0x2077u, .b = 0x0037u},
         {.c = 0x2078u, .b = 0x0038u}, {.c = 0x2079u, .b = 0x0039u}, {.c = 0x207Au, .b = 0x002Bu},
         {.c = 0x207Bu, .b = 0x2212u}, {.c = 0x207Cu, .b = 0x003Du}, {.c = 0x207Du, .b = 0x0028u},
         {.c = 0x207Eu, .b = 0x0029u}, {.c = 0x207Fu, .b = 0x006Eu}, {.c = 0x2080u, .b = 0x0030u},
         {.c = 0x2081u, .b = 0x0031u}, {.c = 0x2082u, .b = 0x0032u}, {.c = 0x2083u, .b = 0x0033u},
         {.c = 0x2084u, .b = 0x0034u}, {.c = 0x2085u, .b = 0x0035u}, {.c = 0x2086u, .b = 0x0036u},
         {.c = 0x2087u, .b = 0x0037u}, {.c = 0x2088u, .b = 0x0038u}, {.c = 0x2089u, .b = 0x0039u},
         {.c = 0x208Au, .b = 0x002Bu}, {.c = 0x208Bu, .b = 0x2212u}, {.c = 0x208Cu, .b = 0x003Du},
         {.c = 0x208Du, .b = 0x0028u}, {.c = 0x208Eu, .b = 0x0029u}, {.c = 0x2090u, .b = 0x0061u},
         {.c = 0x2091u, .b = 0x0065u}, {.c = 0x2092u, .b = 0x006Fu}, {.c = 0x2093u, .b = 0x0078u},
         {.c = 0x2094u, .b = 0x0259u}, {.c = 0x2095u, .b = 0x0068u}, {.c = 0x2096u, .b = 0x006Bu},
         {.c = 0x2097u, .b = 0x006Cu}, {.c = 0x2098u, .b = 0x006Du}, {.c = 0x2099u, .b = 0x006Eu},
         {.c = 0x209Au, .b = 0x0070u}, {.c = 0x209Bu, .b = 0x0073u}, {.c = 0x209Cu, .b = 0x0074u},
     });

     for (const auto& [c, b] : superscript)
          if (codePoint == c)
          {
               buffer[0] = b;
               return {buffer.data(), 1};
          }
     struct LigatureSubstitution
     {
          std::uint32_t c;
          std::array<std::uint32_t, 3> decomposed;
          std::uint8_t n;
     };
     static constexpr auto ligatureSubstitution = std::to_array<LigatureSubstitution>({
         {.c = 0x2100u, .decomposed = {0x0061u, 0x002Fu, 0x0063u}, .n = 3},
         {.c = 0x2101u, .decomposed = {0x0061u, 0x002Fu, 0x0073u}, .n = 3},
         {.c = 0x2102u, .decomposed = {0x0043u, 0, 0}, .n = 1},
         {.c = 0x2103u, .decomposed = {0x00B0u, 0x0043u, 0}, .n = 2},
         {.c = 0x2105u, .decomposed = {0x0063u, 0x002Fu, 0x006Fu}, .n = 3},
         {.c = 0x2106u, .decomposed = {0x0063u, 0x002Fu, 0x0075u}, .n = 3},
         {.c = 0x2107u, .decomposed = {0x0190u, 0, 0}, .n = 1},
         {.c = 0x2109u, .decomposed = {0x00B0u, 0x0046u, 0}, .n = 2},
         {.c = 0x210Au, .decomposed = {0x0067u, 0, 0}, .n = 1},
         {.c = 0x210Bu, .decomposed = {0x0048u, 0, 0}, .n = 1},
         {.c = 0x210Cu, .decomposed = {0x0048u, 0, 0}, .n = 1},
         {.c = 0x210Du, .decomposed = {0x0048u, 0, 0}, .n = 1},
         {.c = 0x210Eu, .decomposed = {0x0068u, 0, 0}, .n = 1},
         {.c = 0x210Fu, .decomposed = {0x0127u, 0, 0}, .n = 1},
         {.c = 0x2110u, .decomposed = {0x0049u, 0, 0}, .n = 1},
         {.c = 0x2111u, .decomposed = {0x0049u, 0, 0}, .n = 1},
         {.c = 0x2112u, .decomposed = {0x004Cu, 0, 0}, .n = 1},
         {.c = 0x2113u, .decomposed = {0x006Cu, 0, 0}, .n = 1},
         {.c = 0x2115u, .decomposed = {0x004Eu, 0, 0}, .n = 1},
         {.c = 0x2116u, .decomposed = {0x004Eu, 0x006Fu, 0}, .n = 2},
         {.c = 0x2119u, .decomposed = {0x0050u, 0, 0}, .n = 1},
         {.c = 0x211Au, .decomposed = {0x0051u, 0, 0}, .n = 1},
         {.c = 0x211Bu, .decomposed = {0x0052u, 0, 0}, .n = 1},
         {.c = 0x211Cu, .decomposed = {0x0052u, 0, 0}, .n = 1},
         {.c = 0x211Du, .decomposed = {0x0052u, 0, 0}, .n = 1},
         {.c = 0x2120u, .decomposed = {0x0053u, 0x004Du, 0}, .n = 2},
         {.c = 0x2122u, .decomposed = {0x0054u, 0x004Du, 0}, .n = 2},
         {.c = 0x2124u, .decomposed = {0x005Au, 0, 0}, .n = 1},
         {.c = 0x2126u, .decomposed = {0x03A9u, 0, 0}, .n = 1},
         {.c = 0x212Au, .decomposed = {0x004Bu, 0, 0}, .n = 1},
         {.c = 0x212Bu, .decomposed = {0x00C5u, 0, 0}, .n = 1},
         {.c = 0x212Cu, .decomposed = {0x0042u, 0, 0}, .n = 1},
         {.c = 0x212Du, .decomposed = {0x0043u, 0, 0}, .n = 1},
         {.c = 0x212Fu, .decomposed = {0x0065u, 0, 0}, .n = 1},
         {.c = 0x2130u, .decomposed = {0x0045u, 0, 0}, .n = 1},
         {.c = 0x2131u, .decomposed = {0x0046u, 0, 0}, .n = 1},
         {.c = 0x2133u, .decomposed = {0x004Du, 0, 0}, .n = 1},
         {.c = 0x2134u, .decomposed = {0x006Fu, 0, 0}, .n = 1},
         {.c = 0x2139u, .decomposed = {0x0069u, 0, 0}, .n = 1},
     });

     for (const auto& entry : ligatureSubstitution)
          if (codePoint == entry.c)
          {
               for (std::uint8_t i = 0; i < entry.n; i++) buffer[i] = entry.decomposed[i];
               return {buffer.data(), entry.n};
          }

     struct ReorderableSequence
     {
          std::uint32_t codePoint;
          std::array<std::uint32_t, 4> decomposed;
          std::uint8_t n;
     };
     static constexpr auto reorderableSequence = std::to_array<ReorderableSequence>({
         {.codePoint = 0x2160u, .decomposed = {0x0049u, 0, 0, 0}, .n = 1},
         {.codePoint = 0x2161u, .decomposed = {0x0049u, 0x0049u, 0, 0}, .n = 2},
         {.codePoint = 0x2162u, .decomposed = {0x0049u, 0x0049u, 0x0049u, 0}, .n = 3},
         {.codePoint = 0x2163u, .decomposed = {0x0049u, 0x0056u, 0, 0}, .n = 2},
         {.codePoint = 0x2164u, .decomposed = {0x0056u, 0, 0, 0}, .n = 1},
         {.codePoint = 0x2165u, .decomposed = {0x0056u, 0x0049u, 0, 0}, .n = 2},
         {.codePoint = 0x2166u, .decomposed = {0x0056u, 0x0049u, 0x0049u, 0}, .n = 3},
         {.codePoint = 0x2167u, .decomposed = {0x0056u, 0x0049u, 0x0049u, 0x0049u}, .n = 4},
         {.codePoint = 0x2168u, .decomposed = {0x0049u, 0x0058u, 0, 0}, .n = 2},
         {.codePoint = 0x2169u, .decomposed = {0x0058u, 0, 0, 0}, .n = 1},
         {.codePoint = 0x216Au, .decomposed = {0x0058u, 0x0049u, 0, 0}, .n = 2},
         {.codePoint = 0x216Bu, .decomposed = {0x0058u, 0x0049u, 0x0049u, 0}, .n = 3},
         {.codePoint = 0x216Cu, .decomposed = {0x004Cu, 0, 0, 0}, .n = 1},
         {.codePoint = 0x216Du, .decomposed = {0x0043u, 0, 0, 0}, .n = 1},
         {.codePoint = 0x216Eu, .decomposed = {0x0044u, 0, 0, 0}, .n = 1},
         {.codePoint = 0x216Fu, .decomposed = {0x004Du, 0, 0, 0}, .n = 1},
         {.codePoint = 0x2170u, .decomposed = {0x0069u, 0, 0, 0}, .n = 1},
         {.codePoint = 0x2171u, .decomposed = {0x0069u, 0x0069u, 0, 0}, .n = 2},
         {.codePoint = 0x2172u, .decomposed = {0x0069u, 0x0069u, 0x0069u, 0}, .n = 3},
         {.codePoint = 0x2173u, .decomposed = {0x0069u, 0x0076u, 0, 0}, .n = 2},
         {.codePoint = 0x2174u, .decomposed = {0x0076u, 0, 0, 0}, .n = 1},
         {.codePoint = 0x2175u, .decomposed = {0x0076u, 0x0069u, 0, 0}, .n = 2},
         {.codePoint = 0x2176u, .decomposed = {0x0076u, 0x0069u, 0x0069u, 0}, .n = 3},
         {.codePoint = 0x2177u, .decomposed = {0x0076u, 0x0069u, 0x0069u, 0x0069u}, .n = 4},
         {.codePoint = 0x2178u, .decomposed = {0x0069u, 0x0078u, 0, 0}, .n = 2},
         {.codePoint = 0x2179u, .decomposed = {0x0078u, 0, 0, 0}, .n = 1},
         {.codePoint = 0x217Au, .decomposed = {0x0078u, 0x0069u, 0, 0}, .n = 2},
         {.codePoint = 0x217Bu, .decomposed = {0x0078u, 0x0069u, 0x0069u, 0}, .n = 3},
         {.codePoint = 0x217Cu, .decomposed = {0x006Cu, 0, 0, 0}, .n = 1},
         {.codePoint = 0x217Du, .decomposed = {0x0063u, 0, 0, 0}, .n = 1},
         {.codePoint = 0x217Eu, .decomposed = {0x0064u, 0, 0, 0}, .n = 1},
         {.codePoint = 0x217Fu, .decomposed = {0x006Du, 0, 0, 0}, .n = 1},
     });

     for (const auto& entry : reorderableSequence)
          if (codePoint == entry.codePoint)
          {
               for (std::uint8_t i = 0; i < entry.n; i++) buffer[i] = entry.decomposed[i];
               return {buffer.data(), entry.n};
          }

     if (codePoint >= 0x2460u && codePoint <= 0x2468u)
     {
          buffer[0] = 0x0031u + (codePoint - 0x2460u);
          return {buffer.data(), 1};
     }
     if (codePoint >= 0x2469u && codePoint <= 0x2473u)
     {
          const auto value = codePoint - 0x2460u + 1u;
          buffer[0] = 0x0030u + (value / 10u);
          buffer[1] = 0x0030u + (value % 10u);
          return {buffer.data(), 2};
     }
     if (codePoint >= 0x2474u && codePoint <= 0x247Cu)
     {
          buffer[0] = 0x0031u + (codePoint - 0x2474u);
          return {buffer.data(), 1};
     }
     if (codePoint >= 0x247Du && codePoint <= 0x2487u)
     {
          const auto value = codePoint - 0x2474u + 1u;
          buffer[0] = 0x0030u + (value / 10u);
          buffer[1] = 0x0030u + (value % 10u);
          return {buffer.data(), 2};
     }
     if (codePoint >= 0x24B6u && codePoint <= 0x24CFu)
     {
          buffer[0] = 0x0041u + (codePoint - 0x24B6u);
          return {buffer.data(), 1};
     }
     if (codePoint >= 0x24D0u && codePoint <= 0x24E9u)
     {
          buffer[0] = 0x0061u + (codePoint - 0x24D0u);
          return {buffer.data(), 1};
     }

     struct MathBlock
     {
          std::uint32_t low;
          std::uint32_t high;
          std::uint32_t base;
     };
     static constexpr auto mathBlock = std::to_array<MathBlock>({
         {.low = 0x1D400u, .high = 0x1D419u, .base = 0x0041u}, {.low = 0x1D41Au, .high = 0x1D433u, .base = 0x0061u},
         {.low = 0x1D434u, .high = 0x1D44Du, .base = 0x0041u}, {.low = 0x1D44Eu, .high = 0x1D467u, .base = 0x0061u},
         {.low = 0x1D468u, .high = 0x1D481u, .base = 0x0041u}, {.low = 0x1D482u, .high = 0x1D49Bu, .base = 0x0061u},
         {.low = 0x1D49Cu, .high = 0x1D4B5u, .base = 0x0041u}, {.low = 0x1D4B6u, .high = 0x1D4CFu, .base = 0x0061u},
         {.low = 0x1D4D0u, .high = 0x1D4E9u, .base = 0x0041u}, {.low = 0x1D4EAu, .high = 0x1D503u, .base = 0x0061u},
         {.low = 0x1D504u, .high = 0x1D51Du, .base = 0x0041u}, {.low = 0x1D51Eu, .high = 0x1D537u, .base = 0x0061u},
         {.low = 0x1D538u, .high = 0x1D551u, .base = 0x0041u}, {.low = 0x1D552u, .high = 0x1D56Bu, .base = 0x0061u},
         {.low = 0x1D56Cu, .high = 0x1D585u, .base = 0x0041u}, {.low = 0x1D586u, .high = 0x1D59Fu, .base = 0x0061u},
         {.low = 0x1D5A0u, .high = 0x1D5B9u, .base = 0x0041u}, {.low = 0x1D5BAu, .high = 0x1D5D3u, .base = 0x0061u},
         {.low = 0x1D5D4u, .high = 0x1D5EDu, .base = 0x0041u}, {.low = 0x1D5EEu, .high = 0x1D607u, .base = 0x0061u},
         {.low = 0x1D608u, .high = 0x1D621u, .base = 0x0041u}, {.low = 0x1D622u, .high = 0x1D63Bu, .base = 0x0061u},
         {.low = 0x1D63Cu, .high = 0x1D655u, .base = 0x0041u}, {.low = 0x1D656u, .high = 0x1D66Fu, .base = 0x0061u},
         {.low = 0x1D670u, .high = 0x1D689u, .base = 0x0041u}, {.low = 0x1D68Au, .high = 0x1D6A3u, .base = 0x0061u},
         {.low = 0x1D7CEu, .high = 0x1D7D7u, .base = 0x0030u}, {.low = 0x1D7D8u, .high = 0x1D7E1u, .base = 0x0030u},
         {.low = 0x1D7E2u, .high = 0x1D7EBu, .base = 0x0030u}, {.low = 0x1D7ECu, .high = 0x1D7F5u, .base = 0x0030u},
         {.low = 0x1D7F6u, .high = 0x1D7FFu, .base = 0x0030u},
     });
     for (const auto& [low, high, base] : mathBlock)
          if (codePoint >= low && codePoint <= high)
          {
               buffer[0] = base + (codePoint - low);
               return {buffer.data(), 1};
          }

     struct Hole
     {
          std::uint32_t c;
          std::uint32_t b;
     };
     static constexpr auto holes = std::to_array<Hole>({
         {.c = 0x1D455u, .b = 0x0068u}, {.c = 0x1D49Du, .b = 0x0042u}, {.c = 0x1D4A0u, .b = 0x0045u},
         {.c = 0x1D4A1u, .b = 0x0046u}, {.c = 0x1D4A3u, .b = 0x0048u}, {.c = 0x1D4A4u, .b = 0x0049u},
         {.c = 0x1D4A7u, .b = 0x004Cu}, {.c = 0x1D4A8u, .b = 0x004Du}, {.c = 0x1D4ADu, .b = 0x0052u},
         {.c = 0x1D4BAu, .b = 0x0065u}, {.c = 0x1D4BCu, .b = 0x0067u}, {.c = 0x1D4C4u, .b = 0x006Fu},
         {.c = 0x1D506u, .b = 0x0043u}, {.c = 0x1D50Bu, .b = 0x0048u}, {.c = 0x1D50Cu, .b = 0x0049u},
         {.c = 0x1D515u, .b = 0x0052u}, {.c = 0x1D51Du, .b = 0x005Au}, {.c = 0x1D53Au, .b = 0x0043u},
         {.c = 0x1D53Fu, .b = 0x0048u}, {.c = 0x1D545u, .b = 0x004Eu}, {.c = 0x1D547u, .b = 0x0050u},
         {.c = 0x1D548u, .b = 0x0051u}, {.c = 0x1D549u, .b = 0x0052u}, {.c = 0x1D551u, .b = 0x005Au},
     });

     for (const auto& [c, b] : holes)
          if (codePoint == c)
          {
               buffer[0] = b;
               return {buffer.data(), 1};
          }

     return {};
}

struct Pair
{
     std::uint32_t upper;
     std::uint32_t lower;
};

struct Range
{
     std::uint32_t first;
     std::uint32_t last;
     std::uint32_t delta;
};

static constexpr std::array specials{
#include "lowerUpper.inc"
};

static constexpr std::array ranges{
    Range{.first = 0x0041u, .last = 0x005Au, .delta = 0x20u},
};

std::uint32_t UnicodeDatabase::SimpleUppercase(std::uint32_t codePoint) noexcept
{
     for (const auto& [upper, lower] : specials)
          if (codePoint == lower) return upper;
     for (const auto& [first, last, delta] : ranges)
          if (codePoint >= first + delta && codePoint <= last + delta) return codePoint - delta;
     return codePoint;
}

std::uint32_t UnicodeDatabase::SimpleLowercase(std::uint32_t codePoint) noexcept
{
     for (const auto& [upper, lower] : specials)
          if (codePoint == upper) return lower;
     for (const auto& [first, last, delta] : ranges)
          if (codePoint >= first && codePoint <= last) return codePoint + delta;
     return codePoint;
}
BidiClass UnicodeDatabase::GetBidiClass(std::uint32_t codePoint) noexcept
{
     if (codePoint == 0x200Eu) return BidiClass::L;   // LEFT-TO-RIGHT MARK
     if (codePoint == 0x200Fu) return BidiClass::R;   // RIGHT-TO-LEFT MARK
     if (codePoint == 0x061Cu) return BidiClass::AL;  // ARABIC LETTER MARK
     if (codePoint == 0x202Au) return BidiClass::LRE; // LEFT-TO-RIGHT EMBEDDING
     if (codePoint == 0x202Bu) return BidiClass::RLE; // RIGHT-TO-LEFT EMBEDDING
     if (codePoint == 0x202Cu) return BidiClass::PDF; // POP DIRECTIONAL FORMATTING
     if (codePoint == 0x202Du) return BidiClass::LRO; // LEFT-TO-RIGHT OVERRIDE
     if (codePoint == 0x202Eu) return BidiClass::RLO; // RIGHT-TO-LEFT OVERRIDE
     if (codePoint == 0x2066u) return BidiClass::LRI; // LEFT-TO-RIGHT ISOLATE
     if (codePoint == 0x2067u) return BidiClass::RLI; // RIGHT-TO-LEFT ISOLATE
     if (codePoint == 0x2068u) return BidiClass::FSI; // FIRST STRONG ISOLATE
     if (codePoint == 0x2069u) return BidiClass::PDI; // POP DIRECTIONAL ISOLATE

     if (codePoint >= 0x0041u && codePoint <= 0x005Au) return BidiClass::L; // A-Z
     if (codePoint >= 0x0061u && codePoint <= 0x007Au) return BidiClass::L; // a-z

     if (codePoint >= 0x0030u && codePoint <= 0x0039u) return BidiClass::EN; // 0-9

     if (codePoint == 0x0009u || codePoint == 0x000Bu || codePoint == 0x000Cu) return BidiClass::S; // TAB, VT, FF
     if (codePoint == 0x000Au || codePoint == 0x000Du || codePoint == 0x001Cu || codePoint == 0x001Du ||
         codePoint == 0x001Eu || codePoint == 0x2029u)
          return BidiClass::B; // LF, CR, FS, GS, RS, PSEP

     if (codePoint == 0x0020u ||                           // SPACE
         codePoint == 0x00A0u ||                           // NO-BREAK SPACE
         codePoint == 0x1680u ||                           // OGHAM SPACE MARK
         (codePoint >= 0x2000u && codePoint <= 0x200Au) || // Various spaces
         codePoint == 0x202Fu ||                           // NARROW NO-BREAK SPACE
         codePoint == 0x205Fu ||                           // MEDIUM MATHEMATICAL SPACE
         codePoint == 0x3000u)                             // IDEOGRAPHIC SPACE
          return BidiClass::WS;

     if ((codePoint >= 0x0600u && codePoint <= 0x07BFu) || // Arabic
         (codePoint >= 0x0800u && codePoint <= 0x08FFu) || // Samaritan, Mandaic, Syriac Supp
         (codePoint >= 0xFB50u && codePoint <= 0xFDFFu) || // Arabic Presentation Forms-A
         (codePoint >= 0xFE70u && codePoint <= 0xFEFFu))
     {

          if ((codePoint >= 0x0660u && codePoint <= 0x0669u) || (codePoint >= 0x06F0u && codePoint <= 0x06F9u))
               return BidiClass::AN;

          // Arabic Marks
          if ((codePoint >= 0x0610u && codePoint <= 0x061Au) || (codePoint >= 0x064Bu && codePoint <= 0x065Fu))
               return BidiClass::NSM;

          return BidiClass::AL;
     }

     if (codePoint >= 0x05D0u && codePoint <= 0x05EAu) return BidiClass::R;   // Letters
     if (codePoint >= 0x05F0u && codePoint <= 0x05F4u) return BidiClass::R;   // Ligatures
     if (codePoint >= 0x0590u && codePoint <= 0x05CFu) return BidiClass::NSM; // Marks

     if ((codePoint >= 0x0700u && codePoint <= 0x074Fu) || (codePoint >= 0x0750u && codePoint <= 0x077Fu))
          return BidiClass::AL;

     if ((codePoint >= 0x0300u && codePoint <= 0x036Fu) || (codePoint >= 0x1AB0u && codePoint <= 0x1AFFu) ||
         (codePoint >= 0x1DC0u && codePoint <= 0x1DFFu) || (codePoint >= 0x20D0u && codePoint <= 0x20FFu))
          return BidiClass::NSM;

     if (codePoint == 0x200Du || codePoint == 0x200Cu) return BidiClass::BN;

     return BidiClass::L;
}
bool UnicodeDatabase::IsWhitespace(std::uint32_t codePoint) noexcept
{
     return codePoint == 0x0009u || codePoint == 0x000Au || codePoint == 0x000Du || codePoint == 0x0020u ||
            codePoint == 0x00A0u || codePoint == 0x200Bu || codePoint == 0x3000u || codePoint == 0xFEFFu;
}
