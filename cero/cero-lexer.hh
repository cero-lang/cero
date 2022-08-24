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
    Identifier,
    Auto,
    LeftParen,
    RightParen
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
  Lexer(std::vector<std::string> &source);

  auto get_tokens() -> std::vector<Token>
  {
    return std::move(m_tokens);
  }

protected:
  template <typename T>  auto next() -> T;
  auto lex_alphabet() -> std::optional<Token>;

private:
  struct {
    std::size_t line { 0 };
    std::size_t character { 0 };
  } m_position;

  std::vector<std::string> m_source;
  std::vector<Token> m_tokens;
};

} // namespace Cero
