// SPDX-License-Identifier: GPL-3.0-or-later

#include <span>
#include <string_view>

using std::string_view;
using std::span;

auto main (const int argc, char *argv[]) noexcept ->  int
{
  for (const string_view argument : span(argv, argc)) {
    if (argument == "--dump-ast") {
      // ...
    }
  }

  return 0;
}
