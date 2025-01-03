#include <cstddef>
#include <iostream>
#include <span>

#include "argument_parser.hpp"

int main(const int argc, const char* const* const argv) {
  std::span<const char* const> args(argv, static_cast<size_t>(argc));

  lrd::ArgumentParser parser{args};
  std::cout << parser << '\n';
}