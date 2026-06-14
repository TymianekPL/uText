#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <span>

namespace text::unicode
{
     enum struct Locale : std::uint8_t
     {
          Root,

          Afrikaans,
          Albanian,
          Arabic,
          Armenian,
          Azerbaijani,
          Basque,
          Belarusian,
          Bengali,
          Bosnian,
          Bulgarian,
          Catalan,
          Chinese,
          Croatian,
          Czech,
          Danish,
          Dutch,
          English,
          Estonian,
          Finnish,
          French,
          Galician,
          Georgian,
          German,
          Greek,
          Gujarati,
          Hebrew,
          Hindi,
          Hungarian,
          Icelandic,
          Indonesian,
          Irish,
          Italian,
          Japanese,
          Kannada,
          Kazakh,
          Khmer,
          Korean,
          Kyrgyz,
          Lao,
          Latvian,
          Lithuanian,
          Macedonian,
          Malay,
          Malayalam,
          Maltese,
          Marathi,
          Mongolian,
          Nepali,
          Norwegian,
          Odia,
          Persian,
          Polish,
          Portuguese,
          Punjabi,
          Romanian,
          Russian,
          Serbian,
          Sinhala,
          Slovak,
          Slovenian,
          Somali,
          Spanish,
          Swahili,
          Swedish,
          Tagalog,
          Tajik,
          Tamil,
          Telugu,
          Thai,
          Tibetan,
          Turkish,
          Turkmen,
          Ukrainian,
          Urdu,
          Uzbek,
          Vietnamese,
          Welsh,
          Yoruba,
          Zulu,

          Custom
     };

     enum struct GraphemeClusterBoundaryProperty : std::uint8_t
     {
          Other,
          CarriageReturn,
          LineFeed,
          Control,
          Extend,
          ZeroWidthJoiner,
          SpacingMark,
          Prepend,
          RegionalIndicator,
          HangulL,
          HangulV,
          HangulT,
          HangulLV,
          HangulLVT,
          ExtPic,
          EmojiModifier,
          InCBLinker,
          InCBConsonant,
          InCBExtend,
     };

     enum struct BidiClass : std::uint8_t
     {
          L,
          R,
          AL,
          EN,
          AN,
          WS,
          ON,
          NSM,
          BN,
          B,
          S,
          ET,
          ES,
          CS,
          LRE,
          LRO,
          RLE,
          RLO,
          PDF,
          LRI,
          RLI,
          FSI,
          PDI
     };

     enum struct Direction : std::uint8_t
     {
          LTR,
          RTL
     };
     enum struct WordBreakProperty : std::uint8_t
     {
          Other,
          CR,
          LF,
          Newline,
          Extend,
          ZWJ,
          WSegSpace,
          Format,
          Katakana,
          HebrewLetter,
          ALetterUpper,
          ALetterLower,
          SingleQuote,
          DoubleQuote,
          MidNumLet,
          MidLetter,
          MidNum,
          Numeric,
          ExtendNumLet,
          RegionalIndicator,
     };

     enum struct SentenceBreakProperty : std::uint8_t
     {
          Other,
          CR,
          LF,
          Extend,
          Sep,
          Format,
          Sp,
          Lower,
          Upper,
          OLetter,
          Numeric,
          ATerm,
          STerm,
          Close,
          SContinue,
     };

     enum struct LineBreakClass : std::uint8_t
     {
          BK,
          CR,
          LF,
          CM,
          NL,
          SG,
          WJ,
          ZW,
          GL,
          SP,
          ZWJ,
          B2,
          BA,
          BB,
          HY,
          CB,
          CL,
          CP,
          EX,
          IN,
          NS,
          OP,
          QU,
          IS,
          NU,
          PO,
          PR,
          SY,
          AI,
          AL,
          CJ,
          EB,
          EM,
          H2,
          H3,
          HL,
          ID,
          JL,
          JT,
          JV,
          RI,
          SA,
          XX,
     };

     enum struct NumericType : std::uint8_t
     {
          None,
          Decimal,
          Digit,
          Numeric
     };

     struct NumericValue
     {
          NumericType type = NumericType::None;
          std::int32_t numerator = 0;
          std::int32_t denominator = 1;
     };

