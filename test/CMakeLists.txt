add_executable(tests test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain dll-playground-compiler_options h5-extension)

include(Catch)
catch_discover_tests(tests)