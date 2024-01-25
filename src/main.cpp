#include "GrpcIngesterManager.h"
#include "UdpReceiver.h"

#include <iostream>
#include <memory>

namespace udp_grpc_proxy
{

[[noreturn]] int main()
{
    GrpcIngesterManager ingester_manager{};

    // Initialize gRPC channels and add them to the ingesterManager, e.g:
    // auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    // ingesterManager.add_ingester("grpc_service_hash", channel);

    // Initialize the UdpReceiver
    const uint16_t port = 12345; // Set the UDP port number
    UdpReceiver udp_receiver{port, ingester_manager};

    // Start the UdpReceiver
    udp_receiver.start();

    std::cout << "UDP Receiver is running on port " << port << std::endl;

    // Keep the main thread alive
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
} // namespace udp_grpc_proxy