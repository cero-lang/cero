// SPDX-License-Identifier: Apache-2.0

#include "cero-lexer.hh"

namespace Cero {

// Tokenize the source. This is done by splitting the source into lines and
// then splitting each line into tokens. The tokens are then stored in the
// m_tokens vector.

Lexer::Lexer(std::vector<std::string> &source)
    : m_source { std::move(source) }
{
  while (true) {
    std::optional<Token> token = std::nullopt;

    if (!token)
      token = lex_alphabet();

    if (!token) // When all else fails. This is the default case.
      break;

    if (token)
      m_tokens.emplace_back(token.value());
  }
}

template <typename T> auto Lexer::next() -> T
{
  // If the current line is empty, or if the current character is the last one
  // in the line, move to the next line and reset the character position to the
  // beginning of the line.
  if (m_position.line < m_source.size()
          && m_source[m_position.line].length() == 0
      || m_position.character >= m_source.at(m_position.line).length())
    m_position.line++, m_position.character = 0;

  // If there are no more lines in the source, return.
  if (m_position.line >= m_source.size()
      || m_position.line == m_source.size() - 1
          && m_position.character == m_source.at(m_position.line).length()) {
    return 0;
  }

  return m_source[m_position.line][m_position.character++];
}

// Identifiers and keywords are lexed as identifiers.
auto Lexer::lex_alphabet() -> std::optional<Token>
{
  auto c = next<char>();
  if (!isalpha(c))
    return std::nullopt;

  std::string s { c };
  while (isalnum(c = next<char>()) || c == '_')
    s.push_back(c);

  if (Token::Keywords.contains(s))
    return Token { Token::Keywords.at(s), s };
  return Token { Token::Kind::Identifier, s };
}

} // namespace Cero
