# RTYPE

## Description

RTYPE is a 2D shoot'em up game in C++<br>
The player joins a team of 4 spaceships and has to fight enemies in order to stay alive.<br>
The last survivor wins the game.

## Prerequisites

- [CMake](https://cmake.org/download/)

- [Visual Studio 2019](https://visualstudio.microsoft.com/fr/downloads/)

##  R-TYPE Setup WINDOWS and LINUX

### Clone the repository

```bash
git clone git@github.com:SloWayyy/r-type.git
```

### Build the project

#### Windows


```bash
cd r-type
cmake.exe -B build
cmake.exe --build build
cd build/Debug
```

#### Linux

```bash
cd r-type
cmake -B build
cmake --build build
cd build/
```

### Launch the project

#### Server

```bash
./r-type_server [port] [ip]
```

<table>
  <tr>
    <th>Argument</th>
    <th>Type</th>
    <th>Description</th>
  </tr>
  <tr>
    <td>port</td>
    <td>Int</td>
    <td>Port TCP on which the server will listen</td>
  </tr>
  <tr>
    <td>ip</td>
    <td>String</td>
    <td>Ip address of the server</td>
  </tr>
</table>


#### Client

```bash
./r-type_client port ip
```

<table>
  <tr>
    <th>Argument</th>
    <th>Type</th>
    <th>Description</th>
  </tr>
  <tr>
    <td>port</td>
    <td>Int</td>
    <td>Port TCP on which the client will connect</td>
  </tr>
  <tr>
    <td>ip</td>
    <td>String</td>
    <td>Ip address of the server</td>
  </tr>

</table>

## Internal documentation

- [Server](network/Readme.md)

- [Game Engine](ecs/Readme.md)

- [Client](client/Readme.md)

## Code formatter

  We used clang-format to format our code. You can find the configuration file in the root of the project.

- [Clang-format](https://www.webkit.org/code-style-guidelines/)

 This is how we use it on linux:
  ```bash
  clang-format -i -style=file <file>
  ```

  or for all files in a directory :
  ```bash
  find server -name '*.cpp' -exec clang-format -i {} \;
  ```

## Authors

| [<img src="https://github.com/kvn703.png?size=85" width=85><br><sub>Kevin NADARAJAH</sub>](https://github.com/kvn703) | [<img src="https://github.com/SloWayyy.png?size=85" width=85><br><sub>Mehdi Djendar</sub>](https://github.com/SloWayyy) | [<img src="https://github.com/Sloyi.png?size=85" width=85><br><sub>Rayan Es-Seffar</sub>](https://github.com/Sloyi) | [<img src="https://github.com/JonathanYakan.png?size=85" width=85><br><sub>Jonathan Yakan</sub>](https://github.com/JonathanYakan) | [<img src="https://github.com/Tahalani.png?size=85" width=85><br><sub>Taha ALANI</sub>](https://github.com/Tahalani) |
|:-----------------------------------------------------------------------------------------------------------------------:|:-----------------------------------------------------------------------------------------------------------------------:|:-------------------------------------------------------------------------------------------------------------------:|:----------------------------------------------------------------------------------------------------------------------------------:|:--------------------------------------------------------------------------------------------------------------:|