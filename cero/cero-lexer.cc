// SPDX-License-Identifier: Apache-2.0

#include "cero-lexer.hh"

#include <cctype>
#include <string>

namespace Cero {

// This is a simple lexer that tokenizes Cero source code.
auto Lexer::lex(const bool cache) -> Token
{
  // Check if we have a cached token. If so, consume it and return it.
  if (!cache && !m_cache.empty()) {
    auto &token = m_cache.front();
    return m_cache.erase(m_cache.begin()), token;
  }

  auto character = read();
  while (isspace(character = read()) != 0)
    next();

  if (character == EOF)
    return { Token::Kind::End };

  // [a-zA-Z][a-zA-Z0-9]*
  if (isalpha(character) || character == '_') {
    std::string identifier;
    while (isalnum(character = read()) || character == '_')
      next(), identifier.push_back(character);
    if (Token::Keywords.contains(identifier))
      return { Token::Keywords.at(identifier), identifier };
    return { Token::Kind::Identifier, identifier };
  }

  if (character == '(')
    return { Token::Kind::LeftParen };
  if (character == ')')
    return { Token::Kind::RightParen };

  return { Token::Kind::Unexpected };
}

//! Returns the character at the current position if there is one, otherwise the
//! stream is advanced until a non-empty line is found. If the stream is at the
//! end of the file, the EOF token is returned.
auto Lexer::read() -> char
{
  if (m_position.line < m_source.size()
      && m_source[m_position.line].length() == 0) {
    next();
  }

  if (m_position.line >= m_source.size()
      || m_position.line == m_source.size() - 1
          && m_position.character == m_source.at(m_position.line).length()) {
    return EOF;
  }

  return m_source[0][m_position.character];
}

//! Advances the stream to the next character. If there is none, the stream is
//! advanced to the next line and the character position is reset to the
//! beginning of the line.
auto Lexer::next() -> void
{
  if (m_position.character < m_source.at(m_position.line).length())
    ++m_position.character;

  else {
    ++m_position.line;
    m_position.character = 0;
  }
}

//! Push the next token(s) onto the cache. This can also be used for lookahead.
auto Lexer::push(const unsigned int offset) -> void
{
  // TODO: Rework this with a proper cache mechanism.
  for (auto i = m_cache.size(); i < offset; i++)
    m_cache.push_back(lex(true));
}

//! pop is a bit of a misnomer, it does not consume, but rather returns the top
//! token on the cache. This is useful for when we want to peek at the next
//! token without consuming it.
auto Lexer::pop() -> Token
{
  // HACK: This ensures that we don't have to deal with the fact that the cache
  // is empty. We need a proper caching mechanism, but for now this is fine.
  if (m_cache.empty())
    push();

  return m_cache.back();
}

} // namespace Cero
