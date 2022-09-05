// SPDX-License-Identifier: Apache-2.0

#include "cero-lexical-analysis.hh"

#include <future>
#include <queue>
#include <fstream>

namespace Cero {
namespace {
  using namespace std; // 7.3.4 [namespace.udir]
}

LexicalAnalysis::LexicalAnalysis(string &stream)
    : m_stream { stream, 0 }
{
  for (;;) {
    optional<Token> token = nullopt;

    do {
      next();
    } while (isspace(read()));

    if (!token) token = keywords();
    if (!token) token = symbols();
    if (!token) break;

    m_tokens.emplace_back(token.value());
  }
}

auto LexicalAnalysis::keywords() const -> optional<Token>
{
  auto c = read();
  if (!isalpha(c))
    return nullopt;

  // std::string represents ~24 bytes per string object, plus ~15 extra bytes
  // for the SSO buffer. This means that a 16 character seq will have ~40 bytes
  // of overhead. On the bright side, keywords and identifiers are usually small
  // enough to use small string optimization which avoid heap allocations.

  string lexeme {};
  do {
    lexeme.push_back(c);
    next();
  } while (isalnum(c = read()) || c == '_');

  if (const auto token = matches(lexeme)
    #define CERO_KEYWORD_TOKEN(kind, lexeme) \
      .Case(lexeme, Token::Kind::kind)
    #include "cero-token.def"
      .Default(Token::Kind::END); token() != Token::Kind::END)
    return Token{ token(), lexeme };
  return Token { Token::Kind::IDENTIFIER, lexeme };
}

auto LexicalAnalysis::symbols() const -> optional<Token>
{
  auto lexeme = string{read()};

  if (const auto token = matches(lexeme)
    #define CERO_SYMBOL_TOKEN(kind, lexeme) \
      .Case(lexeme, Token::Kind::kind)
    #include "cero-token.def"
      .Default(Token::Kind::END); token() != Token::Kind::END)
    return next(), Token { token(), lexeme };
  return next(), nullopt;
}

auto LexicalAnalysis::next() const -> void
{
  m_stream.second++;
}

auto LexicalAnalysis::read() const -> char
{
  return m_stream.second < m_stream.first.size() ? m_stream.first[m_stream.second] : '\0';
}

} // namespace Cero
