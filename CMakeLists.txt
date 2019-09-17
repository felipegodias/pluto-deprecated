cmake_minimum_required(VERSION 3.6)

project(pluto CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/debug)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/lib/release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug)

include(${CMAKE_CURRENT_SOURCE_DIR}/conan/conanbuildinfo.cmake)
conan_basic_setup()

set(PLUTO_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/pluto/include)
set(PLUTO_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/pluto/src)

set(PLUTO_SOURCE_FILES "")
add_subdirectory(pluto/src/pluto)

add_library(pluto SHARED ${PLUTO_SOURCE_FILES})

set_target_properties(pluto PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)

target_compile_definitions(pluto PRIVATE PLUTO_DLL_EXPORT)

target_link_libraries(pluto PUBLIC ${CONAN_LIBS})

target_include_directories(pluto PUBLIC
    $<BUILD_INTERFACE:${PLUTO_INCLUDE_DIR}>
)

target_include_directories(pluto PRIVATE
    $<BUILD_INTERFACE:${PLUTO_SOURCE_DIR}>
)

add_subdirectory(examples/sandbox)
add_subdirectory(tools/pluto_asset_manager)