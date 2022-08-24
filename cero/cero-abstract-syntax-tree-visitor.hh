// SPDX-License-Identifier: Apache-2.0

#pragma once

namespace Cero {

class FunctionDefinition;

class AbstractSyntaxTreeVisitor {
public:
  virtual ~AbstractSyntaxTreeVisitor() = default;
  virtual auto visit(const FunctionDefinition *node) -> void = 0;
};

} // namespace Cero
