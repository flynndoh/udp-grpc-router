#include "UdpReceiver.h"

namespace udp_grpc_proxy
{

UdpReceiver::UdpReceiver(uint16_t port, GrpcIngesterManager &ingester_manager) : _ingester_manager(ingester_manager)
{
    init_socket(port);
}

UdpReceiver::~UdpReceiver()
{
    _close(_socket_fd);
}

void UdpReceiver::init_socket(uint16_t port)
{
    if ((_socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        throw std::runtime_error("Socket creation failed");
    }

    memset(&_server_address, 0, sizeof(_server_address));
    memset(&_client_address, 0, sizeof(_client_address));

    _server_address.sin_family = AF_INET; // IPv4
    _server_address.sin_addr.s_addr = INADDR_ANY;
    _server_address.sin_port = htons(port);

    if (bind(_socket_fd, (const struct sockaddr *)&_server_address, sizeof(_server_address)) < 0)
    {
        throw std::runtime_error("Socket bind failed");
    }
}

void UdpReceiver::start()
{
    std::thread receiver_thread{&UdpReceiver::handle_receive, this};
    receiver_thread.detach();
}

static bool check_crc(const uint16_t received_crc, const char *data, size_t size)
{
    // TODO: Implement CRC check here
    return true;
}

[[noreturn]] void UdpReceiver::handle_receive()
{
    socklen_t len;
    while (true)
    {
        int n = recvfrom(_socket_fd, _receive_buffer.data(), _receive_buffer.size(), MSG_WAITALL,
                         (struct sockaddr *)&_client_address, &len);
        if (n <= 0)
        {
            std::fprintf(stderr, "Receive from socket failed");
            continue;
        }
        if (n <= sizeof(UdpPacketHeader))
        {
            std::fprintf(stderr, "Received data is too small to contain a valid UDP packet header");
            continue;
        }

        // Extract and deserialize the header
        UdpPacketHeader header = UdpPacketHeader::deserialize(_receive_buffer.data(), sizeof(UdpPacketHeader));

        // Perform CRC check
        if (!check_crc(header.crc, _receive_buffer.data() + sizeof(UdpPacketHeader), n - sizeof(UdpPacketHeader)))
        {
            std::fprintf(stderr, "CRC check failed");
            continue;
        }

        // Extract the content and forward to processing
        extract_payload(_receive_buffer.data(), n);
    }
}

void UdpReceiver::extract_payload(const char *data, std::size_t size)
{
    if (size <= sizeof(UdpPacketHeader))
    {
        std::fprintf(stderr, "Received data is too small to contain a valid UDP packet");
        return;
    }

    UdpPacketHeader header = UdpPacketHeader::deserialize(data, sizeof(UdpPacketHeader));
    const char *payload = data + sizeof(UdpPacketHeader);
    std::size_t payload_size = size - sizeof(UdpPacketHeader);

    // Forward to the appropriate gRPC ingester based on grpc_service_hash
    _ingester_manager.forward(header.grpc_service_hash, payload, payload_size);
}
} // namespace udp_grpc_proxy