# Implementation of OpenGL tutorial by theCherno on Linux


This is the implementation of [theCherno's tutorial on youtube](https://www.youtube.com/watch?v=W3gAzLwfIP0&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2
)
for Linux (Ubuntu) using CMake

So the dependencies I've installed

`$ sudo apt install libgl1-mesa-dev libglu1-mesa-dev libglfw3-dev cmake make`

Download [Glew](https://glad.dav1d.de/) as source zip in your downloads directory
The latest version at the moment is 2.1.0

Open your terminal and run

Extract the zip, go to that directory (where Makefile is present) and run `$ make`

This will build lib files in the lib directory.

What we need is `glew-2.1.0/lib/libGLEW.so.2.1.0` (version may vary in the future)

We link this library and include directory in our CMakeLists.txt

I've placed our main files in `src` directory and GLEW's `include` and `lib` directories
in `Dependencies` directory for my convenience
