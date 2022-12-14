cmake_minimum_required(VERSION 3.7)

project(db_opencv_src VERSION 1.0.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(PkgConfig REQUIRED)
find_package( OpenCV REQUIRED )

add_subdirectory(dataaccess)
add_subdirectory(classificator)
add_subdirectory(imagefactories)
add_subdirectory(gui)

include_directories(${PROJECT_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${Qt5Widgets_INCLUDE_DIRS}
                    ${Qt5Core_INCLUDE_DIRS}
                    ${Qt5Gui_INCLUDE_DIRS}
                    ${Qt5OpenGL_INCLUDE_DIRS}
                    ${QWT_INCLUDE_DIRS}
                    ${OpenCV_INCLUDE_DIRS}
                    ${CMAKE_CURRENT_SOURCE_DIR}/dataaccess
                    ${CMAKE_CURRENT_SOURCE_DIR}/classificator
                    ${CMAKE_CURRENT_SOURCE_DIR}/imagefactories
                    ${CMAKE_CURRENT_SOURCE_DIR}/gui
                   )

link_directories(${CMAKE_SOURCE_DIR}/src)

link_directories(${CMAKE_SOURCE_DIR}/src/dataaccess)
link_directories(${CMAKE_SOURCE_DIR}/src/classificator)
link_directories(${CMAKE_SOURCE_DIR}/src/imagefactories)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/gui)

set(CMAKE_BUILD_TYPE Debug)

set(HEADERS
    db_opencv_singleton.h
    opencvcore.h
)

set(FORMS
)

set(SOURCES
    db_opencv_singleton.cpp
    opencvcore.cpp
)

set(RESOURCES
    aircraftResources.qrc
)

qt5_wrap_ui(UIS_HDRS ${FORMS} )
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS} ${UIS_HDRS} ${RESOURCES} )

target_link_libraries(${PROJECT_NAME} Qt5::Core ${QWT_LIBRARIES} ${OpenCV_LIBS} imagefactories db_data_access db_gui)

set(HEADERS
    db_opencv_singleton.h
)

set(FORMS
)

set(${PROJECT_NAME}_RUNTIME_LIBRARY_DIRS ${CMAKE_INSTALL_LIBDIR} )

target_include_directories(${PROJECT_NAME} PUBLIC
	$<INSTALL_INTERFACE:include/${PROJECT_NAME}> )

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION lib
)

install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

