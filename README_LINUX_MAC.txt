How to compile:
=============
To compile Framework use provided CMake script

To compile game u should use next command in directory Project:

g++ game.cpp -I ../Framework -lFrameworkRelease -L../Framework -lSDL2 -lSDL2_image

to run executable you need to set PATH variable

export LD_LIBRARY_PATH=/home/user/wayToFramework/Framework

launch ./a.out

if you need to install SDL library use next commad:
sudo apt install libsdl2-image-dev

Notes:
=====
You should not do any changes in launch.cpp/Framework.h as part of a test task.
There is a sample project "Project2", you can modify it or create your own.