     enum struct Script : std::uint8_t
     {
          Unknown,
          Common,
          Inherited,
          Latin,
          Greek,
          Cyrillic,
          Arabic,
          Hebrew,
          Devanagari,
          Bengali,
          Gurmukhi,
          Gujarati,
          Oriya,
          Tamil,
          Telugu,
          Kannada,
          Malayalam,
          Sinhala,
          Thai,
          Lao,
          Tibetan,
          Myanmar,
          Georgian,
          Hangul,
          Ethiopic,
          Cherokee,
          Canadian_Aboriginal,
          Ogham,
          Runic,
          Khmer,
          Mongolian,
          Hiragana,
          Katakana,
          Bopomofo,
          Han,
          Yi,
          OldItalic,
          Gothic,
          Deseret,
          Tagalog,
          Hanunoo,
          Buhid,
          Tagbanwa,
          Limbu,
          TaiLe,
          LinearB,
          Ugaritic,
          Shavian,
          Osmanya,
          Cypriot,
          Braille,
          Buginese,
          Coptic,
          NewTaiLue,
          Glagolitic,
          Tifinagh,
          SylotiNagri,
          OldPersian,
          Kharoshthi,
          Balinese,
          Cuneiform,
          Phoenician,
          PhagsPa,
          Nkoo,
          Sundanese,
          Lepcha,
          OlChiki,
          Vai,
          Saurashtra,
          KayahLi,
          Rejang,
          Lycian,
          Carian,
          Lydian,
          Cham,
          TaiTham,
          TaiViet,
          Avestan,
          EgyptianHieroglyphs,
          Samaritan,
          Lisu,
          Bamum,
          Javanese,
          MeeteiMayek,
          ImperialAramaic,
          OldSouthArabian,
          InscriptionalParthian,
          InscriptionalPahlavi,
          OldTurkic,
          Kaithi,
          Batak,
          Brahmi,
          Mandaic,
          Chakma,
          MiaoShu,
          Sharada,
          SoraSompeng,
          Takri,
          Caucasian_Albanian,
          DuployanShorthand,
          Elbasan,
          Grantha,
          Khojki,
          Khudawadi,
          LinearA,
          Mahajani,
          Manichaean,
          MendeKikakui,
          Modi,
          Mro,
          Nabataean,
          OldNorthArabian,
          OldPermic,
          PahawhHmong,
          Palmyrene,
          PauCinHau,
          PsalterPahlavi,
          Siddham,
          Tirhuta,
          WarangCiti,
          Ahom,
          HatranAramaic,
          Multani,
          OldHungarian,
          Adlam,
          Bhaiksuki,
          Marchen,
          Newa,
          Osage,
          Zanabazar,
          Masaram_Gondi,
          Nushu,
          Sogdian,
          OldSogdian,
          Elymaic,
          HanifiRohingya,
          Makasar,
          Nandinagari,
          NyiakengPuachueHmong,
          Wancho,
          Chorasmian,
          DivesAkuru,
          KhitanSmallScript,
          Yezidi,
          CyproMinoan,
          OldUyghur,
          Tangsa,
          Toto,
          Vithkuqi,
          Kawi,
          NagMundari,
          Garay,
          GurungKhema,
          KiratRai,
          OlOnal,
          Sunuwar,
          Todhri,
          TuluTigalari,
     };

     enum struct GeneralCategory : std::uint8_t
     {
          Lu,
          Ll,
          Lt,
          Lm,
          Lo,
          Mn,
          Mc,
          Me,
          Nd,
          Nl,
          No,
          Pc,
          Pd,
          Ps,
          Pe,
          Pi,
          Pf,
          Po,
          Sm,
          Sc,
          Sk,
          So,
          Zs,
          Zl,
          Zp,
          Cc,
          Cf,
          Cs,
          Co,
          Cn,
     };

     enum struct CaseFoldMode : std::uint8_t
     {
          Full,
          Simple,
          Turkic,
     };

     enum struct NormalizationForm : std::uint8_t
     {
          NFC,
          NFD,
          NFKC,
          NFKD
     };

     template <Locale L> struct LocaleTraits
     {
          static constexpr bool HasTurkishDottedI = false;
          static constexpr bool FinalSigmaLowering = false;
          static constexpr bool HasLithuanianAccents = false;
          static constexpr bool UsesArabicIndicDigits = false;
          static constexpr bool UsesExtendedArabicIndicDigits = false;
          static constexpr Direction DefaultDirection = Direction::LTR;
          static constexpr Script PrimaryScript = Script::Unknown;
     };

     template <> struct LocaleTraits<Locale::Root>
     {
          static constexpr bool HasTurkishDottedI = false;
          static constexpr bool FinalSigmaLowering = true;
          static constexpr bool HasLithuanianAccents = false;
          static constexpr bool UsesArabicIndicDigits = false;
          static constexpr bool UsesExtendedArabicIndicDigits = false;
          static constexpr Direction DefaultDirection = Direction::LTR;
          static constexpr Script PrimaryScript = Script::Common;
     };

