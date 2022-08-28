// SPDX-License-Identifier: Apache-2.0

#include "cero-concrete-syntax-tree.hh"

#include "cero-lexer.hh"
#include "cero-semantic.hh"

#include <format>
#include <memory>

namespace Cero {

// <external-declaration> ::= <module-definition>
//                          | <namespace-definition>
//                          | <function-definition>
//                          | <declaration>

ConcreteSyntaxTree::ConcreteSyntaxTree(std::vector<Token> tokens)
    : m_collect(std::move(tokens))
    , m_token { m_collect.size() - 1, m_collect.at(m_collect.size() - 1) }
    , m_abstract_syntax_tree { std::make_unique<AbstractSyntaxTree>() }
    , m_semantic { std::make_unique<Semantic>() }
{
  while (!m_collect.empty()) {

    // For now, auto is essentially the only type of declaration. Therefore it's
    // okay to consume the token immediately. If we add more types of declarations,
    // we'll need to revisit this.

    expect(Token::Kind::AUTO);
    expect(Token::Kind::IDENTIFIER);

    if (m_token.rhs.kind() == Token::Kind::LPAREN) {
      parse_function_definition();
    } else {
      // parse_declaration();
    }
  }

  m_abstract_syntax_tree->visit(m_semantic.get());
  m_abstract_syntax_tree->codegen();
}

// <function-definition> ::= <?> <identifier> <parameter-list> <trailing-return> <type-specifier>
// <compound-statement>

auto ConcreteSyntaxTree::parse_function_definition() -> void
{
  auto identifier = m_token.lhs.string();

  // <parameter-list>
  expect(Token::Kind::LPAREN);
  while (expect(Token::Kind::IDENTIFIER, true)) {
    if (expect(Token::Kind::COLON, true))
      expect(Token::Kind::INTEGER);
    // Expect either a comma or a right parenthese.
    if (!expect(Token::Kind::COMMA, true))
      break;
  }

  // <trailing-return>
  // type-specifier is optional since it can be inferred from the return keyword.
  expect(Token::Kind::RPAREN);
  if (expect(Token::Kind::ARROW, true)) {
    // <type-specifier>
    expect(Token::Kind::INTEGER);
  }

  // <compound-statement>
  expect(Token::Kind::LBRACE);
  expect(Token::Kind::RBRACE);

  m_abstract_syntax_tree->add_node(
      std::make_unique<FunctionDefinition>(identifier));
}

auto ConcreteSyntaxTree::expect(const Token::Kind kind, const bool optional) -> bool
{
  // TODO: Add a exception message for unexpected token kind.
  if (m_token.rhs.kind() != kind) {
    // Sometime it is fine to omit the Token.
    if (optional) {
      return false;
    }
    throw;
  }

  // Save the current token.
  m_token.lhs = m_token.rhs;

  // Pop the current token from the collection.
  m_collect.pop_back();

  // If the collection is not empty, get the next token.
  if (m_token.index > 0)
    m_token.rhs = m_collect.at(--m_token.index);

  // Return the current token. This is the token that was consumed.
  return true;
}

} // namespace Cero
