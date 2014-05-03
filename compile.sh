#!/bin/bash

gcc -o joystick joystick.cpp `sdl-config --cflags --libs`
