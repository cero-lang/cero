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
  };

  // clang-format off

  inline static std::unordered_map<std::string, Kind> Keywords {
    { "auto", Kind::Auto }
  };

  // clang-format on

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
  Kind m_kind { Kind::End };
  std::string m_string {};
};

class Lexer {
public:
  Lexer(std::vector<std::string> &source)
      : m_source { std::move(source) }
  {
    push();
  }

  //! Lexes the next token.
  auto lex(bool cache = false) -> Token;
  //! Helper function to read the next character from the source.
  auto read() -> int;
  //! Helper function to forward the next character from the source.
  auto next() -> void;
  //! Helper function to cache a token.
  auto push(unsigned int offset = 1) -> void;
  //! Helper function to pop a token from the cache.
  auto pop() -> Token;

  // TODO:
  // We should probably create more helper functions for the other lexer instead
  // of adding them all in one place ( Lexer::lex() ).
  //   auto lex_keyword() -> std::optional<Token>;
  //   auto lex_identifier() -> std::optional<Token>;

private:
  struct {
    std::size_t line { 0 };
    std::size_t character { 0 };
  } m_position;

  std::vector<std::string> m_source;
  std::vector<Token> m_cache;
};

} // namespace Cero
