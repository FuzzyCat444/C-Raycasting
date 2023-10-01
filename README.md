# FuzzyCat - Raycasting in C
A simple 3D raycasting application written in C. It uses SDL in the main.c file but is modular enough to be moved to any other windowing library such as Allegro or GLFW. 

To compile this application, create a folder called "craycasting" and place the src folder, bmp files, and build.bat into the folder. Install the SDL2-2.28.3 dev library for visual studio at "C:\SDL2-2.28.3" or wherever you want, updating the build.bat file to refer to the correct library directory. Copy the SDL.dll file from "C:\SDL2-2.28.3\lib\x64" into the "craycasting" folder. Then open the "x64 Native Tools Command Prompt for VS 2022" application and run the "build.bat" file. 
