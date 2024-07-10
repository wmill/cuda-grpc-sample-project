#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

#include "gen/sample.grpc.pb.h"

using cudasample::BoopReply;
using cudasample::BoopRequest;
using cudasample::Greeter;

std::string local_server_address = "localhost:50051";


class GreeterServiceImpl final : public Greeter::Service {
  Status SayBoop(ServerContext* context, const BoopRequest* request,
                  BoopReply* reply) override {
    std::string prefix("Boop ");
    reply->set_message(prefix + request->name());
    return Status::OK;
  }
};


void RunServer() {
  std::string server_address(local_server_address);
  GreeterServiceImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();

  return 0;
}