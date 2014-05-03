#!/bin/bash

gcc -o joystick joystick.cpp `sdl-config --cflags --libs`
./joystick 169.254.1.1 23
