# stm32-midi-keyboard
Midi keyboard project for ELEC3300, a course on embedded systems, implemented using STM32 with C++20.

## Environment
Download all of these before continuing:

- [arm-none-eabi-gcc](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads) for compiling (make sure you use the lastest version)
- [OpenOCD](https://github.com/xpack-dev-tools/openocd-xpack/releases/) for flashing and debugging
- [CMake](https://cmake.org/download/) for the build system
- [Ninja](https://ninja-build.org/) or some kind of Make
	- Ninja is faster and easier to install on windows
	- Mac/Linux can just use builtin Make easily

### Package Manager: Use Brew on Mac
https://brew.sh/

```
brew install --cask gcc-arm-embedded
brew install ninja
brew install cmake
brew install open-ocd
```

### Package Manager: Use Anaconda

https://anaconda.org/
While Anaconda is usually for handling Python packages you can find many. You may need this anyway

```
conda install -c conda-forge cmake ninja 
conda install -c litex-hub openocd # max/linux
conda install -c m-labs openocd # windows/linux
conda install -c dhgoldberg gcc-arm-none-eabi
```

### Package Manager: Use Chocolatey on Windows

https://chocolatey.org/

```
choco install gcc-arm-embedded
choco install openocd
choco install ninja
choco install cmake
```

## Building and Flashing:
In the command palette
- type `build` and find `CMake: Build`, remember the keyboard shortcut shown on the right for future use
- use `CMake: Build Target` and choose `flash`
- use `CMake: Set Build Target` to choose the default build target
    - this would make `CMake: Build` run that target
    - the default default build target is `all` which just compiles

## VSCode

Recommended plugins for VSCode:

* C/C++ by Microsoft
* CMake Tools by Microsoft
* Clang-Format by xaver
* Cortex-Debug by marus25
