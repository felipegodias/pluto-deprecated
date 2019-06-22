list(APPEND PLUTO_SOURCE_FILES
    # ./
    ${CMAKE_CURRENT_SOURCE_DIR}/root.cpp
    # ./di
    ${CMAKE_CURRENT_SOURCE_DIR}/di/base_factory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/di/di_container.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/di/singleton.cpp
    # ./event
    ${CMAKE_CURRENT_SOURCE_DIR}/event/base_event.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/event/event_installer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/event/event_manager.cpp
    # ./config
    ${CMAKE_CURRENT_SOURCE_DIR}/config/config_installer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/config/config_manager.cpp
    # ./file
    ${CMAKE_CURRENT_SOURCE_DIR}/file/file_installer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/file/file_manager.cpp
    # ./input
    ${CMAKE_CURRENT_SOURCE_DIR}/input/input_installer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/input/input_manager.cpp
    # ./log
    ${CMAKE_CURRENT_SOURCE_DIR}/log/log_installer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/log/log_manager.cpp
    # ./math
    ${CMAKE_CURRENT_SOURCE_DIR}/math/vector2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/vector3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/vector4.cpp
    # ./simulation
    ${CMAKE_CURRENT_SOURCE_DIR}/simulation/simulation_installer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/simulation/simulation_manager.cpp
    # ./window
    ${CMAKE_CURRENT_SOURCE_DIR}/window/window_installer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/window/window_manager.cpp
)

set(PLUTO_SOURCE_FILES ${PLUTO_SOURCE_FILES} PARENT_SCOPE)