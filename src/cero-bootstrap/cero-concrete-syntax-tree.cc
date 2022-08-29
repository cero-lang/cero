// SPDX-License-Identifier: Apache-2.0

#include "cero-concrete-syntax-tree.hh"

#include "cero-codegen-llvm.hh"
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
    : m_tokens(std::move(tokens))
    , m_token { m_tokens.at(m_tokens.size() - 1), m_tokens.size() - 1 }
    , m_abstract_syntax_tree { std::make_unique<AbstractSyntaxTree>() }
    , m_semantic { std::make_unique<Semantic>() }
{
  // For now, auto is essentially the only type of declaration. Therefore it's
  // okay to consume the token immediately. If we add more types of
  // declarations, we'll need to revisit this.
  while (!m_tokens.empty()) {
    expect(Token::Kind::NAMESPACE, [this] {
      expect(Token::Kind::IDENTIFIER, m_optional);
      m_namespace.second = m_token.label;
      expect(Token::Kind::LBRACE, [this] {
        m_namespace.first = !m_namespace.first;
      });
    });
    expect(Token::Kind::AUTO);
    expect(Token::Kind::IDENTIFIER);
    expect(Token::Kind::LPAREN, [this] {
      parse_function_definition();
    });
    expect(Token::Kind::RBRACE, m_namespace.first);
    expect(Token::Kind::END);
  }

  m_abstract_syntax_tree->visit(m_semantic.get());
  m_abstract_syntax_tree->codegen();
}

auto ConcreteSyntaxTree::parse_function_definition() -> void
{
  // TODO: There are some decisions that need to be made about Cero's type system.
  //       At the moment, we're just going to assume that the type is void and
  //       that the function definition doesn't have any parameters.

  expect(Token::Kind::RPAREN);
  expect(Token::Kind::LBRACE);
  expect(Token::Kind::RBRACE);

  m_abstract_syntax_tree->add_node(std::make_unique<FunctionDefinition>(m_token.label, m_namespace));
}

// TODO: We need more flexibility in the way we handle optional tokens values.
auto ConcreteSyntaxTree::expect(const Token::Kind kind, const bool optional) -> bool
{
  // Avoid throwing an exception if the token is optional.
  if (m_token() != kind)
    return optional ? false : throw;

  // Pop the current token from the collection and advance to the next token.
  if (m_tokens.pop_back(); m_token() != Token::Kind::END)
    m_token = m_tokens.at(--m_token);

  return true;
}

// This overload make it easier to determine which rules are applicable to a
// given token. For example, if we want to parse a function definition, we can
// just call parse_function_definition() if the current token is a LPAREN.
// Otherwise, we can just skip the callback.

auto ConcreteSyntaxTree::expect(const Token::Kind kind, const std::function<void()> &callback) -> void
{
  if (m_token() != kind)
    return;

  // Pop the current token from the collection and advance to the next token.
  if (m_tokens.pop_back(); m_token() != Token::Kind::END)
    m_token = m_tokens.at(--m_token);

  callback();
}

} // namespace Cero
