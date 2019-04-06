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
        Buildfile();
        //this class is responsible for opening and parsing the json file. If no such file, provides defaults.
        //if file is invalid JSON/schema, throws an error
  std::vector<std::string> get_cpp_files(){
    return cpp_files;
  }
  std::vector<std::string> get_compilation_flags(){
    return compilation_flags;
  }
  std::vector<std::string> get_linker_flags(){
    return linker_flags;
  }
  std::string get_executable_name(){
    return executable;
  }

};
#endif
