#include "argument_parser.hpp"

#include <filesystem>
#include <ostream>

namespace lrd {
ArgumentParser::ArgumentParser(
    std::span<const char* const> args, std::string_view usage,
    std::string_view description, std::string_view epilog,
    std::string_view prefixChars, std::string_view fromFilePrefixChars,
    std::string_view argumentDefault, std::string_view conflictHandler,
    bool addHelp, bool allowAbbrev, bool exitOnError, bool suggestOnError)
    : m_programName{static_cast<std::filesystem::path>(args[0])
                        .filename()
                        .string()},
      m_usage{usage},
      m_description{description},
      m_epilog{epilog},
      m_prefixChars{prefixChars},
      m_fromFilePrefixChars{fromFilePrefixChars},
      m_argumentDefault{argumentDefault},
      m_conflictHandler{conflictHandler},
      m_addHelp{addHelp},
      m_allowAbbrev{allowAbbrev},
      m_exitOnError{exitOnError},
      m_suggestOnError{suggestOnError} {}

std::ostream& operator<<(std::ostream& os, const ArgumentParser& parser) {
  return os << "ArgumentParser(prog = " << parser.m_programName
            << ", usage = " << parser.m_usage
            << ", description = " << parser.m_description
            << ", formatter_class = " << "TODO"
            << ", conflict_handler = " << parser.m_conflictHandler
            << std::boolalpha << ", add_help = " << parser.m_addHelp << ')';
}
}  // namespace lrd