     template <> struct LocaleTraits<Locale::Turkish>
     {
          static constexpr bool HasTurkishDottedI = true;
          static constexpr bool FinalSigmaLowering = false;
          static constexpr bool HasLithuanianAccents = false;
          static constexpr bool UsesArabicIndicDigits = false;
          static constexpr bool UsesExtendedArabicIndicDigits = false;
          static constexpr Direction DefaultDirection = Direction::LTR;
          static constexpr Script PrimaryScript = Script::Latin;
     };

     template <> struct LocaleTraits<Locale::Azerbaijani>
     {
          static constexpr bool HasTurkishDottedI = true;
          static constexpr bool FinalSigmaLowering = false;
          static constexpr bool HasLithuanianAccents = false;
          static constexpr bool UsesArabicIndicDigits = false;
          static constexpr bool UsesExtendedArabicIndicDigits = false;
          static constexpr Direction DefaultDirection = Direction::LTR;
          static constexpr Script PrimaryScript = Script::Latin;
     };

     template <> struct LocaleTraits<Locale::Lithuanian>
     {
          static constexpr bool HasTurkishDottedI = false;
          static constexpr bool FinalSigmaLowering = false;
          static constexpr bool HasLithuanianAccents = true;
          static constexpr bool UsesArabicIndicDigits = false;
          static constexpr bool UsesExtendedArabicIndicDigits = false;
          static constexpr Direction DefaultDirection = Direction::LTR;
          static constexpr Script PrimaryScript = Script::Latin;
     };

     template <> struct LocaleTraits<Locale::Greek>
     {
          static constexpr bool HasTurkishDottedI = false;
          static constexpr bool FinalSigmaLowering = true;
          static constexpr bool HasLithuanianAccents = false;
          static constexpr bool UsesArabicIndicDigits = false;
          static constexpr bool UsesExtendedArabicIndicDigits = false;
          static constexpr Direction DefaultDirection = Direction::LTR;
          static constexpr Script PrimaryScript = Script::Greek;
     };

     template <> struct LocaleTraits<Locale::Arabic>
     {
          static constexpr bool HasTurkishDottedI = false;
          static constexpr bool FinalSigmaLowering = false;
          static constexpr bool HasLithuanianAccents = false;
          static constexpr bool UsesArabicIndicDigits = true;
          static constexpr bool UsesExtendedArabicIndicDigits = false;
          static constexpr Direction DefaultDirection = Direction::RTL;
          static constexpr Script PrimaryScript = Script::Arabic;
     };

     template <> struct LocaleTraits<Locale::Persian>
     {
          static constexpr bool HasTurkishDottedI = false;
          static constexpr bool FinalSigmaLowering = false;
          static constexpr bool HasLithuanianAccents = false;
          static constexpr bool UsesArabicIndicDigits = false;
          static constexpr bool UsesExtendedArabicIndicDigits = true;
          static constexpr Direction DefaultDirection = Direction::RTL;
          static constexpr Script PrimaryScript = Script::Arabic;
     };

     template <> struct LocaleTraits<Locale::Urdu>
     {
          static constexpr bool HasTurkishDottedI = false;
          static constexpr bool FinalSigmaLowering = false;
          static constexpr bool HasLithuanianAccents = false;
          static constexpr bool UsesArabicIndicDigits = false;
          static constexpr bool UsesExtendedArabicIndicDigits = true;
          static constexpr Direction DefaultDirection = Direction::RTL;
          static constexpr Script PrimaryScript = Script::Arabic;
     };

     template <> struct LocaleTraits<Locale::Hebrew>
     {
          static constexpr bool HasTurkishDottedI = false;
          static constexpr bool FinalSigmaLowering = false;
          static constexpr bool HasLithuanianAccents = false;
          static constexpr bool UsesArabicIndicDigits = false;
          static constexpr bool UsesExtendedArabicIndicDigits = false;
          static constexpr Direction DefaultDirection = Direction::RTL;
          static constexpr Script PrimaryScript = Script::Hebrew;
     };

     template <> struct LocaleTraits<Locale::Japanese>
     {
          static constexpr bool HasTurkishDottedI = false;
          static constexpr bool FinalSigmaLowering = false;
          static constexpr bool HasLithuanianAccents = false;
          static constexpr bool UsesArabicIndicDigits = false;
          static constexpr bool UsesExtendedArabicIndicDigits = false;
          static constexpr Direction DefaultDirection = Direction::LTR;
          static constexpr Script PrimaryScript = Script::Hiragana;
     };

     template <> struct LocaleTraits<Locale::Chinese>
     {
          static constexpr bool HasTurkishDottedI = false;
          static constexpr bool FinalSigmaLowering = false;
          static constexpr bool HasLithuanianAccents = false;
          static constexpr bool UsesArabicIndicDigits = false;
          static constexpr bool UsesExtendedArabicIndicDigits = false;
          static constexpr Direction DefaultDirection = Direction::LTR;
          static constexpr Script PrimaryScript = Script::Han;
     };

