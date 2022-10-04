#!/bin/bash


# clang-format a directory + subdirectories
function clang-format-all() {
    dir=${1:-.} # Default: format current directory.
    all=("$@")
    rest=("${all[@]:1}")
    if [ -z "$rest" ]; then
        # Default: format current directory.
        subdirs="$dir"
    else
        # Otherwise, expand everything into one long string.
        subdirs=("${rest[@]/#/$dir/}")
    fi

    echo "Formatting $subdirs"

    blacklist='.*/(stm32.*|system_stm32|syscalls.c|sysmem.c)'

    # Format all matching files in specified dir/subdirs[@]
    clang-format -i -fallback-style=LLVM -style=file $(find $subdirs -type f -name "*.h" -o -name "*.c" -o -name "*.hpp" -o -name "*.cpp" | grep -Ev $blacklist)
}

clang-format-all Core Inc Src