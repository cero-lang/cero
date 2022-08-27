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
    Auto,
    LeftParenthese,
    RightParenthese
  };

  inline static std::unordered_map<std::string, Kind> Keywords {
    { "auto", Kind::Auto }
  };

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
  Kind m_kind;
  std::string m_string {};
};

class Lexer {
public:
  Lexer() = default;
  auto lex(const std::string_view &line) -> std::vector<Token>;

  auto get_tokens() -> std::vector<Token>
  {
    return std::move(m_tokens);
  }

protected:
  template <typename T>  auto next() -> T;
  auto eat() -> void;
  auto lex_alphabet() -> std::optional<Token>;
  auto lex_parenthese() -> std::optional<Token>;

private:
  std::string_view m_line;
  std::size_t m_character{};
  std::vector<Token> m_tokens;
};

} // namespace Cero
