//
//  Buildfile.cpp
//  KTPuild-Jarek
//
//  Created by Nathan Brown on 4/6/19.
//  Copyright © 2019 Nathan Brown. All rights reserved.
//

#include <stdio.h>
#include "Buildfile.h"
#include <fstream>
#include "nlohmann/json.hpp"
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

  //set executable name
  executable = data["Name"];

  //set the compilation flags
  for(auto &i: data["Flags"]){
    compilation_flags.push_back(i);
  }

  //set the linker flags. For now they are the same as the compilation flags, but
  //we may want to provide the ability to override this
  linker_flags = compilation_flags;

  //set the sources
  for(auto &i: data["Sources"]){
    cpp_files.push_back(i);
  }
}




