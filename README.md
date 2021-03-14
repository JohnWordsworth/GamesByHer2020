# [WIP] Space Race - an SFML engine 2D game

## Built with  
**Programming language:** C++  
**Library:** [Simple and Fast Multimedia Library (SFML)](https://www.sfml-dev.org/)
**Testing framework:** N/a

## Authors  
[Clarissa Liljander](https://github.com/clalil)  

## About this code  
This code is part of one of my three projects from the [GamesByHer](http://gamedevcourse.paradoxinteractive.com/) 2020 course. 

The additional classes built into this SFML engine with namespace __gbh__ were written by John Wordsworth and Sofie Angere from Paradox Interactive's [GamesByHer](http://gamedevcourse.paradoxinteractive.com/) course and included into the original fork boilerplate for this project.

## Dependencies  
This project can be built using [Xcode](https://developer.apple.com/xcode/) or [Visual Studio](https://visualstudio.microsoft.com/).

## Setup  
The following instructions were written by John Wordsworth in the original fork of this project boilerplate.

### macOS Setup
1. Install the latest version of Xcode
2. Launch a terminal and run 'xcode-select --install' to install the comand line tools.
3. Install **Homebrew**
4. In a terminal run, 'brew install cmake'
5. Run 'brew install vcpkg'
6. Run 'vcpkg install sfml box2d'
7. Run setup-macos.sh

#### If you run into errors
If you are getting the error ** No CMAKE_C_COMPILER could be found.**, then it means that CMake cannot determine which version of XCode you are using. The first port of call in this case is to ensure that you have the latest version of Xcode and CMake installed. If you are sure you do, then you also need to ensure that you can access your tools from the command line. To do this, launch a terminal and run the following;

* sudo xcode-select --install
* sudo xcode-select --reset
* sudo xcode-select --license

## Acknowledgement  
Thank you to Paradox Interactive's [GamesByHer](https://gamedevcourse.paradoxinteractive.com/) initiative for creating this challenge.
