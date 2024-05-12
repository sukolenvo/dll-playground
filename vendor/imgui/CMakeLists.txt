include(FetchContent)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.90.6
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
)

FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
  FetchContent_Populate(imgui)
endif()

file(GLOB source ${imgui_SOURCE_DIR}/*.cpp)

add_library(imgui STATIC ${source} ${imgui_SOURCE_DIR}/backends/imgui_impl_dx9.cpp ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR}/ ${imgui_SOURCE_DIR}/backends/)