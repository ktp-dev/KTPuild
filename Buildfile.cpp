//
//  Buildfile.cpp
//  KTPuild-Jarek
//
//  Created by Nathan Brown on 4/6/19.
//  Copyright © 2019 Nathan Brown. All rights reserved.
//

#include "Buildfile.h"
#include "BuildfileEntry.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace std;
using namespace nlohmann;

Buildfile :: Buildfile(){
  json data;
  ifstream fs("Buildfile.json");
  if (!fs.is_open()) {
    std::cerr << "Missing build file, -10 KTPoints\n";
    throw(std::runtime_error("no buildfile"));
  }

  //parses JSON file. Will throw if invalid JSON
  fs >> data;
  if(!data.is_array()) {
      throw std::runtime_error("Error! Buildfile.json must be in an array format. If you're seeing this, adding '[' to the beginning of Buildfile.json and ']' to the end will likely fix this issue.");
  }
  bool found_main = false;
  for(const auto& entry : data) {
    BuildfileEntry current{entry["Sources"], entry["Flags"], entry["Flags"], entry["Name"], entry.value("test", false)};
    if(current.is_test) {
        tests.emplace_back(std::move(current));
    } else if(found_main) {
        throw std::runtime_error("Error! Multiple non-test entries were found. This is currently not supported. Please mark one of them as a test executable by adding \"test\": true accordingly"); //TODO: include names of exes in conflict in error message
    } else {
        main_exe = std::move(current);
        found_main = true;
    }
  }
  if(!found_main) {
      throw std::runtime_error("Error! Did not find a non-test executable. This is currently not supported");
  }
}
