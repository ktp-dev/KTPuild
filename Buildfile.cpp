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

using namespace std;
using namespace nlohmann;

Buildfile :: Buildfile(){
  json data;
  ifstream fs("Buildfile.json");
  fs >> data;

  //set executable name
  executable = data["Name"];
  //set the compilation flags
  for(auto &i: data["Flags"]){
    compilation_flags.push_back(i);
  }
  //set teh linker flags
  linker_flags = compilation_flags;

  for(auto &i: data["Sources"]){
    cpp_files.push_back(i);
  }

}




