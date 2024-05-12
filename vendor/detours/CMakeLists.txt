add_library(ms-detours-bin SHARED IMPORTED)
set_target_properties(ms-detours-bin PROPERTIES IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/detours.lib)

add_library(ms-detours INTERFACE)
target_compile_definitions(ms-detours INTERFACE _X86_)
target_include_directories(ms-detours INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(ms-detours INTERFACE ms-detours-bin)