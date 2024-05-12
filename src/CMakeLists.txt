file(GLOB source *.cpp)

add_library(h5-extension SHARED ${source})
target_include_directories(h5-extension INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(h5-extension PRIVATE dll-playground-compiler_options ms-detours kiero-lib imgui)