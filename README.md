urov
====

## To Do
1. Figure out how client.c, joystick.cpp, and boardcode.ino are communicating with one another
2. Fix the hang-up that happens around line 78 in joystick.cpp
3. Control the LED's on the arduino with the controller

## Installation & Usage
First, install SDL.

- on Linux: `sudo apt-get install libsdl1.2-dev`
- on Mac: install [Homebrew](https://github.com/Homebrew/homebrew/wiki/installation), then `brew install sdl` (you may need to install an older verison)
- on Windows: install SDL via https://www.libsdl.org/download-1.2.php

To compile and run joystick.cpp on Linux and Mac, use these executables

    ./compile.sh
    ./run.sh

To compile on Windows, use a compiler such as [MinGW](http://www.mingw.org/) or Visual Studio. Use these arguments when compiling:

    gcc -o joystick joystick.cpp `sdl-config --cflags --libs`
