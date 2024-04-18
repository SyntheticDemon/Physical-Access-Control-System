cmake_minimum_required(VERSION 3.16)
project(RFIDServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Network HttpServer REQUIRED)

add_executable(RFIDServer main.cpp)

target_link_libraries(RFIDServer Qt6::Core Qt6::Network Qt6::HttpServer)

include(GNUInstallDirs)
install(TARGETS RFIDServer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
