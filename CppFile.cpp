//
//  CppFile.cpp
//  KTPuild-Jarek
//
//  Created by Nathan Brown on 4/6/19.
//  Copyright © 2019 Nathan Brown. All rights reserved.
//

#include <stdio.h>
#include "CppFile.h"

bool has_changed(std::string filename){
  return true;
}
std::string convert_to_obj_file(std::string cpp_file){
  auto it = cpp_file.find_last_of(".");

  cpp_file.replace(it, 4, ".o");

  return cpp_file;

}
