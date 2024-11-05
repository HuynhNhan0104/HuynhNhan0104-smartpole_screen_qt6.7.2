cmake_minimum_required(VERSION 3.19)

project(smartpole_screen VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(Qt6_DIR )

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick  Mqtt Multimedia)
find_package(Threads)



qt_standard_project_setup()

qt_add_executable(appsmartpole_screen
    main.cpp
)

qt_add_qml_module(appsmartpole_screen
    URI smartpole_screen
    VERSION 1.0
    QML_FILES
        Main.qml
        QML_FILES RadialBarProgress.qml
        SOURCES dashboardController.h dashboardController.cpp
        SOURCES serial/ODR_01.cpp serial/ODR_01.h serial/rs485.cpp serial/rs485.h serial/serial_interface.cpp serial/serial_interface.h serial/serial_port.cpp serial/serial_port.h serial/auto_connect.h serial/auto_connect.cpp 
        SOURCES videoController.h videoController.cpp
        SOURCES
        QML_FILES VideoView.qml
        SOURCES mqttHandler.h mqttHandler.cpp
        SOURCES httpHandler.h httpHandler.cpp
        # QML_FILES config.jso
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appsmartpole_screen PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appsmartpole_screen
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appsmartpole_screen
    PRIVATE Qt6::Quick Qt6::Mqtt Qt6::Multimedia

)

# set(CMAKE_INSTALL_PREFIX "/home/nhan/appqt/")
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/deloy)



include(GNUInstallDirs)
install(TARGETS appsmartpole_screen
    BUNDLE DESTINATION .
    # LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_qml_app_script(
    TARGET appsmartpole_screen
    FILENAME_VARIABLE deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
    DEPLOY_USER_QML_MODULES_ON_UNSUPPORTED_PLATFORM
)
install(SCRIPT ${deploy_script})
