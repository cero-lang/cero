// SPDX-License-Identifier: Apache-2.0

#pragma once

namespace Cero {

class FunctionDefinition;

class AstVisitor {
public:
  virtual ~AstVisitor() = default;
  virtual auto visit_function_definition(const FunctionDefinition *ast) -> void = 0;
};

} // namespace Cero
