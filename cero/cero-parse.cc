// SPDX-License-Identifier: Apache-2.0

#include "cero-parse.hh"
#include "cero-ast.hh"

namespace Cero {

auto Parser::parse() -> void
{
  auto ast = std::make_unique<Ast>();
  auto token { Token { Token::Kind::Unexpected } };

  while ((token = m_tokens.lex()).kind() != Token::Kind::End) {
    switch (token.kind()) {
    case Token::Kind::Auto: {
      if ((token = m_tokens.lex()).kind() == Token::Kind::Identifier) {
        const auto previous = token;
        if ((token = m_tokens.lex()).kind() == Token::Kind::LeftParen) {
          ast = parse_function_definition(previous);
          ast->codegen();
        }
      }
    }
    [[fallthrough]] default:
      break;
    }
    break;
  }
}

auto Parser::parse_function_definition(const Token &token) const -> std::unique_ptr<FunctionDefinition>
{
  return std::make_unique<FunctionDefinition>(token);
}

} // namespace Cero
