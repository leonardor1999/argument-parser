#include <iostream>

#include "argument_parser.hpp"

int main(int argc, char* argv[]) {
  lrd::ArgumentParser parser("Example Argument Parser");

  parser.add_argument("input", "Path to input file", true, "",
                      "^.+\\.(txt|csv)$", 'i');
  parser.add_argument("output", "Path to output file", true, "", "", 'o');
  parser.add_argument("verbose", "Enable verbose mode", false, "false",
                      "^(true|false)$", 'v');

  try {
    parser.parse(argc, argv);

    auto get_arg = [&parser](std::string_view name) {
      return parser.get(name);
    };

    std::string input = get_arg("input");
    std::string output = get_arg("output");
    std::string verbose = get_arg("verbose");

    std::cout << "Input: " << input << "\n";
    std::cout << "Output: " << output << "\n";
    std::cout << "Verbose: " << verbose << "\n";

  } catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
    parser.print_help();
    return 1;
  }

  return 0;
}