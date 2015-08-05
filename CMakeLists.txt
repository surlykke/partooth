set(PROJECT_NAME partooth)
cmake_minimum_required( VERSION 3.0.2 )
project(${PROJECT_NAME})

find_package(Qt5Widgets REQUIRED)
find_package(Qt5DBus REQUIRED)


if(NOT CMAKE_BUILD_TYPE)
    set( CMAKE_BUILD_TYPE Debug )
endif (NOT CMAKE_BUILD_TYPE)


set(HDRS
    dbus_types.h
)

set(SRCS
    main.cpp
	mainwindow.cpp
)


set(UIS
    mainwindow.ui
)

set(MOCS
	mainwindow.h
	)

qt5_wrap_cpp(MOCS_GENERATED ${MOCS})
qt5_wrap_ui(UIS_GENERATED ${UIS})

#qt5_add_resources(QRCS_GENERATED ${QRCS})

#qt5_add_dbus_adaptor(DBUS_GENERATED net.connman.Agent.xml agent.h Agent)
#
set_source_files_properties(org.freedesktop.DBus.ObjectManager.xml PROPERTIES INCLUDE dbus_types.h)
set_source_files_properties(org.bluez.Adapter1.xml PROPERTIES INCLUDE dbus_types.h)
qt5_add_dbus_interface(DBUS_GENERATED org.freedesktop.DBus.ObjectManager.xml org.freedesktop.DBus.ObjectManager)
qt5_add_dbus_interface(DBUS_GENERATED org.bluez.Adapter1.xml org.bluez.Adapter1)
qt5_add_dbus_interface(DBUS_GENERATED org.bluez.Device1.xml org.bluez.Device1)
#

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(${PROJECT_NAME}
   ${SRCS}
   ${UIS_GENERATED}
   ${MOCS_GENERATED}
   ${DBUS_GENERATED}
)



#add_dependencies(${PROJECT_NAME} ${LXQT_LIBRARIES})
target_link_libraries(${PROJECT_NAME} 
					  Qt5::Widgets
                      Qt5::DBus
                     )