     template <> struct LocaleTraits<Locale::Korean>
     {
          static constexpr bool HasTurkishDottedI = false;
          static constexpr bool FinalSigmaLowering = false;
          static constexpr bool HasLithuanianAccents = false;
          static constexpr bool UsesArabicIndicDigits = false;
          static constexpr bool UsesExtendedArabicIndicDigits = false;
          static constexpr Direction DefaultDirection = Direction::LTR;
          static constexpr Script PrimaryScript = Script::Hangul;
     };

     template <> struct LocaleTraits<Locale::Thai>
     {
          static constexpr bool HasTurkishDottedI = false;
          static constexpr bool FinalSigmaLowering = false;
          static constexpr bool HasLithuanianAccents = false;
          static constexpr bool UsesArabicIndicDigits = false;
          static constexpr bool UsesExtendedArabicIndicDigits = false;
          static constexpr Direction DefaultDirection = Direction::LTR;
          static constexpr Script PrimaryScript = Script::Thai;
     };

     class UnicodeDatabase
     {
     public:
          [[nodiscard]] static GraphemeClusterBoundaryProperty GraphemeProperty(std::uint32_t codePoint) noexcept;

          [[nodiscard]] static WordBreakProperty WordBreak(std::uint32_t codePoint) noexcept;

          [[nodiscard]] static SentenceBreakProperty SentenceBreak(std::uint32_t codePoint) noexcept;

          [[nodiscard]] static LineBreakClass LineBreak(std::uint32_t codePoint) noexcept;

          [[nodiscard]] static BidiClass GetBidiClass(std::uint32_t codePoint) noexcept;

          [[nodiscard]] static std::uint8_t CombiningClass(std::uint32_t codePoint) noexcept;

          [[nodiscard]] static GeneralCategory Category(std::uint32_t codePoint) noexcept;

          [[nodiscard]] static Script ScriptOf(std::uint32_t codePoint) noexcept;

          [[nodiscard]] static NumericValue Numeric(std::uint32_t codePoint) noexcept;

          [[nodiscard]] static std::uint32_t SimpleUppercase(std::uint32_t codePoint) noexcept;

          [[nodiscard]] static std::uint32_t SimpleLowercase(std::uint32_t codePoint) noexcept;

          [[nodiscard]] static std::uint32_t SimpleTitlecase(std::uint32_t codePoint) noexcept;

          [[nodiscard]] static std::uint32_t SimpleCaseFold(std::uint32_t codePoint) noexcept;

          [[nodiscard]] static std::span<const std::uint32_t> FullUppercase(
              std::uint32_t codePoint, std::array<std::uint32_t, 3>& buffer) noexcept;

          [[nodiscard]] static std::span<const std::uint32_t> FullLowercase(
              std::uint32_t codePoint, std::array<std::uint32_t, 3>& buffer) noexcept;

          [[nodiscard]] static std::span<const std::uint32_t> FullTitlecase(
              std::uint32_t codePoint, std::array<std::uint32_t, 3>& buffer) noexcept;

          [[nodiscard]] static std::span<const std::uint32_t> FullCaseFold(
              std::uint32_t codePoint, std::array<std::uint32_t, 3>& buffer) noexcept;

          [[nodiscard]] static std::span<const std::uint32_t> CanonicalDecompose(
              std::uint32_t codePoint, std::array<std::uint32_t, 3>& buffer) noexcept;

          [[nodiscard]] static std::uint32_t CanonicalCompose(std::uint32_t starter, std::uint32_t combining) noexcept;

          [[nodiscard]] static std::span<const std::uint32_t> CompatibilityDecompose(
              std::uint32_t codePoint, std::array<std::uint32_t, 8>& buffer) noexcept;

          [[nodiscard]] static bool IsWhitespace(std::uint32_t codePoint) noexcept;

          [[nodiscard]] static bool IsEmoji(std::uint32_t codePoint) noexcept;

          [[nodiscard]] static bool IsAlphabetic(std::uint32_t codePoint) noexcept;

          [[nodiscard]] static bool IsUppercase(std::uint32_t codePoint) noexcept;

          [[nodiscard]] static bool IsLowercase(std::uint32_t codePoint) noexcept;

          [[nodiscard]] static bool IsCased(std::uint32_t codePoint) noexcept;

          [[nodiscard]] static bool IsIDStart(std::uint32_t codePoint) noexcept;

          [[nodiscard]] static bool IsIDContinue(std::uint32_t codePoint) noexcept;
     };

     [[nodiscard]] constexpr GraphemeClusterBoundaryProperty GraphemeClusterBoundaryPropertyOf(
         std::uint32_t codePoint) noexcept
     { return UnicodeDatabase::GraphemeProperty(codePoint); }

