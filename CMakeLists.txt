cmake_minimum_required(VERSION 3.20)
project(EunoiaUI-Framework VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE EUNOIA_UI_HEADERS
    "Include/EunoiaUI/*.h"
)

file(GLOB_RECURSE EUNOIA_UI_SOURCES
    "Source/*.cpp"
)

add_library(EunoiaUI STATIC
    ${EUNOIA_UI_HEADERS}
    ${EUNOIA_UI_SOURCES}
)

target_include_directories(EunoiaUI PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>
    $<INSTALL_INTERFACE:include>
)

target_compile_definitions(EunoiaUI PUBLIC
    EUNOIA_UI_STANDALONE=1
)
