cmake_minimum_required(VERSION 3.7)

project(classificator VERSION 1.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(PkgConfig REQUIRED)
#find_package(FFTW REQUIRED)
pkg_check_modules(FFTW IMPORTED_TARGET REQUIRED fftw3)
find_package( OpenCV REQUIRED )

include_directories(${PROJECT_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${Qt5Core_INCLUDE_DIRS}
                    ${Qt5Gui_INCLUDE_DIRS}
                    ${Qt5Network_INCLUDE_DIRS}
                    ${CMAKE_SOURCE_DIR}
                    ${CMAKE_SOURCE_DIR}/src
                    ${CMAKE_SOURCE_DIR}/src/dataaccess
                    )

link_directories(${CMAKE_SOURCE_DIR}/src)
link_directories(${CMAKE_SOURCE_DIR}/src/dataaccess)
link_directories(${CMAKE_SOURCE_DIR}/src/gui)

set(CMAKE_BUILD_TYPE Debug)

set(HEADERS
    AircraftType.h
    AircraftImage.h
)

set(SOURCES
    AircraftType.cpp
    AircraftImage.cpp
)

set(FORMS
)

set(RESOURCES
)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS} ${RESOURCES}  )

target_link_libraries(${PROJECT_NAME} Qt5::Core ${QWT_LIBRARIES} ${OPENCV_LIBRARIES} db_data_access )


