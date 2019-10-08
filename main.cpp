#include "Buildfile.h"
#include "CppFile.h"
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;
const int MAX_PROCESSES = 4;
const char* const COMPILER = "g++";


void run_command(const vector<string>& command) {
    //To run a command, we have to call execvp. execvp expects a char** (C style array) that is null terminated. Thus, convert our nice C++ array of strings to a C array of C strings. Also, print them out so the user knows what we are running.
    vector<char*> command_cstrs;
    for (auto &f : command) {
        cout << f << " ";
        command_cstrs.push_back(const_cast<char*>(f.c_str()));
    }
    cout << endl;
    command_cstrs.push_back(nullptr);
    execvp(command_cstrs[0], command_cstrs.data());
    throw std::runtime_error("Compilation failed. Stopping");
}

// Returns the PID we just waited on, or 0 if there are no more children
int wait_or_throw(const std::string& msg) {
    int status;
    int ret = wait(&status);
    if(ret == -1 && errno == ECHILD) {
        //No more children. return 0
        return 0;
    }
    if (!WIFEXITED(status) || WEXITSTATUS(status)) {
        std::cerr << WIFEXITED(status) << " " << WEXITSTATUS(status) << std::endl;
        throw std::runtime_error(msg);
    }
    return ret;
}

void compile_object_files(const Buildfile& buildfile, const vector<string>& changed_files) {
    int num_processes = 0;
    //Construct the g++ compilation command
    //g++ [FLAGS] -o [OBJ FILENAME] -c [CPP FILENAME]
    //Can construct the g++ [FLAGS] section now- it is the same for every file
    vector<string> command;
    command.push_back(COMPILER);
    for (string &flag : buildfile.get_compilation_flags()) {
        command.push_back(flag);
    }
    for (const string &filename : changed_files) {
        if (fork()) {
            //we are parent responsible for checking on the progress of the multiple compiles in-flight
            ++num_processes;
            if (num_processes == MAX_PROCESSES) {
                wait_or_throw("Compilation failed. Stopping");
                --num_processes;
            }
        } else {
            //we are the child, responsible for running the command
            //for every changed file, issue g++ command
            string obj_file = convert_to_obj_file(filename);
            command.push_back("-o");
            command.push_back(obj_file);
            command.push_back("-c");
            command.push_back(filename);
            run_command(command);
        }
    }
    //wait for all compiles underway to finish
    while (wait_or_throw("Compilation failed. Stopping")) ;
}

void link_executable(const Buildfile& buildfile, const vector<string>& cpp_files) {
    //g++ [LINKING FLAGS] -o output [OBJECT FILES]
    vector<string> command;
    command.push_back(COMPILER);
    for (const string &flag: buildfile.get_linker_flags()) {
        command.push_back(flag);
    }
    command.push_back("-o");
    command.push_back(buildfile.get_executable_name());
    for (const string &cpp_file: cpp_files) {
        command.push_back(convert_to_obj_file(cpp_file));
    }
    if (pid_t id = fork()) {
        //we are parent
        wait_or_throw("Linking failed");
    } else {
        //child, executes the command
        run_command(command);
    }
}

bool is_created(const string filename){

    // tests for the existence of the file with success being a 0
    if(access(filename.c_str(), F_OK) == 0)
    {
        return true;
    }
    return false;
}

int main() {
    /*
     * 1) get the cpp files
     * 2) determine what files to recompile
     *      - need to recompile if no obj file we have to create it, or if there is an obj file,
     *      but the cpp file has been modified more recently than the obj file
     *      - need to check to see if there is an executable created
     * 3) if none, we are done
     * 4) else, recompile them (done!)
     * 5) relink everything
     * 6) done!
     */
    try {
        Buildfile buildfile;
        vector<string> cpp_filename = buildfile.get_cpp_files(); //1
        vector<string> changed;
        for (string &file : cpp_filename) { //2
            if (has_changed(file))
                changed.push_back(file);
        }
        // if there is nothing to reompile and the executable has been created we are done
        if (changed.empty() && is_created(Buildfile.executable))
            return 0; //3

        compile_object_files(buildfile, changed); //4
        link_executable(buildfile, cpp_filename); //4

        //step 6 DONE
    }
    catch(std::exception &err) {
        std::cerr << err.what();
        return -1;
    }
}
