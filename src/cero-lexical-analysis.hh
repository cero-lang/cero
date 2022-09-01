//! \file cero-lexical-analysis.hh
//!
//! # Cero's lexical analysis.
//!
//! The lexer form the first phase of Cero's front-end. It convert a sequence of characters into a
//! sequence of lexical tokens (strings with an assigned and thus identified meaning). Lexing can be
//! divided into two stages: the scanning, which segments the input string into syntactic units
//! called lexemes(\ref footnote "1") and categorizes these into token(\ref footnote "2") classes; and the evaluating,
//! which converts lexemes into processed values. Most of the complexity is deferred to semantic
//! analysis phases.
//!
//! \anchor footnote 1. A lexeme is a sequence of characters in the source program that matches the
//! pattern for a token and is identified by the lexical analyzer as an instance of that token.
//!
//! \anchor footnote 2. A token is a pair consisting of a token name and an optional attribute
//! value. The token name is an abstract symbol representing a kind of lexical unit, e.g., a
//! particular keyword, or sequence of input characters denoting an identifier. The token names are
//! the input symbols that the parser processes.
//!
//! SPDX-License-Identifier: Apache-2.0

#pragma once

#include <any>
#include <optional>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace cero {

struct token {
  enum struct kind {
    LBRACE,     // {
    RBRACE,     // }
    LPAREN,     // (
    RPAREN,     // )
    COLON,      // :
    SEMICOLON,  // ;
    COMMA,      // ,
    ARROW,      // ->
    END,
    IDENTIFIER,
    AUTO,
  };

  inline static std::unordered_map<std::string, kind> keywords {
    { "auto", kind::AUTO },
  };

  //! An attribute is not always a lexeme. std::optional<std::any> is used to represent this
  //! distinction for now. Revisit this later?
  explicit token(const kind kind, std::optional<std::any> &&attribute = std::nullopt) noexcept
      : m_token { kind, attribute }
  {
  }

private:
  std::tuple<kind, std::optional<std::any>> m_token;
};

class lexer {
public:
  lexer() = default;

  auto scan(const std::string_view &input) -> std::vector<token>;
  auto next() -> void;
  auto read() const -> char;

  static auto async(const std::string_view &input) -> std::vector<token>;

  auto keywords() -> std::optional<token>;
  auto symbols() -> std::optional<token>;

private:
  std::string_view   m_stream_view;
  std::size_t        m_stream_index{};
  std::vector<token> m_tokens;
};


} // namespace cero
