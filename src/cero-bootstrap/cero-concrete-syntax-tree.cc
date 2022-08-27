// SPDX-License-Identifier: Apache-2.0

#include "cero-concrete-syntax-tree.hh"

#include "cero-lexer.hh"
#include "cero-semantic.hh"

#include <format>
#include <memory>

namespace Cero {

// <external-declaration> ::= <function-definition>
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

    expect(Token::Kind::Auto);
    expect(Token::Kind::Identifier);

    if (m_token.rhs.kind() == Token::Kind::LeftParenthese) {
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
  auto name = m_token.lhs.string();

  expect(Token::Kind::LeftParenthese);
  expect(Token::Kind::RightParenthese);

  // type-specifier is optional since it can be inferred from the return type.
  if (m_token.rhs.kind() == Token::Kind::TrailingReturn)
    expect(Token::Kind::TrailingReturn);

  expect(Token::Kind::LeftBracket);
  expect(Token::Kind::RightBracket);

  m_abstract_syntax_tree->add_node(std::make_unique<FunctionDefinition>(name));
}

auto ConcreteSyntaxTree::expect(const Token::Kind kind) -> Token
{
  // TODO: Add a exception message for unexpected token kind.
  if (m_token.rhs.kind() != kind)
    throw;

  // Save the current token.
  m_token.lhs = m_token.rhs;

  // Pop the current token from the collection.
  m_collect.pop_back();

  // If the collection is not empty, get the next token.
  if (m_token.index > 0)
    m_token.rhs = m_collect.at(--m_token.index);

  // Return the current token. This is the token that was consumed.
  return m_token.lhs;
}

} // namespace Cero
