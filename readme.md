# ballet
My realisation of every programmer's primal urge to implement [Verlet integration](https://en.wikipedia.org/wiki/Verlet_integration).

## Demo Compilation Instructions
### Submodules
This repo makes use of 2 submodules: [huh](https://gitlab.com/ar-be/huh) for my generic vec2 struct, and [shitrndr](https://github.com/argonberyllium/shitrndr) for rendering the demo window.  
After cloning the repo, cd into the directory and run
```
$ git submodule init
$ git submodule update
```
in your shell in order to initialise and download the latest versions of the repos.
Alternatively, just pass in the `--recurse-submodules` flag when cloning.

### Other Dependencies
Make sure your linker has access to the following libraries:
- SDL2
- SDL2_ttf


You should now be able to just build with `make`. If you prefer another compiler, such as g++, over clang++, simply replace the `CXX` variable in the makefile.

### Running
If you compiled the project without removing the `DEBUG_TEXT` flag from the source code, you'll need to supply the executable with a TrueType font, with the relative path of `./res/font.ttf` for whatever working directory you're running it from(most likely the project root or the makefile-generated `bin`). If you don't already have one on hand, I recommend [GNU unifont](https://unifoundry.com/unifont/).
