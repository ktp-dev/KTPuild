# Overview of the codebase
This document is meant to serve as a guide to the codebase and underlying concepts used. It's meant to be helpful for FAQ's and help new members understand what code does and why it is structured the way that it is.


## What are we building
Our tool will be used to replace `make` in school/small projects. It's meant to be more intuitive and easier to use, while hopefully providing for more features out of the box.

Ultimately, our tool has one main job: build an executable for a school project so a student can run their code. Let's see how that's done.

## Building an executable in C++
C++ has two main stages for compilation. First is the compilation stage.

### Compilation Stage
 Notably, the compilation phase does not produce an executable, and all the times we've said we "compiled" to an executable, we really should have said we "compiled and linked" to an executable. The compilation stage does a few things, but most importantly, it transforms a single .cpp file into a single .o (object) file. Object files are just already-compiled C++ code. Feel free to ask anyone who has taken 370 a bit about what goes into them! One or more object files are fed into a linker to produce an executable.

As an idea of what work is done here, the compiler will provide errors such as type mismatches or calling unknown functions.

### Linking Stage
After each .cpp file is compiled into an object (.o) file, all of the object files need to be combined into a single executable. The linker will take the information put into all of the object files and stich it together into one executable.

The linker is what puts in the hard work to make code that calls functions not defined in the same .cpp "just work". As an idea, this is where you can face errors like duplicate definitions or no definitions.

## Why do we have two steps?
Good question! Let's see what we have to do if it were just one step.


When compiling our own C++ projects (without using make), we have to type in a long command that looks something like 

`g++ file1.cpp file2.cpp -o myexe.exe`. 

This command runs g++, a c++ compiler, and tells g++ to go straight from .cpp files to an executable (which is why it is so common to say we compiled to an executable). Under the hood, g++ builds object files and then links them, but hides that work from us. 

Now, let's say we make a change to file2.cpp. When we re-run this command, g++ has to do the hard work of re-compiling file1.cpp. This may not seem like a big deal for one or two or even twenty additional files. What if it were 1,000? 10,000? All of that unnecessary work takes time and money (computing isn't free!). Object files allow us to only do that hard work of compiling a .cpp file once (until our .cpp file changes). Build systems (like this one!) automate the process of determining which .cpp files have changed, and perform the minimal work necessary to update the executable. Without them, either we would have to always recompile every file, or we would have to keep track of which files have changed since we last compiled. What a mess!


# How do we determine what to do in order to update the executable (and then do it)?
Well, let's see! We can start with what happens in `main` and determine what work is done where (and when)!

`Buildfile buildfile;`

The first thing we do in main is construct an instance of the Buildfile class.
## Buildfile
The Buildfile class represents what information is provided to us by the Buildfile.json file the user provides. Looking in Buildfile.h, we can see the Buildfile class holds a vector of C++ files, a vector of compilation flags, and a vector of linking flags in addition to the name of the executable to produce. Let's see how these are filled in in the constructor!

