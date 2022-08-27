// SPDX-License-Identifier: Apache-2.0

#include "cero-lexer.hh"

namespace Cero {

auto Lexer::lex(const std::string_view &line) -> std::vector<Token>
{
  m_line = line;

  while (true) {
    std::optional<Token> token = std::nullopt;

    if (!token)
      token = lex_alphabet();

    if (!token)
      token = lex_parenthese();

    if (!token) // When all else fails. TODO: Error handling.
      break;

    m_tokens.emplace_back(token.value());
  }

  return m_tokens;
}

template <typename T> auto Lexer::next() -> T
{
  return m_character < m_line.size() ? m_line[m_character] : 0;
}

// TODO: We should rename next to eat or rethink how we consume characters.
auto Lexer::eat() -> void
{
  m_character++;
}

auto Lexer::lex_alphabet() -> std::optional<Token>
{
  auto c = next<char>();
  if (!isalpha(c))
    return std::nullopt;

  // Consume the alphabet character.
  eat();
  std::string s { c };
  while (isalnum(c = next<char>()) || c == '_')
    eat(), s.push_back(c);

  if (Token::Keywords.contains(s))
    return Token { Token::Keywords.at(s), s };
  return Token { Token::Kind::Identifier, s };
}

auto Lexer::lex_parenthese() -> std::optional<Token>
{
  const auto c = next<char>();

  if (c == '(')
    return eat(), Token { Token::Kind::LeftParenthese };
  if (c == ')')
    return eat(), Token { Token::Kind::RightParenthese };

  return std::nullopt;
}

} // namespace Cero
