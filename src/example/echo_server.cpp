#include <thread>
#include <boost/asio.hpp>
#include <iostream>
#include <array>

using namespace boost;
using asio::ip::tcp;

#define BUFFER_SIZE 512

class Session:public std::enable_shared_from_this<Session>{
public:
  Session(tcp::socket socket):_socket(std::move(socket)){}
  ~Session(){}
  void start(){do_read();}
private:
  void do_read(){
    auto self(shared_from_this());
    asio::async_read(_socket,asio::buffer(_buffer,BUFFER_SIZE),
                     [this, self](std::error_code ec, size_t size){
                       if(!ec)
                         do_write(size);
                       else
                         std::cout<<"read error: "<<ec<<std::endl;
                     });
  }

  void do_write(const size_t& size){
    auto self(shared_from_this());
    asio::async_write(_socket, asio::buffer(_buffer, size),
                      [this,self](std::error_code ec, size_t size){
                        if(!ec)
                          std::cout<<"echo to "<<_socket.remote_endpoint().address().to_string()<<":"<<_socket.remote_endpoint().port()<< " : "<<_buffer<<std::endl;
                        else
                          std::cout<<"write error: "<<ec<<std::endl;
                        do_read();
                      });
  }

  tcp::socket _socket;
  char _buffer[BUFFER_SIZE];
};

class Server{
public:
  Server(asio::io_context& io_context, tcp::endpoint& endpoint);
  ~Server();

private:
  void do_accept();

  tcp::acceptor _acceptor;
};

Server::Server(asio::io_context& io_context, tcp::endpoint& endpoint)
  :_acceptor(io_context, endpoint)
{
  do_accept();
}

Server::~Server()
{}

void Server::do_accept(){
  _acceptor.async_accept(
                         [this](std::error_code ec, tcp::socket socket){
                           if(!ec)
                             (std::make_shared<Session>(std::move(socket)))->start();
                           else
                             std::cout<<"accept error: "<<ec<<std::endl;
                           do_accept();
                         });
}

int main(int argc, char* argv[]){
  if(argc<2){
    std::cout<<"Usage: ./server port"<<std::endl;
    return 1;
  }
  asio::io_context io_context;
  tcp::endpoint ep(tcp::v4(), std::atoi(argv[1]));
  Server s(io_context, ep);
  io_context.run();
  return 0;
}


