set(PROJECT_NAME partooth)
cmake_minimum_required( VERSION 3.0.2 )
set(CMAKE_CXX_FLAGS "-std=c++14 ${CMAKE_CXX_FLAGS}")

project(${PROJECT_NAME})

find_package(Qt5Widgets REQUIRED)
find_package(Qt5DBus REQUIRED)

if(NOT CMAKE_BUILD_TYPE)
    set( CMAKE_BUILD_TYPE Debug )
endif (NOT CMAKE_BUILD_TYPE)

set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTOUIC ON)

file(GLOB SRCS *.cpp)
file(GLOB UIS *.ui)

qt5_wrap_ui(UIHS ${UIS})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(${PROJECT_NAME}
   ${SRCS}
   ${UIHS}
)

target_link_libraries(${PROJECT_NAME} 
                      Qt5::Widgets
                      Qt5::DBus
                     )


