project(sandbox CXX)

add_executable(sandbox sandbox.cpp)

target_link_libraries(sandbox PRIVATE pluto)