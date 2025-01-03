#include "argument_parser.hpp"

namespace lrd {
ArgumentParser::ArgumentParser(std::string_view description)
    : m_description(description) {}

void ArgumentParser::add_argument(std::string_view name, std::string_view help,
                                  bool required, std::string_view default_value,
                                  std::string_view validation_regex,
                                  char short_flag) {
  std::string extended_help = std::string(help);
  if (!validation_regex.empty() && validation_regex == "^(true|false)$") {
    extended_help += " (valid values: true, false)";
  }
  m_arguments[std::string(name)] = {extended_help,
                                    required,
                                    std::string(default_value),
                                    "",
                                    std::string(validation_regex),
                                    short_flag};
  if (short_flag != '\0') {
    m_short_to_long[short_flag] = std::string(name);
  }
}

void ArgumentParser::parse(int argc, char* argv[]) {
  if (argc == 1) {
    print_help();
    throw std::runtime_error(
        "No arguments provided. Please provide valid arguments or use --help "
        "to view usage instructions.");
  }

  std::vector<std::string_view> args(argv + 1, argv + argc);
  for (size_t i = 0; i < args.size(); ++i) {
    if (args[i] == "--help") {
      print_help();
      handle_help_output();
      std::exit(0);
    }

    if (args[i].starts_with("--")) {
      parse_long_flag(args, i);
    } else if (args[i].starts_with('-')) {
      parse_short_flags(args, i);
    } else {
      throw std::runtime_error("Unexpected argument: " + std::string(args[i]));
    }
  }

  // Check for missing required arguments
  for (const auto& [key, arg] : m_arguments) {
    if (arg.required && arg.value.empty()) {
      throw std::runtime_error("Missing required argument: " + key);
    }
  }

  // Validate dependencies between arguments
  validate_dependencies();
}

std::string ArgumentParser::get(std::string_view name) const {
  auto it = m_arguments.find(std::string(name));
  if (it != m_arguments.end()) {
    return it->second.value.empty() ? it->second.default_value
                                    : it->second.value;
  }
  throw std::runtime_error("Argument not found: " + std::string(name));
}

void ArgumentParser::print_help(const std::string& output_file) const {
  std::ostream* out = &std::cout;
  std::ofstream file;

  if (!output_file.empty()) {
    file.open(output_file);
    if (!file.is_open()) {
      throw std::runtime_error("Could not open file for writing: " +
                               output_file);
    }
    out = &file;
  }

  (*out) << m_description << "\n\nArguments:\n";
  for (const auto& [name, arg] : m_arguments) {
    (*out) << "  --" << name;
    if (arg.short_flag != '\0') {
      (*out) << " (-" << arg.short_flag << ")";
    }
    (*out) << "\t" << arg.help;
    if (!arg.default_value.empty()) {
      (*out) << " (default: " << arg.default_value << ")";
    }
    (*out) << "\n";
  }
  (*out) << "  --help\tShow this help message and exit\n";

  if (file.is_open()) {
    file.close();
  }
}

void ArgumentParser::handle_help_output(const std::string& output_file) const {
  if (!output_file.empty()) {
    print_help(output_file);
    std::cout << "Help message written to " << output_file << "\n";
  } else {
    print_help();
  }
}

void ArgumentParser::parse_long_flag(const std::vector<std::string_view>& args,
                                     size_t& i) {
  std::string_view name = args[i].substr(2);
  if (name.empty()) {
    throw std::runtime_error("Invalid argument: " + std::string(args[i]));
  }

  auto arg_iter = m_arguments.find(std::string(name));
  if (arg_iter == m_arguments.end()) {
    throw std::runtime_error("Unknown argument: " + std::string(name));
  }

  if (i + 1 < args.size() && !args[i + 1].starts_with('-')) {
    arg_iter->second.value = std::string(args[++i]);

    if (!arg_iter->second.validation_regex.empty() &&
        !std::regex_match(arg_iter->second.value,
                          std::regex(arg_iter->second.validation_regex))) {
      throw std::runtime_error("Invalid value for argument --" +
                               std::string(name) + ": " +
                               arg_iter->second.value);
    }
  } else if (arg_iter->second.required) {
    throw std::runtime_error("Missing value for required argument: " +
                             std::string(name));
  }
}

void ArgumentParser::parse_short_flags(
    const std::vector<std::string_view>& args, size_t& i) {
  std::string_view flags = args[i].substr(1);
  for (char flag : flags) {
    auto iter = m_short_to_long.find(flag);
    if (iter == m_short_to_long.end()) {
      throw std::runtime_error(std::string("Unknown flag: -") + flag);
    }

    auto& argument = m_arguments[iter->second];
    if (!argument.value.empty()) {
      continue;  // Skip if already set
    }

    if (i + 1 < args.size() && flags.size() == 1 &&
        !args[i + 1].starts_with('-')) {
      argument.value = std::string(args[++i]);
    } else {
      argument.value = "true";
    }
  }
}

void ArgumentParser::validate_dependencies() {
  for (const auto& [key, arg] : m_arguments) {
    if (arg.required && arg.value.empty()) {
      throw std::runtime_error("Missing required argument: " + key);
    }
  }

  // Example of extended dependency handling
  if (auto input_it = m_arguments.find("input");
      input_it != m_arguments.end() && input_it->second.value.empty()) {
    if (auto output_it = m_arguments.find("output");
        output_it != m_arguments.end() && !output_it->second.value.empty()) {
      throw std::runtime_error(
          "The 'input' argument is required when 'output' is provided.");
    }
  }

  if (auto verbose_it = m_arguments.find("verbose");
      verbose_it != m_arguments.end() && verbose_it->second.value == "true") {
    if (auto output_it = m_arguments.find("output");
        output_it != m_arguments.end() && output_it->second.value.empty()) {
      throw std::runtime_error(
          "The 'output' argument is required when 'verbose' mode is "
          "enabled.");
    }
  }
}
}  // namespace lrd
