list(APPEND PLUTO_SOURCE_FILES
    # ./
    ${CMAKE_CURRENT_SOURCE_DIR}/di_container.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/root.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/singleton.cpp
    # ./log
    ${CMAKE_CURRENT_SOURCE_DIR}/log/log_manager.cpp
)

set(PLUTO_SOURCE_FILES ${PLUTO_SOURCE_FILES} PARENT_SCOPE)