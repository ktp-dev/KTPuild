//
//  CppFile.cpp
//  KTPuild-Jarek
//
//  Created by Nathan Brown on 4/6/19.
//  Copyright © 2019 Nathan Brown. All rights reserved.
//

#include <cstdio>
#include "CppFile.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cassert>
#include <ctime>



struct timespec getTimeSpec(const std::string &filename) {
    struct stat sb;

    //see if the filename exists
    if (stat(filename.c_str(), &sb) == -1) {
        //if not, construct a default one. 0, 0 represents 0 seconds since the epoch
        struct timespec currTime = {0, 0};
        return currTime;
    }

    struct timespec timeStampCpp;
    //MacOS and Linux differ in the name of the timespec struct that contains the last modified time. Use #ifdef to only compile the correct line for a given platform (Mac or Linux)
#ifdef __APPLE__
    timeStampCpp = sb.st_mtimespec;
#else
    timeStampCpp = sb.st_mtim;
#endif
    return timeStampCpp;
}

//Returns true if the lhs is older than the rhs side
bool operator<(struct timespec lhs, struct timespec rhs) {
    if (lhs.tv_sec == rhs.tv_sec)
        return lhs.tv_nsec < rhs.tv_nsec;
    else
        return lhs.tv_sec < rhs.tv_sec;
}

bool has_changed(const std::string& filename){
  struct timespec cppTime = getTimeSpec(filename);
  std::string objFilename = convert_to_obj_file(filename);
  struct timespec objTime = getTimeSpec(objFilename);

  return objTime < cppTime;
}

std::string convert_to_obj_file(const std::string& cpp_file){
    size_t len = cpp_file.find_last_of("."); //don't know the file extension. Can be ".c", ".cc", ".cpp", or something else. Not safe to assume it is ".cpp"
    std::string obj_file{cpp_file.begin(), cpp_file.begin() + len}; //make a copy of the filename up to but not including the "." character
    obj_file += ".o";
    return obj_file;
}
