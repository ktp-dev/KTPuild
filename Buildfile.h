#ifndef BUILDFILE_H
#define BUILDFILE_H
#include "BuildfileEntry.h"
#include <vector>

class Buildfile {
    //holds all of the "{}" in the Buildfile.json. The last one is the default
    std::vector<BuildfileEntry> tests;
    BuildfileEntry main_exe;
public:
  //this class is responsible for opening and parsing the json file.
  //if file does not exist, or is invalid JSON/schema, throws an error
  Buildfile();
  const std::vector<std::string>& get_cpp_files() const {
      return main_exe.cpp_files;
  }
  const std::vector<std::string>& get_compilation_flags() const {
    return main_exe.compilation_flags;
  }
  const std::vector<std::string>& get_linker_flags() const {
    return main_exe.linker_flags;
  }
  const std::string& get_executable_name() const {
    return main_exe.executable;
  }
};
#endif
