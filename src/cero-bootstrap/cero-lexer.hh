// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace Cero {

struct Token {
  enum struct Kind {
    // ?
    END,
    IDENTIFIER,

    // Symbols
    LBRACE,     // {
    RBRACE,     // }
    LPAREN,     // (
    RPAREN,     // )
    COLON,      // :
    SEMICOLON,  // ;
    COMMA,      // ,
    ARROW,      // ->
    DIV,        // /

    // Keywords
    NAMESPACE,
    AUTO,
    RETURN,
    VOID,

    // Literals
    INTEGER,
    CHAR,
    FLOAT,
    DOUBLE,
    STRING,
    BOOL
  };

  inline static std::unordered_map<std::string, Kind> KEYWORDS {
    { "namespace", Kind::NAMESPACE },
    {      "auto", Kind::AUTO },
    {    "return", Kind::RETURN },
    {      "void", Kind::VOID },
  };

  Token() = default;

  Token(const Kind kind) noexcept
      : m_kind { kind }
  {
  }

  Token(const Kind kind, std::string label) noexcept
      : m_kind { kind }
      , m_label { std::move(label) }
  {
  }

  [[nodiscard]] auto kind() const -> Kind { return m_kind; }
  [[nodiscard]] auto label() const -> std::string { return m_label; }

private:
  Kind m_kind {};
  std::string m_label {};
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

private:
  std::string_view m_line;
  std::size_t m_character{};
  std::vector<Token> m_tokens;
};

} // namespace Cero
