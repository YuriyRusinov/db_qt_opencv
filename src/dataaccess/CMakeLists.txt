cmake_minimum_required (VERSION 3.10)
project(db_data_access VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(PkgConfig REQUIRED)

find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)

include_directories(${PROJECT_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${Qt5Core_INCLUDE_DIRS}
                    ${Qt5Widgets_INCLUDE_DIRS}
                    ${OpenCV_INCLUDE_DIRS}
                    )

set(CMAKE_BUILD_TYPE Debug)

set(HEADERS
    opencv_database.h
    opencv_pgdatabase.h
    opencv_db_result.h
    opencv_db_pgresult.h
)

set(SOURCES
    opencv_database.cpp
    opencv_pgdatabase.cpp
    opencv_db_result.cpp
    opencv_db_pgresult.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS} )

target_link_libraries(${PROJECT_NAME} -lpqxx -lpq ${OpenCV_LIBS})
