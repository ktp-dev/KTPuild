#ifndef DEFAULTS_H
#define DEFAULTS_H
#include <vector>
#include <string>
//default cpp files is just *.cpp, which can be handled by Buildfile ctor
//std::vector<std::string> default_cpp_files;
std::vector<std::string> default_compilation_flags;
std::vector<std::string> default_linker_flags;
std::string default_exe_name;
#endif
