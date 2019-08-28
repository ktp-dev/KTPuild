# Welcome!!
This is the documentation for KTP's build system, KTPuild. KTPuild is designed to be a simpler, faster version of **make**. Let's get started!

## Getting Started
For reference, anything in `this font` after a $ means to run everything after the $ in your terminal/command prompt. For example, `$ echo Hello World` in your terminal should print out "Hello World" on the next line.
 If you get any errors, please email nlbrow@umich.edu with a screenshot of the error, as well as any information you have as to what you were running on.
### Prerequsites
CAEN should work right out of the box. If not, please contact nlbrow@umich.edu!
* Linux (CAEN) or MacOS only. (Sorry Windows folk! We're working on it!)
* git. On Mac, you should be able to install this by running `$ git` in your terminal, which will ask you to install xcode command-line tools. Hit **Yes!**
* g++ compiler. Try running `$ g++ --version`. If it doesn't blow up, then you're good! This will also be installed on MacOS through XCode.
* Make. This should come pre-installed or with XCode. Try `$ make --version` to ensure you're good to go!. I know, I know, we need the thing we are replacing. Everyone has to start somewhere right?

### Installation steps
A reminder: anything after a '$' you should copy/paste right into your terminal/command line!

0) change into the directory where you want the code of this tool to go. Personally, I put everything in Documents, but I've known many to put things into a Code directory.
`$ cd <Directory Path here>`
(for me, I would run `$ cd ~/Documents`).

1) Download the code: `$ git clone git@github.com:ktp-dev/KTPuild.git`.
This will put the code into a folder named KTPuild in the directory you are in. If you forget where you are, you can always run `$ pwd` (which stands for **p**rint **w**orking **d**irectory) to see.

2) Compile the code. Run `$ cd KTPuild && make` (cd stands for **c**hange **d**irectory. So, this command changes into the directory with the downloaded code and runs the make command to compile everything for us).

3) To ensure everything is in working order, go ahead and run `$ ls`. This will list all of the files in our current directory. If you don't see files related to the KTPuild project, make sure you've successfully completed steps 1 & 2. The past step should have generated something called KTPuild. If you run it, it should re-compile itself. Try it out like so! `$ ./KTPuild`


## Contributing
Thank you so much for wanting to give back! Since this is a tool developed by KTP, only KTP members can write code and develop new features. If you're interested in reporting a bug or requesting a feature, please open up a new [GitHub Issue!](https://github.com/ktp-dev/KTPuild/issues/new)

You can track our progress and see what we are working on at [our Trello board](https://trello.com/b/SFRNcef1/overview-board). To request permission, please make a Trello account and email nlbrow@umich.edu your Trello username and/or email. Thanks!

Once you have access, please read the boards description. It should be availabe by visiting *Show Menu* > *About this board*. This will contain information on how to quickly parse the board and understand the workflow of the cards.

For first-time contributors, I suggest working on something labeled **Quick Win** (the light green color). If there are no Quick Wins available, feel free to partner with someone working on a Quick Win, or dive into work on one of the Epics in the left-hand column.

When you have something to work on, please make a branch for your work. You can do so by running `$ git checkout -b <Desired Branchname>`. Please name your branch something along the lines of the card so people can understand what work is being done on that branch!

Go ahead and make your desired changes! When you're done, ensure everything has been committed (`$ git add -A; $ git commit -m "PUT YOUR COMMIT MSG HERE"`), and pushed `$ git push -u`

Then, go ahead and open up a [pull request](https://github.com/ktp-dev/KTPuild/compare). Compare master on the left and your branch on the right, then press open pull request! Move your Trello Card to "In Review", and put something in the slack channel #build-system to review it!

CONGRATULATIONS!!! You did it! Woohoo! Go you! Now, feel free to do it again, or help someone new in doing it again with you!
