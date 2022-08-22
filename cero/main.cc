// SPDX-License-Identifier: Apache-2.0

#include "cero-parse.hh"

#include <gsl/assert>

#include <span>
#include <string>
#include <string_view>
#include <fstream>
#include <vector>

auto main([[maybe_unused]] const int argc, [[maybe_unused]] char *argv[]) -> int
{
  std::ifstream read("main.cero");
  std::vector<std::string> source;
  std::string line;

  while (std::getline(read, line))
    source.push_back(line);

  Cero::Parser parser(source);
  parser.parse();
  return 0;
}
