# hbgl


## List of GLFW releases

   [Releases](https://github.com/glfw/glfw/releases)

## Installing on Windows

- MSYS2

Before installing the packages, make sure you're running the MinGW 64-bit shell.

   ```
   pacman -S mingw-w64-x86_64-glfw
   pacman -S mingw-w64-x86_64-glew
   ```

- To build the static library, execute:

   ```
   hbmk2 hbgl.hbp
   ```

- To build and run samples and test code, navigate to the examples directory and execute:

   ```
   cd examples
   hbmk2 main.prg
   ```

## Installing on Linux

- Debian

   ```
   sudo apt-get update
   sudo apt-get install libgl-dev libglfw3-dev
   ```
- To build the static library, execute:

   ```
   hbmk2 hbgl.hbp
   ```

- To build and run samples and test code, navigate to the examples directory and execute:

   ```
   cd examples
   hbmk2 main.prg
   ```
---
