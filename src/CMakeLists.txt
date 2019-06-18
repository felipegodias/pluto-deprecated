list(APPEND PLUTO_SOURCE_FILES
    # ./
    ${CMAKE_CURRENT_SOURCE_DIR}/root.cpp
    # ./di
    ${CMAKE_CURRENT_SOURCE_DIR}/di/base_factory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/di/di_container.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/di/singleton.cpp
    # ./config
    ${CMAKE_CURRENT_SOURCE_DIR}/config/config_installer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/config/config_manager.cpp
    # ./log
    ${CMAKE_CURRENT_SOURCE_DIR}/log/log_manager.cpp
)

set(PLUTO_SOURCE_FILES ${PLUTO_SOURCE_FILES} PARENT_SCOPE)