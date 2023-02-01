cmake_minimum_required(VERSION 3.7)

project(db_gui VERSION 1.0)

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
pkg_check_modules(FFTW IMPORTED_TARGET REQUIRED fftw3)
find_package( OpenCV REQUIRED )

include_directories(${PROJECT_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${Qt5Widgets_INCLUDE_DIRS}
                    ${Qt5Core_INCLUDE_DIRS}
                    ${Qt5Gui_INCLUDE_DIRS}
                    ${Qt5Network_INCLUDE_DIRS}
                    ${CMAKE_SOURCE_DIR}
                    ${CMAKE_SOURCE_DIR}/src
                    ${CMAKE_SOURCE_DIR}/src/dataaccess
                    ${CMAKE_SOURCE_DIR}/src/classificator
                    ${CMAKE_SOURCE_DIR}/src/imagefactories
                    )

link_directories(${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/classificator)

set(CMAKE_BUILD_TYPE Debug)

set(HEADERS
    dbLoginForm.h
    cvImageForm.h
    cvImageListForm.h
    cvImageModel.h
    AircraftTypeForm.h
    AircraftTypeListForm.h
    AircraftTypeModel.h
)

set(SOURCES
    dbLoginForm.cpp
    cvImageForm.cpp
    cvImageListForm.cpp
    cvImageModel.cpp
    AircraftTypeForm.cpp
    AircraftTypeListForm.cpp
    AircraftTypeModel.cpp
)

set(FORMS
    db_login_form.ui
    cv_image_form.ui
    cv_image_list_form.ui
    aircraft_type_form.ui
    aircraft_type_list_form.ui
)

set(RESOURCES
)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS} ${RESOURCES}  )

target_link_libraries(${PROJECT_NAME} Qt5::Widgets ${QWT_LIBRARIES} ${OPENCV_LIBRARIES} classificator)