     [[nodiscard]] constexpr bool IsGraphemeBreak(GraphemeClusterBoundaryProperty left,
                                                  GraphemeClusterBoundaryProperty right, bool leftWasRI,
                                                  bool inZWJPicSeq) noexcept
     {
          using P = GraphemeClusterBoundaryProperty;

          if (left == P::CarriageReturn && right == P::LineFeed) return false;
          if (left == P::CarriageReturn || left == P::LineFeed || left == P::Control) return true;
          if (right == P::CarriageReturn || right == P::LineFeed || right == P::Control) return true;

          if (left == P::HangulL &&
              (right == P::HangulL || right == P::HangulV || right == P::HangulLV || right == P::HangulLVT))
               return false;
          if ((left == P::HangulLV || left == P::HangulV) && (right == P::HangulV || right == P::HangulT)) return false;
          if ((left == P::HangulLVT || left == P::HangulT) && right == P::HangulT) return false;

          if (right == P::Extend || right == P::ZeroWidthJoiner) return false;
          if (right == P::SpacingMark) return false;
          if (left == P::Prepend) return false;

          if (left == P::ZeroWidthJoiner && right == P::ExtPic && inZWJPicSeq) return false;
          if (left == P::RegionalIndicator && right == P::RegionalIndicator && leftWasRI) return false;
          if (right == P::EmojiModifier && left == P::ExtPic) return false;

          if (left == P::InCBLinker && right == P::InCBConsonant) return false;
          if (left == P::InCBExtend && right == P::InCBConsonant) return false;

          return true;
     }

     template <Locale L> struct WordBreakRules
     {
          [[nodiscard]] static constexpr bool IsWordBoundary(std::uint32_t left, std::uint32_t right) noexcept
          {
               const auto leftProperty = UnicodeDatabase::WordBreak(left);
               const auto rightProperty = UnicodeDatabase::WordBreak(right);

               using W = WordBreakProperty;
               if (leftProperty == W::CR && rightProperty == W::LF) return false;
               if (leftProperty == W::CR || leftProperty == W::LF || leftProperty == W::Newline) return true;
               if (rightProperty == W::CR || rightProperty == W::LF || rightProperty == W::Newline) return true;
               if (leftProperty == W::ZWJ) return false;
               if (leftProperty == W::RegionalIndicator && rightProperty == W::RegionalIndicator) return false;
               if (leftProperty == W::Katakana && rightProperty == W::Katakana) return false;
               if ((leftProperty == W::HebrewLetter || leftProperty == W::ALetterUpper ||
                    leftProperty == W::ALetterLower) &&
                   (rightProperty == W::HebrewLetter || rightProperty == W::ALetterUpper ||
                    rightProperty == W::ALetterLower))
                    return false;
               if ((leftProperty == W::Numeric || leftProperty == W::ALetterUpper || leftProperty == W::ALetterLower ||
                    leftProperty == W::HebrewLetter || leftProperty == W::ExtendNumLet) &&
                   rightProperty == W::ExtendNumLet)
                    return false;
               if (leftProperty == W::ExtendNumLet &&
                   (rightProperty == W::Numeric || rightProperty == W::ALetterUpper ||
                    rightProperty == W::ALetterLower || rightProperty == W::HebrewLetter ||
                    rightProperty == W::Katakana))
                    return false;
               if (leftProperty == W::Numeric && rightProperty == W::Numeric) return false;
               if (leftProperty == W::WSegSpace && rightProperty == W::WSegSpace) return false;
               return true;
          }
     };

     template <> struct WordBreakRules<Locale::Japanese>
     {
          [[nodiscard]] static constexpr bool IsWordBoundary(std::uint32_t /*left*/, std::uint32_t /*right*/) noexcept
          { return true; }
     };

     template <> struct WordBreakRules<Locale::Chinese>
     {
          [[nodiscard]] static constexpr bool IsWordBoundary(std::uint32_t /*left*/, std::uint32_t /*right*/) noexcept
          { return true; }
     };

     template <> struct WordBreakRules<Locale::Thai>
     {
          [[nodiscard]] static constexpr bool IsWordBoundary(std::uint32_t /*left*/, std::uint32_t /*right*/) noexcept
          { return true; }
     };

     template <Locale L> class Collator
     {
     public:
          [[nodiscard]] static constexpr int Compare(std::span<const std::uint32_t> lhs,
                                                     std::span<const std::uint32_t> rhs) noexcept;
     };

