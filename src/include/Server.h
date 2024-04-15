//
// Created by RainbowX on 24-4-2.
//

#ifndef SANDBOX_SERVER_H
#define SANDBOX_SERVER_H

#include <memory>

#include "json.hpp"
#include "boost/asio.hpp"

#include "status.hpp"

namespace SandBox {

    class Server {
        using json = nlohmann::json;
        using io_service = boost::asio::io_service;
        using socket = boost::asio::ip::tcp::socket;
        using endpoint = boost::asio::ip::tcp::endpoint;
        using acceptor = boost::asio::ip::tcp::acceptor;
    public:
        explicit Server(int port = 49999);
        explicit Server(const endpoint& ep);

        void run();
        void run(std::function<json(json)> handler);

        // TODO run single
        // void run_single(std::function<json(json)> handler);

    protected:
        acceptor _acceptor;
        io_service _io_service;

        static json default_handler(const json& data);
    };

}

#endif //SANDBOX_SERVER_H
