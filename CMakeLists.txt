set(PROJECT_NAME partooth)
cmake_minimum_required( VERSION 3.0.2 )
project(${PROJECT_NAME})

find_package(Qt5Widgets REQUIRED)
find_package(Qt5DBus REQUIRED)

if(NOT CMAKE_BUILD_TYPE)
    set( CMAKE_BUILD_TYPE Debug )
endif (NOT CMAKE_BUILD_TYPE)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(SRCS
   main.cpp
   mainwindow.cpp
   org.bluez.Adapter1.cpp
   org.bluez.Device1.cpp
   org.freedesktop.DBus.ObjectManager.cpp
)


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(${PROJECT_NAME}
   ${SRCS}
)

target_link_libraries(${PROJECT_NAME} 
					  Qt5::Widgets
                      Qt5::DBus
                     )


