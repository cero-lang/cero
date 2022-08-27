// SPDX-License-Identifier: Apache-2.0

#include "cero-concrete-syntax-tree.hh"

#include "cero-lexer.hh"
#include "cero-semantic.hh"

#include <format>
#include <iostream>
#include <memory>

namespace Cero {

// <external-declaration> ::= <function-definition>
//                          | <declaration>

ConcreteSyntaxTree::ConcreteSyntaxTree(std::vector<Token> tokens)
    : m_tokens(std::move(tokens))
    , m_index { m_tokens.size() - 1 }
    , m_token { m_tokens.at(m_index) }
    , m_abstract_syntax_tree { std::make_unique<AbstractSyntaxTree>() }
    , m_semantic { std::make_unique<Semantic>() }
{
  // <type-specifier>
  if (m_token.kind() != Token::Kind::Auto)
    throw;

  // <identifier>
  m_token = expect(Token::Kind::Identifier);

  // <parameter-list>
  if (lookahead(1).kind() == Token::Kind::LeftParenthese)
    parse_function_definition();

  m_abstract_syntax_tree->visit(m_semantic.get());
  m_abstract_syntax_tree->codegen();
}

// <function-definition> ::= <type-specifier> <identifier> <parameter-list>
// <compound-statement>
auto ConcreteSyntaxTree::parse_function_definition() -> void
{
  expect(Token::Kind::LeftParenthese);
  m_abstract_syntax_tree->add_node(make_unique<FunctionDefinition>(m_token.string()));
}

// Consume the next token if it's the expected kind. Throw an exception
// otherwise.
auto ConcreteSyntaxTree::expect(const Token::Kind kind) -> Token
{
  m_tokens.pop_back();
  m_token = m_tokens.at(--m_index);

  if (m_token.kind() == kind)
    return m_token;

  _exit(-1);
}

auto ConcreteSyntaxTree::lookahead(const unsigned long long index) -> Token

{
  if (m_index - index <= m_tokens.size())
    return m_tokens.at(m_index - index);

  return Token{ Token::Kind::Transparant };
}

} // namespace Cero
