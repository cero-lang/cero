// SPDX-License-Identifier: Apache-2.0

#include "cero-lexical-analysis.hh"

#include <optional>

namespace cero {


auto lexer::scan(const std::string_view &input) -> std::vector<token>
{
  // store the input for read and next.
  m_stream_view = input;

  while (true) {
    std::optional<token> token = std::nullopt;

    if (const auto c = read(); isspace(c))
      next();

    if (!token)
      token = keywords();

    if (!token)
      token = symbols();

    if (!token) // When all else fails. TODO: Error handling.
      break;

    m_tokens.emplace_back(token.value());
  }

  return m_tokens;
}

auto lexer::next() -> void
{
  m_stream_index++;
}

auto lexer::read() const -> char
{
  return m_stream_index < m_stream_view.size() ? m_stream_view[m_stream_index] : '\0';
}


auto lexer::keywords() -> std::optional<token>
{
  auto c = read();
  if (!isalpha(c))
    return std::nullopt;

  next(); std::string lexeme { c };
  while (isalnum(c = read()) || c == '_')
    next(), lexeme.push_back(c);

  if (token::keywords.contains(lexeme))
    return token { token::keywords.at(lexeme), lexeme };
  return token { token::kind::IDENTIFIER, lexeme };
}

auto lexer::symbols() -> std::optional<token>
{
  auto c = read();

  if (c == '{')
    return next(), token { token::kind::LBRACE };
  if (c == '}')
    return next(), token { token::kind::RBRACE };
  if (c == '(')
    return next(), token { token::kind::LPAREN };
  if (c == ')')
    return next(), token { token::kind::RPAREN };
  if (c == ';')
    return next(), token { token::kind::SEMICOLON };
  if (c == ':')
    return next(), token { token::kind::COLON };
  if (c == ',')
    return next(), token { token::kind::COMMA };
  if (c == '-') {
    if (next(), c = read(); c == '>')
      return next(), token { token::kind::ARROW };
  }
  if (c == '/') {
    if (next(), c = read(); c == '/') {
      while (isalnum(c = read())) {
        next();
      }
      return std::nullopt;
    }
  }
  return std::nullopt;
}

// This is a helper function for std::async to call the lexer in
// a separate thread.

auto lexer::async(const std::string_view &input) -> std::vector<token>
{
  lexer lexer;
  return lexer.scan(input);
}

} // namespace cero
