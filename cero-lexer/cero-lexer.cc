// SPDX-License-Identifier: GPL-3.0-or-later

#include "cero-lexer.hh"

#include <cctype>
#include <string>

using namespace std::string_literals; // enables s-suffix for std::string literals

namespace Cero {

auto Lexer::next() -> Token
{
  auto character = ' ';
  auto string = " "s; // moved into the Token constructor.

  while (isspace(character))
    character = read();

  // [a-zA-Z][a-zA-Z0-9]*
  if (isalpha(character)) {
    string = character;
    while (isalnum(character = read()))
      string += character;
    if (Token::Keywords.contains(string))
      return {Token::Keywords.at(string), string};
    return Token{Token::Kind::Identifier, string};
  }

  //[0-9.]+
  if (isdigit(character) || character == '.') {
    do {
      string += std::to_string(character);
      character = read();
    }
    while (isdigit(character) || character == '.');
    return {Token::Kind::Number, strtod(string.c_str(), nullptr)};
  }

  // EOF doesn't exist if source is a string literal
  if (character == EOF)
    return {Token::Kind::End};
  return {Token::Kind::Unexpected};
}

auto Lexer::read() -> char
{
  if (m_position == m_string_view.length())
    return '\0';
  return m_string_view.at(m_position++);
}

} // namespace cero
