cmake_minimum_required(VERSION 3.7)

project(radius_src VERSION 1.0.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(PkgConfig REQUIRED)
find_package( OpenCV REQUIRED )

include_directories(${PROJECT_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${Qt5Widgets_INCLUDE_DIRS}
                    ${Qt5Core_INCLUDE_DIRS}
                    ${Qt5Gui_INCLUDE_DIRS}
		            ${Qt5OpenGL_INCLUDE_DIRS}
		            ${QWT_INCLUDE_DIRS}
                    ${OpenCV_INCLUDE_DIRS}
                    )


set(HEADERS
)

set(FORMS
)

set(SOURCES
)

set(RESOURCES
)

qt5_wrap_ui(UIS_HDRS ${FORMS} )
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS} ${UIS_HDRS}
	${RESOURCES} )

target_link_libraries(${PROJECT_NAME} Qt5::Widgets ${QWT_LIBRARIES} ${OpenCV_LIBS})

set(${PROJECT_NAME}_RUNTIME_LIBRARY_DIRS ${CMAKE_INSTALL_LIBDIR} )

target_include_directories(${PROJECT_NAME} PUBLIC
	$<INSTALL_INTERFACE:include/${PROJECT_NAME}> )

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION lib
)

install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

