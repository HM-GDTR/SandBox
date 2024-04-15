//
// Created by RainbowX on 24-4-2.
//
#include <thread>
#include <iostream>
#include <exception>

#include "boost/asio.hpp"
#include "spdlog/spdlog.h"

#include "Server.h"
#include "ThreadInfo.hpp"
#include "BaseException.h"

namespace SandBox {

    SandBox::Server::Server(const boost::asio::ip::tcp::endpoint& ep): _io_service(), _acceptor(_io_service, ep) { }
    SandBox::Server::Server(int port): _io_service(), _acceptor(_io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) { }

    using boost::asio::ip::tcp;
    using json = nlohmann::json;
    using namespace std::string_literals;

    void session(tcp::socket socket, std::function<json(json)> handler) {
        // TODO 添加错误检查
        tcp::iostream ios(std::move(socket));

        json json_obj = json::parse(ios), ret_val = handler(json_obj);

        ios<<ret_val.dump(4)<<std::endl;

        ios.close();

        // std::this_thread::sleep_for(std::chrono::seconds(1));
        // boost::system::error_code ec;
        // socket.shutdown(tcp::socket::shutdown_both, ec);
        // socket.close();
    }

    /**
     * 以默认处理函数运行服务器
     */
    void Server::run() {
        run(default_handler);
    }

    /**
     * 运行服务器
     * @param handler 请求处理函数
     */
    void Server::run(std::function<json(json)> handler) {
        while (true) {
            try {
                socket newSocket(_io_service);
                _acceptor.accept(newSocket);
                std::thread{session, std::move(newSocket), std::move(handler)}.detach();
            }
            catch (BaseException& exception) {
                spdlog::error(exception.to_string());
                return;
            }
            catch (...) {
                spdlog::error("Unknown Error.");
                return;
            }
        }
    }

    /**
     * 默认的处理函数
     */
    json Server::default_handler(const json& data) {
        return SandBox::Server::json();
    }

}