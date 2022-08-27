// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace Cero {

struct Token {
  enum struct Kind {
    End,
    Unexpected,
    Transparant,
    Identifier,

    // Symbols
    LeftParenthese,
    RightParenthese,
    LeftBracket,
    RightBracket,
    TrailingReturn,
    SemiColon,
    Colon,

    // Operators
    Comma,

    // Keywords
    Auto,
    Return,
    Int32
  };

  inline static std::unordered_map<std::string, Kind> Keywords {
    {   "auto", Kind::Auto },
    { "return", Kind::Return },
    {    "i32", Kind::Int32 }
  };

  Token() = default;

  Token(const Kind kind) noexcept
      : m_kind { kind }
  {
  }

  Token(const Kind kind, std::string string) noexcept
      : m_kind { kind }
      , m_string { std::move(string) }
  {
  }

  [[nodiscard]] auto kind() const -> Kind { return m_kind; }
  [[nodiscard]] auto string() const -> std::string { return m_string; }

private:
  Kind m_kind {};
  std::string m_string {};
};

class Lexer {
public:
  Lexer() = default;
  auto lex(const std::string_view &line) -> std::vector<Token>;

protected:
  auto next() -> void;
  template <typename T> auto read() -> T;

  auto lex_keywords() -> std::optional<Token>;
  auto lex_symbols() -> std::optional<Token>;
  auto lex_operators() -> std::optional<Token>;

private:
  std::string_view m_line;
  std::size_t m_character{};
  std::vector<Token> m_tokens;
};

} // namespace Cero
