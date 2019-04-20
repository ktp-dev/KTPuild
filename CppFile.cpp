//
//  CppFile.cpp
//  KTPuild-Jarek
//
//  Created by Nathan Brown on 4/6/19.
//  Copyright © 2019 Nathan Brown. All rights reserved.
//

<<<<<<< HEAD
#include <stdio.h>
#include "CppFile.h"

bool has_changed(std::string filename){
  return true;
}
=======

#include <stdio.h>
#include "CppFile.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cassert>
#include <ctime>


bool TSComp(timespec cpp, timespec obj) {
    //Returns true if last obj change happened before last cpp change.
    if (cpp.tv_sec == obj.tv_sec)
        return cpp.tv_nsec > obj.tv_nsec;
    else
        return cpp.tv_sec > obj.tv_sec;
}


struct timespec getTimeSpec(const std::string &filename) {
    struct stat sb;

    if (stat(filename.c_str(), &sb) == -1) {
        struct timespec currTime;
        currTime = {0, 0};
        return currTime;
    }

    struct timespec timeStampCpp;
#ifdef __APPLE__
    timeStampCpp = sb.st_mtimespec;
#else
    timeStampCpp = sb.st_mtim;
#endif
    return timeStampCpp;
}

bool has_changed(std::string filename){
  struct timespec cppTime = getTimeSpec(filename);
  std::string objFilename = convert_to_obj_file(filename);
  struct timespec objTime = getTimeSpec(objFilename);

  return TSComp(cppTime, objTime);
}

>>>>>>> a bit late but its still here
std::string convert_to_obj_file(std::string cpp_file){
  auto it = cpp_file.find_last_of(".");

  cpp_file.replace(it, 4, ".o");

  return cpp_file;

}
