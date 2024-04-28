cmake_minimum_required(VERSION 3.16)
project(RFIDServerr)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network HttpServer WebSockets)

add_executable(RFIDServerr
    main.cpp
    socketserver.cpp
    socketserver.h
    server.cpp
    server.h
)

target_link_libraries(RFIDServerr
    Qt6::Core
    Qt6::Network
    Qt6::HttpServer
    Qt6::WebSockets
)

include(GNUInstallDirs)
install(TARGETS RFIDServerr
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
