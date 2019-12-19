#ifndef BUILDFILE_ENTRY_H
#define BUILDFILE_ENTRY_H
#include <vector>
#include <string>
struct BuildfileEntry {
    std::vector<std::string> cpp_files;
    std::vector<std::string> compilation_flags;
    std::vector<std::string> linker_flags;
    std::string executable;
    bool is_test;
};
#endif
