cmake_minimum_required(VERSION 3.0.0)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build Type (debug or release)" FORCE)
endif()

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
message("> ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
message(">> ${CMAKE_TOOLCHAIN_FILE}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/game/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/game/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/game/bin")


## SpaceRace Project


project(SpaceRace)
set(EXECUTABLE "SpaceRace")

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "source/*.cpp")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "source/*.h")

add_executable(${EXECUTABLE} ${SOURCE_FILES} ${HEADER_FILES})
set_property(TARGET ${EXECUTABLE} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/game/bin")



## Games By Her SFML-Engine

add_subdirectory("sfml-engine")
target_link_libraries(${EXECUTABLE} PRIVATE "sfml-engine")