#pragma once

#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <winsock2.h>

namespace udp_grpc_proxy
{

// Structure for the UDP packet header
struct UdpPacketHeader
{
    uint8_t version;
    uint16_t length;
    uint16_t crc;
    uint8_t sequence;
    uint64_t time_nanoseconds;
    uint8_t grpc_service_hash[16];

    // Method to deserialize the header from a buffer
    static UdpPacketHeader deserialize(const char *buffer, std::size_t size);
};

UdpPacketHeader UdpPacketHeader::deserialize(const char *buffer, std::size_t size)
{
    if (size < sizeof(UdpPacketHeader))
    {
        throw std::runtime_error("Buffer size is too small for UDPPacketHeader deserialization");
    }

    UdpPacketHeader header{};
    size_t offset = 0;

    // Deserialize each field from the buffer
    // Assuming network byte order for multibyte fields, so using ntohs for conversion

    // Version (1 byte)
    std::memcpy(&header.version, buffer + offset, sizeof(header.version));
    offset += sizeof(header.version);

    // Length (2 bytes)
    uint16_t length_network_order;
    std::memcpy(&length_network_order, buffer + offset, sizeof(length_network_order));
    header.length = ntohs(length_network_order);
    offset += sizeof(length_network_order);

    // CRC (2 bytes)
    uint16_t crc_network_order;
    std::memcpy(&crc_network_order, buffer + offset, sizeof(crc_network_order));
    header.crc = ntohs(crc_network_order);
    offset += sizeof(crc_network_order);

    // Sequence (1 byte)
    std::memcpy(&header.sequence, buffer + offset, sizeof(header.sequence));
    offset += sizeof(header.sequence);

    // Time Nanoseconds (8 bytes)
    uint64_t time_nanoseconds_network_order;
    std::memcpy(&time_nanoseconds_network_order, buffer + offset, sizeof(time_nanoseconds_network_order));
    header.time_nanoseconds =
        time_nanoseconds_network_order; // may need to use be64toh to convert from big-endian to host byte order
    offset += sizeof(time_nanoseconds_network_order);

    // GRPC Service Hash (16 bytes)
    std::memcpy(&header.grpc_service_hash, buffer + offset, sizeof(header.grpc_service_hash));
    offset += sizeof(header.grpc_service_hash);

    return header;
}
} // namespace udp_grpc_proxy