#pragma once

#include <span>
#include <string>

namespace lrd {
// class AttributeHolder {};

// class ActionsContainer {};

class ArgumentParser {
 public:
  ArgumentParser() = default;

  ArgumentParser(std::span<const char* const> args, std::string_view usage = "",
                 std::string_view description = "",
                 std::string_view epilog = "",
                 std::string_view prefixChars = "",
                 std::string_view fromFilePrefixChars = "",
                 std::string_view argumentDefault = "",
                 std::string_view conflictHandler = "", bool addHelp = true,
                 bool allowAbbrev = true, bool exitOnError = true,
                 bool suggestOnError = false);

  ~ArgumentParser() = default;

  friend std::ostream& operator<<(std::ostream& os,
                                  const ArgumentParser& parser);

 private:
  std::string m_programName = "PROGRAM_NAME";
  std::string m_usage = "USAGE";
  std::string m_description = "DESCRIPTION";
  std::string m_epilog;
  // m_parents = [];
  // m_formatterClass = HelpFormatter;
  std::string m_prefixChars = "-";
  std::string m_fromFilePrefixChars;
  std::string m_argumentDefault;
  std::string m_conflictHandler = "error";
  bool m_addHelp = true;
  bool m_allowAbbrev = true;
  bool m_exitOnError = true;
  bool m_suggestOnError = false;
};
}  // namespace lrd
