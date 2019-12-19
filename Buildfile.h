#ifndef BUILDFILE_H
#define BUILDFILE_H
#include "BuildfileEntry.h"
#include <vector>

class Buildfile {
    //holds all of the "{}" in the Buildfile.json. The last one is the default
    std::vector<BuildfileEntry> entries;
public:
  //this class is responsible for opening and parsing the json file.
  //if file does not exist, or is invalid JSON/schema, throws an error
  Buildfile();
  std::vector<std::string> get_cpp_files() const {
      return entries.back().cpp_files;
  }
  std::vector<std::string> get_compilation_flags() const {
    return entries.back().compilation_flags;
  }
  std::vector<std::string> get_linker_flags() const {
    return entries.back().linker_flags;
  }
  std::string get_executable_name() const {
    return entries.back().executable;
  }
};
#endif
