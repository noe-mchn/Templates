project(UnitTests)

set (HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Include)
set (SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)

set (HEADERS
    ${HEADER_DIR}/UnitsTests.h
)

set (SOURCES
    ${SOURCE_DIR}/UnitsTests.cpp
)

add_library(${PROJECT_NAME}
STATIC
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(${PROJECT_NAME} 
PUBLIC 
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${HEADER_DIR}>
)


target_link_libraries(${PROJECT_NAME}
PUBLIC
    containers
)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Libraries")