cmake_minimum_required(VERSION 3.26)
project(dll_playground)

option(DLL_PLAYGROUND_WARNINGS_AS_ERRORS "Treat compiler warnings as errors")


add_library(dll-playground-compiler_options INTERFACE)
target_compile_features(dll-playground-compiler_options INTERFACE cxx_std_20)
target_compile_options(dll-playground-compiler_options INTERFACE /W4)
if (DLL_PLAYGROUND_WARNINGS_AS_ERRORS)
  target_compile_options(dll-playground-compiler_options INTERFACE /WX)
endif ()

add_subdirectory(src)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(FetchContent)

  FetchContent_Declare(
      Catch2
      GIT_REPOSITORY https://github.com/catchorg/Catch2.git
      GIT_TAG v3.4.0
      GIT_SHALLOW TRUE
  )
  FetchContent_MakeAvailable(Catch2)

  enable_testing()
  add_subdirectory(test)

  include(CTest)
endif ()