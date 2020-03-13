#include <iostream>
#include <boost/asio.hpp>
#include <array>

using namespace boost;
using asio::ip::tcp;

void do_write(tcp::socket& socket);

void do_read(tcp::socket& socket, size_t size){
  char buffer[512];
  socket.async_read_some(asio::buffer(buffer,512),
                         [&buffer, &socket](std::error_code ec, size_t size){
                           std::cout<<buffer<<std::endl;
                           do_write(socket);
                         });
}

void do_write(tcp::socket& socket){
  char buffer[512];
  std::cin>>buffer;
  socket.async_write_some(asio::buffer(buffer,512),
                          [&socket, &buffer](std::error_code ec, size_t size){
                            if(!ec)
                              do_read(socket, size);
                            else
                              std::cout<<"write error: "<<ec<<std::endl;
                          });
}


int main(int argc, char* argv[]){
  asio::io_context io_context;
  tcp::endpoint ep(asio::ip::address::from_string(argv[1]), std::atoi(argv[2]));
  tcp::socket socket(io_context);

  std::cout<<"start connect"<<std::endl;
  socket.connect(ep);
  std::cout<<"connected"<<std::endl;
  do_write(socket);


  io_context.run();
  return 0;
}
