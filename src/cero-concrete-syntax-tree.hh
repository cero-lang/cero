// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "cero-abstract-syntax-tree.hh"
#include "cero-lexer.hh"
#include "cero-semantic.hh"

namespace Cero {

class ConcreteSyntaxTree {
public:
  ConcreteSyntaxTree(std::vector<Token> tokens);

protected:
  auto parse_function_definition() -> void;
  auto expect(Token::Kind kind, bool optional = false) -> bool;
  auto expect(Token::Kind kind, const std::function<void()> &callback) -> void;

private:
  std::vector<Token> m_tokens;

  // clang-format off
  struct {
    Token token;
    std::size_t index;
    std::string label;

    // Overload some operators to make it easier to access the token.
    // This is somewhat ugly, but it prevents having to write a lot of boilerplate code.

    auto operator() () const -> Token::Kind { return this->index > 0 ? token.kind() : Token::Kind::END; }
    auto operator=  (const Token &token) -> void { this->token = token; if (token.kind() == Token::Kind::IDENTIFIER) label = m_namespace.first ? m_namespace.second + "::" + token.label() : token.label(); }
    auto operator-- () -> size_t { return --index; }
  } m_token;
  // clang-format on

  std::unique_ptr<AbstractSyntaxTree> m_abstract_syntax_tree;
  std::unique_ptr<Semantic> m_semantic;

  // Helper
  bool m_optional = true;
public:
  static inline std::pair<bool, std::string> m_namespace;
};

} // namespace Cero
