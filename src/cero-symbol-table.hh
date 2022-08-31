// SPDX-License-Identifier: Apache-2.0

#pragma once
#include "cero-lexer.hh"

#include <string>
#include <unordered_map>
#include <vector>

namespace Cero {

class SymTable {
public:
  SymTable();

private:
  using Stack = std::vector<std::string>;
  Stack stack;
};

} // namespace Cero
