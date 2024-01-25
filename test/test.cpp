#include <ctime>
#include <iostream>
#include <vector>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "../src/UdpPacketHeader.h"

using udp_grpc_proxy::UdpPacketHeader;

namespace udp_grpc_proxy_tester
{

int main()
{
    WSADATA wsa_data;
    SOCKET udp_socket;
    struct sockaddr_in server_address{};

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2U, 2U), &wsa_data) != 0U)
    {
        std::fprintf(stderr, "WSAStartup failed");
        return 1;
    }

    // Create socket
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0U);
    if (udp_socket == INVALID_SOCKET)
    {
        std::fprintf(stderr, "Failed to create socket");
        WSACleanup();
        return 1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080U);                    // Port to send
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr); // IP to send

    srand(static_cast<unsigned>(time(nullptr))); // Seed for random number generation

    UdpPacketHeader header{};
    header.version = 1U;
    header.sequence = 0U;
    memset(header.grpc_service_hash, 0U, 16U); // Initialize grpc_service_hash with zeros

    const int max_payload_size = 512; // Maximum payload size
    char buffer[sizeof(UdpPacketHeader) + max_payload_size];

    while (true)
    {
        int payload_size = rand() % max_payload_size; // Generate random payload size
        header.length = sizeof(UdpPacketHeader) + payload_size;
        header.crc = 0U;                                                              // Placeholder for CRC
        header.time_nanoseconds = static_cast<uint64_t>(time(nullptr)) * 1000000000U; // Current time in nanoseconds

        memcpy(buffer, &header, sizeof(UdpPacketHeader)); // Copy header to buffer
        for (int i = 0U; i < payload_size; ++i)
        {
            buffer[sizeof(UdpPacketHeader) + i] = rand() % 256U; // Random payload data
        }

        // Send UDP packet
        if (sendto(udp_socket, buffer, header.length, 0U, reinterpret_cast<sockaddr *>(&server_address),
                   sizeof(server_address)) == SOCKET_ERROR)
        {
            std::fprintf(stderr, "sendto() failed with error code: %d", WSAGetLastError());
            break;
        }

        std::fprintf(stdout, "Sent a packet with payload size: %d", payload_size);

        header.sequence++; // Increase sequence for each packet
        Sleep(1000);       // Wait for a second
    }

    closesocket(udp_socket);
    WSACleanup();

    return 0;
}
} // namespace udp_grpc_proxy_tester