// SPDX-License-Identifier: Apache-2.0

#pragma once

namespace Cero {

class FunctionDeclaration;

class ASTVisitor {
public:
  virtual void VisitFunctionDeclaration(const FunctionDeclaration *ast) = 0;
};

} // namespace Cero
