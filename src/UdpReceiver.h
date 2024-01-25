#pragma once

#include "GrpcIngesterManager.h"
#include "UdpPacketHeader.h"
#include <cstdint>
#include <io.h>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>

namespace udp_grpc_proxy
{

class UdpReceiver
{
  public:
    UdpReceiver(uint16_t port, GrpcIngesterManager &ingesterManager);
    ~UdpReceiver();

    // Start the receiver
    void start();

  private:
    int _socket_fd{};
    struct sockaddr_in _server_address
    {
    }, _client_address{};
    std::array<char, 1024> _receive_buffer{};
    GrpcIngesterManager &_ingester_manager;

    // Initialize socket
    void init_socket(uint16_t port);

    // Handle received data
    [[noreturn]] void handle_receive();

    // Extract the content from the received data
    void extract_payload(const char *data, std::size_t size);
};
} // namespace udp_grpc_proxy