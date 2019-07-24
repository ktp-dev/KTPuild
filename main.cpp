#include "Buildfile.h"
#include "CppFile.h"
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;
const int MAX_PROCESSES = 4;

vector<char*> print_and_convert(const vector<string>& args) {
    vector<char*> result;
    for (auto &f : args) {
        cout << f << " ";
        result.push_back(const_cast<char*>(f.c_str()));
    }
    cout << endl;
    result.push_back(nullptr);
    return result;
}

int wait_or_throw(const std::string& msg) {
    int status;
    int ret = wait(&status);
    if (!WIFEXITED(status) || WEXITSTATUS(status))
        throw std::runtime_error(msg);
    return ret;
}

void compile_object_files(const Buildfile& buildfile, const vector<string>& changed_files) {
    int num_processes = 0;
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

            //we have to construct the g++ compilation command
            //g++ [FLAGS] -c [CPP FILENAME] -o [OBJ FILENAME]
            vector<string> command;
            command.push_back("g++");
            for (string &flag : buildfile.get_compilation_flags()) {
                command.push_back(flag);
            }
            command.push_back("-o");
            command.push_back(obj_file);
            command.push_back("-c");
            command.push_back(filename);
            vector<char *> exec_args = print_and_convert(command);
            execvp(exec_args[0], exec_args.data());
            throw std::runtime_error("Compilation failed. Stopping");
        }
    }
    //wait for all compiles underway to finish
    while (wait_or_throw("Compilation failed. Stopping")) ;
}

void link_executable(const Buildfile& buildfile, const vector<string>& cpp_files) {
    //g++ [LINKING FLAGS] -o output [OBJECT FILES]
    vector<string> command;
    command.push_back("g++");
    for (auto &i: buildfile.get_linker_flags()) {
        command.push_back(i);
    }
    command.push_back("-o");
    command.push_back(buildfile.get_executable_name());
    for (auto &i: cpp_files) {
        command.push_back(convert_to_obj_file(i));
    }
    if (pid_t id = fork()) {
        int status;
        waitpid(id, &status, 0); //block until child finishes
        //we are parent
    } else {
        vector<char*> exec_args = print_and_convert(command);
        execvp(exec_args[0], exec_args.data());
        throw std::runtime_error("Could not execute linker");
    }
}

int main() {
    /*
     * 1) get the cpp files
     * 2) determine what cpp files to recompile
     *      - need to recompile if no obj file we have to create it, or if there is an obj file,
     *      but the cpp file has been modified more recently than the obj file
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
        //now we know what cpp files have changed
        if (changed.empty()) //TODO: also ensure exe exists
            return 0; //3

        compile_object_files(buildfile, changed);
        link_executable(buildfile, cpp_filename);

        //step 6 DONE
    }
    catch(std::exception &err) {
        std::cerr << err.what();
        return -1;
    }
}
