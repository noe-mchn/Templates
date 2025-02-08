project(containers)

set (HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)

set (HEADERS
    ${HEADER_DIR}/VectorN.h
    ${HEADER_DIR}/ArrayN.h
    ${HEADER_DIR}/ListN.h
    ${HEADER_DIR}/IntrusiveListN.h
    ${HEADER_DIR}/IteratorsN.h
    ${HEADER_DIR}/VecteurND.h
    ${HEADER_DIR}/MatrixN.h
)

set (SOURCES
    ${SOURCE_DIR}/VectorN.cpp
    ${SOURCE_DIR}/ArrayN.cpp
    ${SOURCE_DIR}/ListN.cpp
    ${SOURCE_DIR}/IntrusiveListN.cpp
    ${SOURCE_DIR}/IteratorsN.cpp
    ${SOURCE_DIR}/VecteurND.cpp
    ${SOURCE_DIR}/MatrixN.cpp
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

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Libraries")
