#pragma once

#include "GrpcIngesterManager.h"
#include <grpcpp/grpcpp.h>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>

namespace udp_grpc_proxy
{

class GrpcIngesterManager
{
  public:
    GrpcIngesterManager();

    // Forward the payload to the appropriate gRPC ingester based on the grpc_service_hash
    void forward(const uint8_t grpc_service_hash[16], const char *data, std::size_t size);

    // Method to add a new gRPC ingester stub
    void add_ingester(const std::string &hash_key, std::shared_ptr<grpc::Channel> channel);

  private:
    // Map to hold the gRPC ingester stubs
    // std::unordered_map<std::string, std::shared_ptr<grpc::GenericStub>> _ingester_stubs;
    std::unordered_map<std::string, double> _ingester_stubs;

    // Helper method to convert grpc_service_hash to string
    static std::string hash_to_string(const uint8_t grpc_service_hash[16]);
};
} // namespace udp_grpc_proxy