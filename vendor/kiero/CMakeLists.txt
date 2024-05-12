include(FetchContent)
FetchContent_Declare(
    kiero
    GIT_REPOSITORY https://github.com/Rebzzel/kiero.git
    GIT_TAG 177c3cf43a1b1be2651dba2a1acb3b334899ead7
    PATCH_COMMAND git apply ${CMAKE_CURRENT_SOURCE_DIR}/kiero_compile_definitions.patch
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
)

FetchContent_GetProperties(kiero)
if(NOT kiero_POPULATED)
  FetchContent_Populate(kiero)
endif()

add_library(minhook STATIC ${kiero_SOURCE_DIR}/minhook/src/buffer.c ${kiero_SOURCE_DIR}/minhook/src/hde/hde32.c ${kiero_SOURCE_DIR}/minhook/src/hook.c ${kiero_SOURCE_DIR}/minhook/src/trampoline.c)
add_library(kiero-lib STATIC ${kiero_SOURCE_DIR}/kiero.cpp)
target_include_directories(kiero-lib INTERFACE ${kiero_SOURCE_DIR})
target_link_libraries(kiero-lib PRIVATE minhook)