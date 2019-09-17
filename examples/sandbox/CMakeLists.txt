project(sandbox CXX)

set(SANDBOX_SOURCE_FILES "")
list(APPEND SANDBOX_SOURCE_FILES
    sandbox.cpp
    components/rotator.cpp
)

add_executable(sandbox ${SANDBOX_SOURCE_FILES})

target_link_libraries(sandbox PRIVATE pluto)