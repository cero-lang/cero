// SPDX-License-Identifier: Apache-2.0

#include "cero-lexer.hh"

namespace Cero {

auto Lexer::lex(const std::string_view &line) -> std::vector<Token>
{
  m_line = line;

  while (true) {
    std::optional<Token> token = std::nullopt;

    if (const auto c = read<char>(); isspace(c))
      next();

    if (!token)
      token = lex_keywords();

    if (!token)
      token = lex_symbols();

    if (!token) // When all else fails. TODO: Error handling.
      break;

    m_tokens.emplace_back(token.value());
  }

  return m_tokens;
}

auto Lexer::next() -> void
{
  m_character++;
}

template <typename T> auto Lexer::read() -> T
{
  return m_character < m_line.size() ? m_line[m_character] : 0;
}

auto Lexer::lex_keywords() -> std::optional<Token>
{
  auto c = read<char>();
  if (!isalpha(c))
    return std::nullopt;

  // We only consume after we've found an alphabetical character.
  // This is to prevent the lexer from consuming the first character of a line.
  next();

  std::string s { c };
  while (isalnum(c = read<char>()) || c == '_')
    next(), s.push_back(c);

  if (Token::KEYWORDS.contains(s))
    return Token { Token::KEYWORDS.at(s), s };
  return Token { Token::Kind::IDENTIFIER, s };
}

auto Lexer::lex_symbols() -> std::optional<Token>
{
  auto c = read<char>();

  if (c == '{')
    return next(), Token { Token::Kind::LBRACE };
  if (c == '}')
    return next(), Token { Token::Kind::RBRACE };
  if (c == '(')
    return next(), Token { Token::Kind::LPAREN };
  if (c == ')')
    return next(), Token { Token::Kind::RPAREN };
  if (c == ';')
    return next(), Token { Token::Kind::SEMICOLON };
  if (c == ':')
    return next(), Token { Token::Kind::COLON };
  if (c == ',')
    return next(), Token { Token::Kind::COMMA };
  if (c == '-') {
    if (next(), c = read<char>(); c == '>')
      return next(), Token { Token::Kind::ARROW };
  }

  return std::nullopt;
}

} // namespace Cero
