project(flappy_bird CXX)

set(FLAPPY_BIRD_SOURCE_FILES "")
list(APPEND FLAPPY_BIRD_SOURCE_FILES
    src/flappy_bird.cpp
    src/components/flappy_animation.cpp
    src/components/flappy_controller.cpp
    src/components/game_over.cpp
    src/components/fps_counter.cpp    
    src/components/ground.cpp
    src/components/intro.cpp
    src/components/pipe.cpp
    src/components/point_counter.cpp
    src/managers/game_manager.cpp
)

add_executable(flappy_bird ${FLAPPY_BIRD_SOURCE_FILES})

target_link_libraries(flappy_bird PRIVATE pluto)