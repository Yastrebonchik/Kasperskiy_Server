cmake_minimum_required(VERSION 3.16)
project(Server_Task)

#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_MODULE_PATH ${./} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(log4cpp_DIR ./)
find_package(log4cpp REQUIRED)

add_executable(Server Server_srcs/server.cpp)
add_executable(Client Client_Sources/Client.cpp)

target_link_libraries(Server LINK_PUBLIC ${LOG4CPP_LIBRARIES})