#ifndef BUILDFILE_H
#define BUILDFILE_H
#include <vector>
#include <string>

class Buildfile {

        std::vector<std::string> cpp_files;
        std::vector<std::string> compilation_flags;
        std::vector<std::string> linker_flags;
        std::string executable;
public:
  //this class is responsible for opening and parsing the json file.
  //if file does not exist, or is invalid JSON/schema, throws an error
  Buildfile();
  std::vector<std::string> get_cpp_files() const {
      return cpp_files;
  }
  std::vector<std::string> get_compilation_flags() const {
    return compilation_flags;
  }
  std::vector<std::string> get_linker_flags() const {
    return linker_flags;
  }
  std::string get_executable_name() const {
    return executable;
  }
};
#endif
