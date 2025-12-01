// nova_keywords.h
// Improved perfect hash keyword lookup — fully constexpr, zero runtime cost
#pragma once
#ifndef NOVA_KEYWORDS_H
#define NOVA_KEYWORDS_H

#include <array>
#include <string_view>
#include <cstdint>
#include "tokens.h"

// ------------------------------------------------------------------
// Better 64-bit constexpr hash (xxHash-inspired or better FNV-1a mix)
// ------------------------------------------------------------------
constexpr uint64_t mix64(uint64_t x) {
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33;
    x *= 0xc4ceb9fe1a85ec53ULL;
    x ^= x >> 33;
    return x;
}

constexpr uint64_t const_hash64(const std::string_view sv) noexcept {
    uint64_t h = 0xa0761d6478bd642fULL; // wyhash seed
    for (const char c : sv)
        h = mix64(h ^ static_cast<unsigned char>(c));
    return h;
}

// ------------------------------------------------------------------
// All reserved words + operators (longest first!)
// ------------------------------------------------------------------
constexpr auto TokenTable = std::to_array<std::pair<std::string_view, TokenType>>({
    // Types
    {"i8", TokenType::Type}, {"i16", TokenType::Type}, {"i32", TokenType::Type},
    {"i64", TokenType::Type}, {"u8", TokenType::Type}, {"u16", TokenType::Type},
    {"u32", TokenType::Type}, {"u64", TokenType::Type}, {"f32", TokenType::Type},
    {"f64", TokenType::Type}, {"bool", TokenType::Type}, {"str", TokenType::Type},
    {"void", TokenType::Type},

    // Keywords
    {"let", TokenType::Let}, {"mutable", TokenType::Mutable}, {"return", TokenType::Return},
    {"else", TokenType::Else}, {"while", TokenType::While}, {"for", TokenType::For},
    {"in", TokenType::In}, {"break", TokenType::Break}, {"continue", TokenType::Continue},
    {"switch", TokenType::Switch}, {"struct", TokenType::Struct}, {"enum", TokenType::Enum},
    {"union", TokenType::Union}, {"family", TokenType::Family}, {"entity", TokenType::Entity},

    // Modifiers
    {"public", TokenType::Modifier}, {"private", TokenType::Modifier},
    {"protected", TokenType::Modifier},

    // Operators — longest first (critical!)
    {"&&", TokenType::And}, {"||", TokenType::Or},
    {"==", TokenType::Equal}, {"!=", TokenType::NotEqual},
    {">=", TokenType::GreaterThanOrEqual}, {"<=", TokenType::LessThanOrEqual},
    {"->", TokenType::Arrow},
    {"+=", TokenType::PlusEqual}, {"-=", TokenType::MinusEqual},
    {"*=", TokenType::MultiplyEqual}, {"/=", TokenType::DivideEqual},
    {"+", TokenType::Plus}, {"-", TokenType::Minus},
    {"*", TokenType::Multiply}, {"/", TokenType::Divide},
    {"=", TokenType::Assignment}, {">", TokenType::GreaterThan},
    {"<", TokenType::LessThan}, {"!", TokenType::NotEqual},
    {"%", TokenType::Mod},

    // Punctuation
    {"(", TokenType::LParen}, {")", TokenType::RParen},
    {"{", TokenType::LBrace}, {"}", TokenType::RBrace},
    {"[", TokenType::LBracket}, {"]", TokenType::RBracket},
    {",", TokenType::Comma}, {";", TokenType::Semicolon},
    {":", TokenType::Colon}, {".", TokenType::Dot}, {"...", TokenType::Dots}
});

constexpr std::size_t KeywordCount = TokenTable.size();
constexpr std::size_t TableSize = 512; // 2–3x load factor recommended

// ------------------------------------------------------------------
// Compile-time perfect hash generator (gperf-style brute force)
// ------------------------------------------------------------------
struct PerfectKeywordHash {
    static constexpr std::size_t size = KeywordCount;
    static constexpr std::size_t table_size = TableSize;

    std::array<std::string_view, size> keys{};
    std::array<int8_t, table_size> table{};  // -1 = empty, else index into TokenTable
    uint64_t seed = 0;

    constexpr PerfectKeywordHash() : keys{}, table{} {
        // Extract keys
        for (std::size_t i = 0; i < size; ++i)
            keys[i] = TokenTable[i].first;

        // Fill table with -1
        table.fill(-1);

        // Brute-force find perfect seed (will succeed quickly with good hash)
        bool found = false;
        for (seed = 0; seed < 10'000'000 && !found; ++seed) {
            std::array<int8_t, table_size> temp{};
            temp.fill(-1);
            bool collision = false;

            for (std::size_t i = 0; i < size; ++i) {
                const uint64_t h = const_hash64(keys[i]);
                const std::size_t idx = mix64(h ^ seed) & (table_size - 1); // better mixing

                if (temp[idx] != -1) {
                    collision = true;
                    break;
                }
                temp[idx] = static_cast<int8_t>(i);
            }

            if (!collision) {
                table = temp;
                found = true;
            }
        }
        if (!found) {
            // can't static_assert here (constructor always instantiates)
            __builtin_unreachable(); // let compile fail naturally
        }


    }

    // ------------------------------------------------------------------
    // constexpr lookup — zero runtime cost
    // ------------------------------------------------------------------
    [[nodiscard]] constexpr TokenType lookup(const std::string_view sv) const noexcept {
        if (sv.empty()) return TokenType::Unknown;

        const uint64_t h = const_hash64(sv);
        const std::size_t idx = mix64(h ^ seed) & (table_size - 1);
        int8_t i = table[idx];

        if (i == -1) return TokenType::Identifier;
        if (keys[i] != sv) return TokenType::Identifier; // hash collision (very rare)

        return TokenTable[i].second;
    }
};

// ------------------------------------------------------------------
// Global constexpr instance — fully computed at compile time
// ------------------------------------------------------------------
inline constexpr PerfectKeywordHash KeywordHasher{};

// ------------------------------------------------------------------
// Public API — blazing fast, constexpr, no overhead
// ------------------------------------------------------------------
constexpr TokenType lookupToken(const std::string_view sv) noexcept {
    return KeywordHasher.lookup(sv);
}

#endif // NOVA_KEYWORDS_H