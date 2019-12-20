#ifndef BUILDFILE_H
#define BUILDFILE_H
#include "BuildfileEntry.h"
#include <vector>

struct Buildfile {
    //holds all of the "{}" in the Buildfile.json. The last one is the default
    std::vector<BuildfileEntry> tests;
    BuildfileEntry main_exe;

  //this class is responsible for opening and parsing the json file.
  //if file does not exist, or is invalid JSON/schema, throws an error
  Buildfile();
};
#endif
