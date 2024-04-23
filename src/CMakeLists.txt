file(GLOB source *.cpp)

add_library(dll-playground-lib ${source})
target_include_directories(dll-playground-lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(dll-playground-lib PRIVATE dll-playground-compiler_options)