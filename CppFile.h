#ifndef CPPFILE_H
#define CPPFILE_H
#include <string>
bool has_changed(const std::string& filename);
std::string convert_to_obj_file(const std::string& cpp_file);
#endif
