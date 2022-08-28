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
  auto expect(Token::Kind kind, bool optional = false) -> bool;
  auto parse_function_definition() -> void;

private:
  std::vector<Token> m_collect;

  struct {
    size_t index;
     Token rhs{}, lhs{};
  } m_token;

  std::unique_ptr<AbstractSyntaxTree> m_abstract_syntax_tree;
  std::unique_ptr<Semantic> m_semantic;
};

} // namespace Cero
