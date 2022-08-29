// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <llvm/Support/Error.h>

namespace Cero {

class JIT {
public:
  JIT();

private:
  llvm::ExitOnError ExitOnErr;
};

} // namespace Cero
