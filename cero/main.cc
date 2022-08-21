// SPDX-License-Identifier: Apache-2.0

#include <span>
#include <string>
#include <string_view>
#include <fstream>
#include <vector>

using std::getline;
using std::ifstream;
using std::ios;
using std::runtime_error;
using std::span;
using std::string;
using std::string_view;
using std::vector;
using std::ios_base;

auto main([[maybe_unused]] const int argc, [[maybe_unused]] char *argv[]) -> int
{
  for (string_view argument : span(argv, argc)) {
    if (argument == "v" || argument == "version") { }
    if (argument == "h" || argument == "help") { }

    ifstream read;
    read.exceptions(read.exceptions() | ios::failbit);
    try {
      read.open(argument.data());
    } catch (ios_base::failure &e) {
      throw runtime_error(e.what());
    }

    vector<string> source;
    string line;
    while (getline(read, line)) {
      source.push_back(line);
    }

    // Lexer lexer(source);
    // lexer.next();
  }
  return 0;
}