### Buildfile Constructor
Opening `Buildfile.cpp` we can see the Buildfile constructor. First, we define an object of the `json` type. This type comes from our [JSON parsing library](https://github.com/nlohmann/json). We don't have to worry about how it works- just how to use it!.
Next, we open up an `ifstream` to read the `Buildfile.json` file, and ensure that the stream is open (aka that the file exists). Then, we parse the JSON file, read out the name, flags, and sources keys. The name flag corresponds to the name of the executable to generate, the flags key corresponds to both compiling and linking flags, and the sources flag corresponds to what .cpp files to compile. That's it! That's all this does- parse the Buildfile.json into C++ types (vectors and strings).

## Determining what work to do
After parsing user input, we have to do something with it. The first thing to do is to determine what .cpp files have changed so that we can recompile their object files. This check is done in the `has_changed` function. Let's see how it works!

### Determining if a .cpp file has changed
The first thing we do is get a timespec for the provided filename. A timespec is a simple struct defined by the operating system that represents a point in time. The timespec we are interested in is the time a file was last modified. We can compare these last modified times for a .cpp and a .o to determine if a .cpp has been touched since it was used to create its object file.

To get this last modified timestamp, we call a function called `stat`. This function is provided to us by the operating system, and it fills in a struct called stat (I know, kinda weird, but also the `stat` call provides the info in `struct stat`). This struct holds a ton of information about the file- things such as who owns it, when was it created, how big is it, etc. All we care about is copying out the timespec the file was last modified. 

When getting the time last modified for an object file, we have to be careful- what if the object file doesn't exist yet? In that case, the `stat` call will return -1, and we provide the time last modified as the Unix Epoc, aka 12:00 am January 1st, 1970. This time is also commonly represented as 0.

After doing these timespec comparisions between every .cpp and its .o, we can tell what .cpp files have to be recompiled. Let's see how we can do that!


## Compiling all changed .cpp files into .o files
This is where things start to get really technical, but that's okay! Feel free to ask people questions if things don't make sense. Before diving into the code, let's chat about what a "process" is.

A process is an executing program. That's all it is. When you start running an executable, you "spawn a process." When you open up your web browser, you "spawn a process." Granted, it likely spawns more behind the scenes (kinda like what we are about to do), but spawning a process and starting a program mean the same thing. However, it is hard for computers to do a lot of things at once. Open ~100 tabs in Chrome all streaming something and you'll see what I mean. To prevent our code from spawning too many processes, we limit ourselves to how many spawned processes we can have at once.

Okay, back to the code. First, we declare how many processes we have spawned (0). Then, we build up what command we need to run in order to compile a .cpp file. First, we push back what compiler we are using (g++) and all compilation flags (things like "-Wall or -Werror"- if you don't know anything about these, don't worry!). Then for each changed file, we call `fork`. `fork` is how Linux/Mac spawns a new process (AKA launches a new application). Which application? Well, our application! I know it sounds weird, but we'll see how we can go from re-running ourselves to being able to run any other application. When another process is spawned running our application, that process doesn't start at `main`. In fact, it starts by returning from `fork`. So, when we call `fork` as one process, there are two returns- one in each process! The only difference between the two processes are in return values. If your process was the original caller of `fork` then the return value is a non-zero ID number of the process that was created. If your process was the one created during `fork`, then your return value is 0. So, when you see a statement like ```if(fork()) {
//some code
} else { 
//other code
}``` both branches of the if/else are executed. Why? Because `fork` returns twice- once for each process, and one return value is non-zero and executes the `if` block, and the other return is 0, and executes the `else` block. Keep in mind, both of these are running in different processes, which means they don't really interact unless you code them to do so. Said differently, things that happen in one process don't impact another process unless it is supposed to. So, when both branches of the `if/else` block execute, they do so independently. 
Inside the `if` block, the parent has to decide if we are going to keep spawning processes, or if we should wait for a process to finish to first. Remember, spawning processes puts a strain on the computer- we don't want to ask too much of the computer, otherwise we will slow down. Let's see what happens when we've spawned as many processes as we feel comfortable with.

`wait_or_throw` calls a function called `wait`. `wait` will block our code from executing (much like reading in user input from `cin` blocks until the user has typed something) until one of the processes we have spawned exits. We then check if we actually waited for a child process, or if the process we waited for exited due to an error. If there was a child that just exited with error, we propogate that error in the form of a C++ exception.

Back to compiling things. So, the main process comes along, spawns `MAX_PROCESSES` number of children, and then waits and keeps spawning them until all of the changed C++ files have been recompiled. That's all it does. All of the actual compilation work is done in the child processes (the ones that are created inside `fork`, return 0 and run the else block). Let's see what they do!

In the `else` block of the `if(fork()) { //stuff } else { //what we want to see }`, we see that we add on things to the command initalized a bit back. By the time we get to `run_command`, the `vector<string> command` looks something like: `g++ FLAGS -o NAME.o -c NAME.cpp`. We have spawned another processes running ourselves already, but we don't want to write a C++ compiler! Let's see how we can have g++ do the work of compiling this code instead of writing that ourselves.

In `run_command`, the function takes a `vector<string>` that is transforms into a `vector<char*>`. The main thing we care about is the call to `execvp`. `execvp` replaces the application running in the calling process with another application. Note, it does NOT spawn a new process (that's what `fork` does). Because it starts running another process, it should not return (just like a call to `assert(false)` won't return!). So with this, we can see the pattern for running another process running another application (like g++!): First, we `fork` to spawn another process. Then, for the process that was just spawned, we call an `exec` function (in this case it is `execvpe`) to swap us for a different application in the new process.

So, now that we know how to spawn a new process and have that process compile a .cpp file to a .o file, the rest of this is just doing the same thing one more time to go from all of the .o files to the executable (perform the linking stage).


# The End
That's it! That's all there is to it. Now go forth and innovate! Feel free to edit me to include all of the awesome features and fixes you make! :D Happy Hacking!
