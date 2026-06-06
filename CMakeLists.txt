cmake_minimum_required(VERSION 3.22)

project(CppEngineByHand LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_definitions(_GNU_SOURCE)

add_subdirectory(Engine)
add_subdirectory(Sandbox)
add_subdirectory(Tests)



