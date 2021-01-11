




## macOS Setup

1. Install the latest version of Xcode
2. Launch a terminal and run 'xcode-select --install' to install the comand line tools.
3. Install **Homebrew**
4. In a terminal run, 'brew install cmake'
5. Run 'brew install vcpkg'
6. Run 'vcpkg install sfml box2d'
7. Run setup-macos.sh


### Help!

If you are getting the error ** No CMAKE_C_COMPILER could be found.**, then it means that CMake cannot determine which version of XCode you are using. The first port of call in this case is to ensure that you have the latest version of Xcode and CMake installed. If you are sure you do, then you also need to ensure that you can access your tools from the command line. To do this, launch a terminal and run the following;

* sudo xcode-select --install
* sudo xcode-select --reset
* sudo xcode-select --license


## Assets

https://craftpix.net/freebies/free-race-track-2d-game-tile-set/