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
<<<<<<< HEAD
=======
#include <iostream>
#include <stdexcept>
>>>>>>> a bit late but its still here
=======
>>>>>>> a9e3d9a1b0e0eaecf23b271c88f9dcfdea9dfd41

using namespace std;
using namespace nlohmann;

<<<<<<< HEAD
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
=======
Buildfile :: Buildfile(){
  json data;
  ifstream fs("Buildfile.json");
>>>>>>> a9e3d9a1b0e0eaecf23b271c88f9dcfdea9dfd41
  fs >> data;

  //set executable name
  executable = data["Name"];
  //set the compilation flags
<<<<<<< HEAD
<<<<<<< HEAD
  for(auto &i: data["Flags"]){
    compilation_flags.push_back(i);
=======
  for (auto &i: data["Flags"]) {
      compilation_flags.push_back(i);
>>>>>>> a bit late but its still here
=======
  for(auto &i: data["Flags"]){
    compilation_flags.push_back(i);
>>>>>>> a9e3d9a1b0e0eaecf23b271c88f9dcfdea9dfd41
  }
  //set teh linker flags
  linker_flags = compilation_flags;

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> a9e3d9a1b0e0eaecf23b271c88f9dcfdea9dfd41
  for(auto &i: data["Sources"]){
    cpp_files.push_back(i);
  }

<<<<<<< HEAD
=======
  for (auto &i: data["Sources"]) {
      cpp_files.push_back(i);
  }


>>>>>>> a bit late but its still here
=======
>>>>>>> a9e3d9a1b0e0eaecf23b271c88f9dcfdea9dfd41
}




