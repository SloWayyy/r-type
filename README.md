# RTYPE

## Description

RTYPE is a 2D shoot'em up game. The player controls a spaceship and must fight against enemies and bosses.

## Installation PRE-REQUISITES

### INSTALL VS CODE

(https://code.visualstudio.com/download)

### INSTALL CMAKE

(https://cmake.org/download/)

### INSTALL GIT BASH

(https://git-scm.com/downloads)

### DEPENDENCIES

### INSTALL VCPKG

(https://vcpkg.io/en/getting-started)

#### LINUX

SET ENVIRONMENT VARIABLE

```bash
export CMAKE_TOOLCHAIN_FILE="[path to vcpkg]/scripts/buildsystems/vcpkg.cmake"
```

#### WINDOWS

SET ENVIRONMENT VARIABLE
```
WINDOWS > VARIABLES ENVIRONMENT > NEW > 
NAME: "CMAKE_TOOLCHAIN_FILE" 
VALUE: "[path to vcpkg]/scripts/buildsystems/vcpkg.cmake"
```

### COMPILATION

#### LINUX

```bash
cmake -B build
cmake --build build
```

The binary is in the build directory.

#### WINDOWS


```bash
cmake.exe -B build
cmake.exe --build build
```
The binary is in the build/Debug directory.


## RUN


