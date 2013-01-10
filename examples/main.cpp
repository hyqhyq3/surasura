#include <iostream>

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp> 
#include "inbuf.h"
#include "outbuf.h"

namespace counter {
    std::map<std::string, std::size_t> counter;
    
    class session : private boost::noncopyable {
    public:
        session(boost::asio::io_service& io_service) :
            socket_(io_service)
        {
            
        }
        
        void start()
        { coro_ = inbuf::coro_t(boost::bind(&session::handle, this, _1)); }
        
        boost::asio::ip::tcp::socket & socket() 
        { return socket_; }
        
    private:
        boost::asio::ip::tcp::socket socket_;
        inbuf::coro_t coro_;
        
        void handle(inbuf::coro_t::caller_type & ca)
        {
            inbuf in(socket_, coro_, ca);
            outbuf out(socket_, coro_, ca);
            boost::system::error_code ec;
            boost::array<uint8_t, 1024> buf;
            std::size_t n;
            do {
                n = in.read_some(boost::asio::buffer(buf), ec);
                if(ec)
                    break;
                out.write(boost::asio::buffer(buf, n), ec);
                if(ec)
                    break;
            } while(true);
            socket().get_io_service().post(boost::bind(&session::destroy, this));
        }
        
        void destroy() 
        {
            delete this;
        }
    };
    
    class server : private boost::noncopyable {
    public:
        server(boost::asio::io_service& io_service, std::string port) :
        io_service_(io_service),
        acceptor_(io_service_)
        {
            boost::asio::ip::tcp::resolver resolver(io_service_);
            boost::asio::ip::tcp::resolver::query query("localhost", port);
            boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
            acceptor_.open(endpoint.protocol());
            acceptor_.bind(endpoint);
            acceptor_.listen();
        }
        void start()
        {
            session* s = new session(io_service_);
            acceptor_.async_accept(s->socket(), [=](const boost::system::error_code& err){
                if(!err) {
                    s->start();
                }
                start();
            });
        }
    private:
        typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;
        boost::asio::io_service& io_service_; 
        boost::asio::ip::tcp::acceptor acceptor_;
    }; 
};


int main(int argc, char **argv) {
    boost::asio::io_service io;
    counter::server server(io, "3000");
    server.start();
    io.run();
    return 0;
}
