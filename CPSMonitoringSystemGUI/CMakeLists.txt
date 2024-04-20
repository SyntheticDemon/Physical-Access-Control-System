cmake_minimum_required(VERSION 3.5)

project(MonitoritngSystem VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure you are finding the appropriate Qt package and including the WebSockets component
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets WebSockets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets WebSockets)

set(PROJECT_SOURCES
        main.cpp
        cpsmainwindow.cpp
        cpsmainwindow.h
        cpshistorywindow.h cpshistorywindow.cpp
        cpstextfield.h cpstextfield.cpp
        cpsbutton.h cpsbutton.cpp
        cpslabel.h cpslabel.cpp
        cpswindowsapitools.h
        cpsapplication.h cpsapplication.cpp
        HTTP
        httprequest.h httprequest.cpp
        httpresponse.h httpresponse.cpp
        bidirectionalconnection.h bidirectionalconnection.cpp
)

set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/icon.rc")

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MonitoritngSystem
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${app_icon_resource_windows}
    )
else()
    if(ANDROID)
        add_library(MonitoritngSystem SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(MonitoritngSystem
            ${PROJECT_SOURCES}
        )
    endif()
endif()

if(WIN32)
    target_link_libraries(MonitoritngSystem PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::WebSockets dwmapi)
else()
    target_link_libraries(MonitoritngSystem PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::WebSockets)
endif()

set_target_properties(MonitoritngSystem PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS MonitoritngSystem
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MonitoritngSystem)
endif()