     template <Locale L> struct DigitTraits
     {
          static constexpr std::array<std::uint32_t, 10> Digits = {U'0', U'1', U'2', U'3', U'4',
                                                                   U'5', U'6', U'7', U'8', U'9'};

          [[nodiscard]] static constexpr std::optional<std::uint32_t> ToLocaleDigit(std::uint32_t ascii) noexcept
          {
               if (ascii >= U'0' && ascii <= U'9') return Digits[ascii - U'0'];
               return std::nullopt;
          }

          [[nodiscard]] static constexpr std::optional<int> FromLocaleDigit(std::uint32_t codePoint) noexcept
          {
               for (int i = 0; i < 10; ++i)
                    if (Digits[i] == codePoint) return i;
               return std::nullopt;
          }
     };

     template <> struct DigitTraits<Locale::Arabic>
     {
          static constexpr std::array<std::uint32_t, 10> Digits = {
              0x0660u, 0x0661u, 0x0662u, 0x0663u, 0x0664u, 0x0665u, 0x0666u, 0x0667u, 0x0668u, 0x0669u,
          };

          [[nodiscard]] static constexpr std::optional<std::uint32_t> ToLocaleDigit(std::uint32_t ascii) noexcept
          {
               if (ascii >= U'0' && ascii <= U'9') return Digits[ascii - U'0'];
               return std::nullopt;
          }

          [[nodiscard]] static constexpr std::optional<int> FromLocaleDigit(std::uint32_t codePoint) noexcept
          {
               if (codePoint >= 0x0660u && codePoint <= 0x0669u) return static_cast<int>(codePoint - 0x0660u);
               if (codePoint >= U'0' && codePoint <= U'9') return static_cast<int>(codePoint - U'0');
               return std::nullopt;
          }
     };

     template <> struct DigitTraits<Locale::Persian>
     {
          static constexpr std::array<std::uint32_t, 10> Digits = {
              0x06F0u, 0x06F1u, 0x06F2u, 0x06F3u, 0x06F4u, 0x06F5u, 0x06F6u, 0x06F7u, 0x06F8u, 0x06F9u,
          };

          [[nodiscard]] static constexpr std::optional<std::uint32_t> ToLocaleDigit(std::uint32_t ascii) noexcept
          {
               if (ascii >= U'0' && ascii <= U'9') return Digits[ascii - U'0'];
               return std::nullopt;
          }

          [[nodiscard]] static constexpr std::optional<int> FromLocaleDigit(std::uint32_t codePoint) noexcept
          {
               if (codePoint >= 0x06F0u && codePoint <= 0x06F9u) return static_cast<int>(codePoint - 0x06F0u);
               if (codePoint >= U'0' && codePoint <= U'9') return static_cast<int>(codePoint - U'0');
               return std::nullopt;
          }
     };

     template <> struct DigitTraits<Locale::Urdu> : DigitTraits<Locale::Persian>
     {
     };

     template <> struct DigitTraits<Locale::Bengali>
     {
          static constexpr std::array<std::uint32_t, 10> Digits = {
              0x09E6u, 0x09E7u, 0x09E8u, 0x09E9u, 0x09EAu, 0x09EBu, 0x09ECu, 0x09EDu, 0x09EEu, 0x09EFu,
          };

          [[nodiscard]] static constexpr std::optional<std::uint32_t> ToLocaleDigit(std::uint32_t ascii) noexcept
          {
               if (ascii >= U'0' && ascii <= U'9') return Digits[ascii - U'0'];
               return std::nullopt;
          }

          [[nodiscard]] static constexpr std::optional<int> FromLocaleDigit(std::uint32_t codePoint) noexcept
          {
               if (codePoint >= 0x09E6u && codePoint <= 0x09EFu) return static_cast<int>(codePoint - 0x09E6u);
               if (codePoint >= U'0' && codePoint <= U'9') return static_cast<int>(codePoint - U'0');
               return std::nullopt;
          }
     };

     template <> struct DigitTraits<Locale::Hindi>
     {
          static constexpr std::array<std::uint32_t, 10> Digits = {
              0x0966u, 0x0967u, 0x0968u, 0x0969u, 0x096Au, 0x096Bu, 0x096Cu, 0x096Du, 0x096Eu, 0x096Fu,
          };

          [[nodiscard]] static constexpr std::optional<std::uint32_t> ToLocaleDigit(std::uint32_t ascii) noexcept
          {
               if (ascii >= U'0' && ascii <= U'9') return Digits[ascii - U'0'];
               return std::nullopt;
          }

          [[nodiscard]] static constexpr std::optional<int> FromLocaleDigit(std::uint32_t codePoint) noexcept
          {
               if (codePoint >= 0x0966u && codePoint <= 0x096Fu) return static_cast<int>(codePoint - 0x0966u);
               if (codePoint >= U'0' && codePoint <= U'9') return static_cast<int>(codePoint - U'0');
               return std::nullopt;
          }
     };

