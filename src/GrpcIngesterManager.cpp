#include "GrpcIngesterManager.h"

namespace udp_grpc_proxy
{

GrpcIngesterManager::GrpcIngesterManager()
{
    // Constructor implementation (if needed)
}

void GrpcIngesterManager::add_ingester(const std::string &hash_key, std::shared_ptr<grpc::Channel> channel)
{
    //    _ingester_stubs[hash_key] = std::make_shared<grpc::GenericStub>(channel);
}

std::string GrpcIngesterManager::hash_to_string(const uint8_t grpc_service_hash[16])
{
    std::stringstream ss;
    for (int i = 0; i < 16; ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(grpc_service_hash[i]);
    }
    return ss.str();
}

void GrpcIngesterManager::forward(const uint8_t grpc_service_hash[16], const char *data, std::size_t size)
{
    std::string hash_key = hash_to_string(grpc_service_hash);
    auto it = _ingester_stubs.find(hash_key);
    if (it != _ingester_stubs.end())
    {
        // Construct the gRPC request and send it to the appropriate service
        grpc::ByteBuffer request;
        // grpc::GenericStub::Call call;

        // it->second->callMethod(/* ... */, &call, /* ... */);

        // Handle the response
    }
    else
    {
        std::fprintf(stderr, "No gRPC ingester found for the provided hash: %s", hash_key.c_str());
    }
}
} // namespace udp_grpc_proxy