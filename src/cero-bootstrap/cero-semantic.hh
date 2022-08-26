// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "cero-abstract-syntax-tree-visitor.hh"

namespace Cero {

class Semantic final : public AbstractSyntaxTreeVisitor {
public:
  auto visit(const FunctionDefinition *node) -> void override;
};

} // namespace Cero
