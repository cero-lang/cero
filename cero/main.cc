// SPDX-License-Identifier: Apache-2.0

#include <span>
#include <string_view>
#include <fstream>
#include <vector>

using std::string_view;
using std::span;

auto main([[maybe_unused]] const int argc, [[maybe_unused]] char *argv[]) -> int
{
  for (std::string_view argument : std::span(argv, argc)) {
    if (argument == "v" || argument == "version") { }
    if (argument == "h" || argument == "help") { }

    std::ifstream read;
    read.exceptions(read.exceptions() | std::ios::failbit);
    try {
      read.open(argument.data());
    } catch (std::ios_base::failure &e) {
      std::runtime_error error(e.what());
    }

    std::vector<std::string> source;
    std::string line;
    while (std::getline(read, line)) {
      source.push_back(line);
    }

    // Lexer lexer(source);
    // lexer.next();
  }
  return 0;
}
