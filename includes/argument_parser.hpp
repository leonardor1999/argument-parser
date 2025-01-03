#pragma once

#include <concepts>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace lrd {
class ArgumentParser {
 public:
  explicit ArgumentParser(std::string_view description = "");

  void add_argument(std::string_view name, std::string_view help = "",
                    bool required = false, std::string_view default_value = "",
                    std::string_view validation_regex = "",
                    char short_flag = '\0');

  void parse(const int argc, const char* const* const argv);

  std::string get(std::string_view name) const;

  void print_help(const std::string& output_file = "") const;

  void handle_help_output(const std::string& output_file = "") const;

 private:
  struct Argument {
    std::string help;
    bool required;
    std::string default_value;
    std::string value;
    std::string validation_regex;
    char short_flag;
  };

  std::string m_description;
  std::unordered_map<std::string, Argument> m_arguments;
  std::unordered_map<char, std::string> m_short_to_long;

  void parse_long_flag(const std::vector<std::string_view>& args, size_t& i);

  void parse_short_flags(const std::vector<std::string_view>& args, size_t& i);

  void validate_dependencies();
};
}  // namespace lrd
