#include "Buildfile.h"
#include "CppFile.h"
#include <iostream>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

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
        if (changed.empty())
            return 0; //3
        for (string &filename : changed) {
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

            if (pid_t id = fork()) {
                int status;
                waitpid(id, &status, 0); //block until child finishes
                //we are parent
                if (!WIFEXITED(status) || WEXITSTATUS(status)) {
                    return -1;
                }
            } else {
                for (auto &f : command) {
                    cout << f << " ";
                }
                cout << endl;
                vector<char *> exec_args;
                for (auto &f : command) {
                    exec_args.push_back((char *) f.c_str());
                }
                exec_args.push_back(nullptr);
                if (execvp("g++", exec_args.data())) {
                    return -1; //exec errored, so we error
                }
                //we are child
                //we call exec with the null terminated list of c strings as arguments
                //first arg is "g++" aka name of program
                //oh shoot, we made this already. It's command.
            }
        }
        //step 6 DONE
        //g++ [LINKING FLAGS] -o output [OBJECT FILES]
        vector<string> command;
        command.push_back("g++");
        for (auto &i: buildfile.get_linker_flags()) {
            command.push_back(i);
        }
        command.push_back("-o");
        command.push_back(buildfile.get_executable_name());
        for (auto &i: cpp_filename) {
            command.push_back(convert_to_obj_file(i));
        }
        if (pid_t id = fork()) {
            int status;
            waitpid(id, &status, 0); //block until child finishes
            //we are parent
        } else {
            for(auto& f : command) {
                cout << f << " ";
            }
            cout << endl;
            vector<char*> exec_args;
            for(auto& f : command) {
                exec_args.push_back((char*)f.c_str());
            }
            exec_args.push_back(nullptr);
            if(execvp("g++", exec_args.data())) {
                return -1; //exec errored, so we error
            }
        }
    }
    catch(std::exception &err) {
        std::cerr << err.what();
        return -1;
    }
}
