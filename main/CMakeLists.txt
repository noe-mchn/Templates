project(main)

set (SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)

message (STATUS ${SOURCE_DIR})

set (SOURCES
    ${SOURCE_DIR}/main.cpp
)

add_executable(${PROJECT_NAME}
    ${SOURCES}
)

target_link_libraries(${PROJECT_NAME}
PUBLIC
    UnitTests
)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Applications")