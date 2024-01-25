cmake_minimum_required(VERSION 3.8)
project(cpp_udp_grpc_router)

set(CMAKE_CXX_STANDARD 17)

# Find required packages
find_package(gRPC CONFIG REQUIRED)

add_executable(cpp_udp_grpc_router src/main.cpp src/UdpReceiver.cpp src/UdpReceiver.h src/GrpcIngesterManager.cpp src/GrpcIngesterManager.h src/UdpPacketHeader.h)
add_executable(udp_packet_sender test/test.cpp)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE gRPC::grpc)

