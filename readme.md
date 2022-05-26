# ballet
My realisation of every programmer's primal urge to implement [Verlet integration](https://en.wikipedia.org/wiki/Verlet_integration).

## Compilation instructions
This repo makes use of 2 submodules: [huh](https://gitlab.com/ar-be/huh) for my generic vec2 struct, and [shitrndr](https://github.com/argonberyllium/shitrndr) for rendering the demo window.  
After cloning the repo, cd into the directory and run:
```
$ git submodule init
$ git submodule update
```
in your shell in order to initialise and download the latest versions of the repos.
Alternatively, just pass in the `--recurse-submodules` flag when cloning.

Assuming you already have SDL2 in your `PATH`, you should now be able to just build with `make`. If you prefer g++ over clang++, simply replace the `CXX` variable.
