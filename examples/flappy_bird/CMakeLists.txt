project(flappy_bird CXX)

set(FLAPPY_BIRD_SOURCE_FILES "")
list(APPEND FLAPPY_BIRD_SOURCE_FILES
    src/flappy_bird.cpp
    src/components/flappy_animation.cpp
    src/components/flappy_controller.cpp
    src/components/ground.cpp
    src/managers/game_manager.cpp
)

add_executable(flappy_bird ${FLAPPY_BIRD_SOURCE_FILES})

target_link_libraries(flappy_bird PRIVATE pluto)