#ifndef BUILDFILE_H
#define BUILDFILE_H
#include <vector>
#include <string>
class Buildfile {
    public:
        //this class is responsible for opening and parsing the json file. If no such file, provides defaults.
        //if file is invalid JSON/schema, throws an error
        std::vector<std::string> get_cpp_files();
        std::vector<std::string> get_compilation_flags();
        std::vector<std::string> get_linker_flags();
        std::string get_executable_name();
};
#endif
