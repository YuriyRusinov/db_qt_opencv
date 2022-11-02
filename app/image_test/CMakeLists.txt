cmake_minimum_required(VERSION 3.7)

project(image_test_app VERSION 1.0)

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

link_directories(${CMAKE_SOURCE_DIR}/src
                 ${CMAKE_SOURCE_DIR}/src/dataaccess
                 ${CMAKE_SOURCE_DIR}/src/classificator
                 ${CMAKE_SOURCE_DIR}/src/imagefactories
)

set(HEADERS
    imagetestmainwindow.h
)

set(FORMS
    image_test_main_window.ui
)

set(SOURCES
    imagetestmainwindow.cpp
    main.cpp
)

set(RESOURCES
)

qt5_wrap_ui(UIS_HDRS ${FORMS} )

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${UIS_HDRS} ${RESOURCES}  )

target_link_libraries(${PROJECT_NAME} Qt5::Widgets ${QWT_LIBRARIES} ${OPENCV_LIBRARIES} db_opencv_src ${imagefactories_LIBS} classificator)

install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
)
