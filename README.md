# stm32-midi-keyboard
Midi keyboard project for ELEC3300, a course on embedded systems, implemented using STM32 (F4) with C++20.

![Picture of the keyboard.](/docs/thumbnail.jpg)

## Features

* 2+ octaves (29) piano keys to flexibly play various melodies
    * Supports multi-press, so that we aren't stuck with boring one-note tunes and can play chords
* Volume control, so that we don’t disturb our neighbours
* Metronome, in case the user can’t keep track of tempo
* TFT display and menu selection
* Record and playback music (supports multiple channels!)
* Load/store MIDI in flash memory
* Send MIDI signals through USB (UART)
* Extra fanciful features:
    * Transpose: shift pitches up/down on the diatonic scale
    * Auto-Chord: quality-of-life function to play octaves, triads, and open triads by just pressing the root key
    * Instruments: supports playback of sine, triangle, square, and sawtooth signals

## Development 
### Environment
Download all of the following before continuing. Preferably, use a package manager to install these.

- [arm-none-eabi-gcc](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads) for compiling (make sure you use the lastest version)
- [OpenOCD](https://github.com/xpack-dev-tools/openocd-xpack/releases/) for flashing and debugging
- [CMake](https://cmake.org/download/) for the build system
- [Ninja](https://ninja-build.org/) or some kind of Make
	- Ninja is faster and easier to install on windows
	- Mac/Linux can just use builtin Make easily

#### Package Manager: Use Brew on Mac
https://brew.sh/

```
brew install --cask gcc-arm-embedded
brew install ninja
brew install cmake
brew install open-ocd
```

#### Package Manager: Use Anaconda

https://anaconda.org/
While Anaconda is usually for handling Python packages you can find many. You may need this anyway

```
conda install -c conda-forge cmake ninja 
conda install -c litex-hub openocd # max/linux
conda install -c m-labs openocd # windows/linux
conda install -c dhgoldberg gcc-arm-none-eabi
```

#### Package Manager: Use Chocolatey on Windows

https://chocolatey.org/

```
choco install gcc-arm-embedded
choco install openocd
choco install ninja
choco install cmake
```

### Building and Flashing:
In the command palette
- type `build` and find `CMake: Build`, remember the keyboard shortcut shown on the right for future use
- use `CMake: Build Target` and choose `flash`
- use `CMake: Set Build Target` to choose the default build target
    - this would make `CMake: Build` run that target
    - the default default build target is `all` which just compiles

### IDE / VSCode

Recommended plugins for VSCode:

* C/C++ by Microsoft
* CMake Tools by Microsoft
* Clang-Format by xaver
* Cortex-Debug by marus25

### Formatting

ClangFormat is used for formatting.

For unix-like systems, we recommend installing a pre-commit hook so that you don't forget to format (because forgetting is _that_ easy).

```sh
# Install git-format-staged.
npm install --global git-format-staged

# Add pre-commit hook.
mkdir -p .git/hooks
cp pre-commit .git/hooks/
chmod u+x .git/hooks/pre-commit
```