     template <Locale L = Locale::Root> class Unicode
     {
     public:
          using Traits = LocaleTraits<L>;
          using Digits = DigitTraits<L>;

          [[nodiscard]] static constexpr Direction DetectParagraphDirection(
              std::span<const std::uint32_t> codePoints) noexcept
          {
               for (const auto codePoint : codePoints)
               {
                    const auto bidiClass = UnicodeDatabase::GetBidiClass(codePoint);
                    if (bidiClass == BidiClass::L) return Direction::LTR;
                    if (bidiClass == BidiClass::R || bidiClass == BidiClass::AL) return Direction::RTL;
               }
               return Traits::DefaultDirection;
          }

          [[nodiscard]] static constexpr std::uint32_t ToUpper(std::uint32_t codePoint) noexcept
          {
               if constexpr (Traits::HasTurkishDottedI)
               {
                    if (codePoint == U'i') return 0x0130u;
                    if (codePoint == 0x0131u) return U'I';
               }
               return UnicodeDatabase::SimpleUppercase(codePoint);
          }

          [[nodiscard]] static constexpr std::uint32_t ToLower(std::uint32_t codePoint) noexcept
          {
               if constexpr (Traits::HasTurkishDottedI)
               {
                    if (codePoint == U'I') return 0x0131u;
                    if (codePoint == 0x0130u) return U'i';
               }
               if constexpr (Traits::FinalSigmaLowering) {}
               return UnicodeDatabase::SimpleLowercase(codePoint);
          }

          [[nodiscard]] static constexpr std::uint32_t ToTitle(std::uint32_t codePoint) noexcept
          {
               if constexpr (Traits::HasTurkishDottedI)
               {
                    if (codePoint == U'i') return 0x0130u;
                    if (codePoint == 0x0131u) return U'I';
               }
               return UnicodeDatabase::SimpleTitlecase(codePoint);
          }

          [[nodiscard]] static constexpr std::span<const std::uint32_t> FullToUpper(
              std::uint32_t codePoint, std::array<std::uint32_t, 3>& buffer) noexcept
          {
               if constexpr (Traits::HasTurkishDottedI)
               {
                    if (codePoint == U'i')
                    {
                         buffer[0] = 0x0130u;
                         return {buffer.data(), 1};
                    }
                    if (codePoint == 0x0131u)
                    {
                         buffer[0] = U'I';
                         return {buffer.data(), 1};
                    }
               }
               return UnicodeDatabase::FullUppercase(codePoint, buffer);
          }

          [[nodiscard]] static constexpr std::span<const std::uint32_t> FullToLower(
              std::uint32_t codePoint, std::array<std::uint32_t, 3>& buffer) noexcept
          {
               if constexpr (Traits::HasTurkishDottedI)
               {
                    if (codePoint == U'I')
                    {
                         buffer[0] = 0x0131u;
                         return {buffer.data(), 1};
                    }
                    if (codePoint == 0x0130u)
                    {
                         buffer[0] = U'i';
                         return {buffer.data(), 1};
                    }
               }
               return UnicodeDatabase::FullLowercase(codePoint, buffer);
          }

          [[nodiscard]] static constexpr std::span<const std::uint32_t> FullToTitle(
              std::uint32_t codePoint, std::array<std::uint32_t, 3>& buffer) noexcept
          {
               if constexpr (Traits::HasTurkishDottedI)
               {
                    if (codePoint == U'i')
                    {
                         buffer[0] = 0x0130u;
                         return {buffer.data(), 1};
                    }
                    if (codePoint == 0x0131u)
                    {
                         buffer[0] = U'I';
                         return {buffer.data(), 1};
                    }
               }
               return UnicodeDatabase::FullTitlecase(codePoint, buffer);
          }

          [[nodiscard]] static constexpr std::uint32_t CaseFold(std::uint32_t codePoint) noexcept
          {
               if constexpr (Traits::HasTurkishDottedI)
               {
                    if (codePoint == U'I') return 0x0131u;
                    if (codePoint == 0x0130u) return U'i';
               }
               return UnicodeDatabase::SimpleCaseFold(codePoint);
          }

          [[nodiscard]] static constexpr std::span<const std::uint32_t> FullCaseFold(
              std::uint32_t codePoint, std::array<std::uint32_t, 3>& buffer) noexcept
          {
               if constexpr (Traits::HasTurkishDottedI)
               {
                    if (codePoint == U'I')
                    {
                         buffer[0] = 0x0131u;
                         return {buffer.data(), 1};
                    }
                    if (codePoint == 0x0130u)
                    {
                         buffer[0] = U'i';
                         return {buffer.data(), 1};
                    }
               }
               return UnicodeDatabase::FullCaseFold(codePoint, buffer);
          }

          [[nodiscard]] static constexpr bool EqualFolded(std::uint32_t lhs, std::uint32_t rhs) noexcept
          { return CaseFold(lhs) == CaseFold(rhs); }

          [[nodiscard]] static constexpr std::span<const std::uint32_t> CanonicalDecompose(
              std::uint32_t codePoint, std::array<std::uint32_t, 3>& buffer) noexcept
          { return UnicodeDatabase::CanonicalDecompose(codePoint, buffer); }

          [[nodiscard]] static constexpr std::uint32_t CanonicalCompose(std::uint32_t starter,
                                                                        std::uint32_t combining) noexcept
          { return UnicodeDatabase::CanonicalCompose(starter, combining); }

          [[nodiscard]] static constexpr std::span<const std::uint32_t> CompatibilityDecompose(
              std::uint32_t codePoint, std::array<std::uint32_t, 8>& buffer) noexcept
          { return UnicodeDatabase::CompatibilityDecompose(codePoint, buffer); }

          [[nodiscard]] static constexpr std::uint8_t CombiningClass(std::uint32_t codePoint) noexcept
          { return UnicodeDatabase::CombiningClass(codePoint); }

          [[nodiscard]] static constexpr bool IsGraphemeBreak(std::uint32_t left, std::uint32_t right, bool leftWasRI,
                                                              bool inZWJPicSeq) noexcept
          {
               return text::unicode::IsGraphemeBreak(UnicodeDatabase::GraphemeProperty(left),
                                                     UnicodeDatabase::GraphemeProperty(right), leftWasRI, inZWJPicSeq);
          }

          [[nodiscard]] static constexpr bool IsWordBoundary(std::uint32_t left, std::uint32_t right) noexcept
          { return WordBreakRules<L>::IsWordBoundary(left, right); }

          [[nodiscard]] static constexpr BidiClass GetBidiClass(std::uint32_t codePoint) noexcept
          { return UnicodeDatabase::GetBidiClass(codePoint); }

          [[nodiscard]] static constexpr bool IsWhitespace(std::uint32_t codePoint) noexcept
          { return UnicodeDatabase::IsWhitespace(codePoint); }

          [[nodiscard]] static constexpr bool IsAlphabetic(std::uint32_t codePoint) noexcept
          { return UnicodeDatabase::IsAlphabetic(codePoint); }

          [[nodiscard]] static constexpr bool IsUppercase(std::uint32_t codePoint) noexcept
          { return UnicodeDatabase::IsUppercase(codePoint); }

          [[nodiscard]] static constexpr bool IsLowercase(std::uint32_t codePoint) noexcept
          { return UnicodeDatabase::IsLowercase(codePoint); }

          [[nodiscard]] static constexpr bool IsCased(std::uint32_t codePoint) noexcept
          { return UnicodeDatabase::IsCased(codePoint); }

          [[nodiscard]] static constexpr GeneralCategory Category(std::uint32_t codePoint) noexcept
          { return UnicodeDatabase::Category(codePoint); }

          [[nodiscard]] static constexpr Script ScriptOf(std::uint32_t codePoint) noexcept
          { return UnicodeDatabase::ScriptOf(codePoint); }

          [[nodiscard]] static constexpr NumericValue Numeric(std::uint32_t codePoint) noexcept
          { return UnicodeDatabase::Numeric(codePoint); }

          [[nodiscard]] static constexpr bool IsIDStart(std::uint32_t codePoint) noexcept
          { return UnicodeDatabase::IsIDStart(codePoint); }

          [[nodiscard]] static constexpr bool IsIDContinue(std::uint32_t codePoint) noexcept
          { return UnicodeDatabase::IsIDContinue(codePoint); }

          [[nodiscard]] static constexpr std::optional<std::uint32_t> ToLocaleDigit(std::uint32_t asciiDigit) noexcept
          { return Digits::ToLocaleDigit(asciiDigit); }

          [[nodiscard]] static constexpr std::optional<int> FromLocaleDigit(std::uint32_t codePoint) noexcept
          { return Digits::FromLocaleDigit(codePoint); }
     };

     using UnicodeRoot = Unicode<Locale::Root>;
     using UnicodeTR = Unicode<Locale::Turkish>;
     using UnicodeAR = Unicode<Locale::Arabic>;
     using UnicodeHE = Unicode<Locale::Hebrew>;
     using UnicodeJA = Unicode<Locale::Japanese>;
     using UnicodeZH = Unicode<Locale::Chinese>;
     using UnicodeKO = Unicode<Locale::Korean>;
     using UnicodeLT = Unicode<Locale::Lithuanian>;
     using UnicodeEL = Unicode<Locale::Greek>;

     static constexpr Locale DefaultLocale = Locale::Root;
} // namespace text::unicode
