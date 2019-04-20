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
<<<<<<< HEAD
=======
#include <iostream>
#include <stdexcept>
>>>>>>> a bit late but its still here

using namespace std;
using namespace nlohmann;

<<<<<<< HEAD
Buildfile :: Buildfile(){
  json data;
  ifstream fs("Buildfile.json");
=======

Buildfile :: Buildfile(){
  json data;
  ifstream fs("Buildfile.json");
  if (!fs.is_open()) {
    std::cerr << "Missing build file, -10 KTPoints\n";
    throw(std::runtime_error("no buildfile"));
  }


>>>>>>> a bit late but its still here
  fs >> data;

  //set executable name
  executable = data["Name"];
  //set the compilation flags
<<<<<<< HEAD
  for(auto &i: data["Flags"]){
    compilation_flags.push_back(i);
=======
  for (auto &i: data["Flags"]) {
      compilation_flags.push_back(i);
>>>>>>> a bit late but its still here
  }
  //set teh linker flags
  linker_flags = compilation_flags;

<<<<<<< HEAD
  for(auto &i: data["Sources"]){
    cpp_files.push_back(i);
  }

=======
  for (auto &i: data["Sources"]) {
      cpp_files.push_back(i);
  }


>>>>>>> a bit late but its still here
}




