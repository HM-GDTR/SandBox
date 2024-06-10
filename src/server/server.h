//
// Created by rainbowx on 24-6-9.
//

#ifndef SERVER_H
#define SERVER_H
#include <json.hpp>
#include <regex>
#include <string>
#include <memory>
#include <Runner.h>

#include "WebServer.h"
#include "AliveAnswer.hpp"

namespace SandBox {
    constexpr int DEFAULT_PORT = 49999;
    inline auto DEFAULT_HOST = "0.0.0.0";

    class Server {
    private:
        int port_;
        std::string host_addr_;
        std::unique_ptr<SandBox::WebServer> web_server_;

    protected:
        std::vector<SandBox::UrlHandler> handlers_;

    public:
        explicit Server(int port = DEFAULT_PORT, std::string host_addr = DEFAULT_HOST);

        void port(int port);
        [[nodiscard]] int port() const;

        void host_addr (std::string host_addr);
        [[nodiscard]] const std::string& host_addr() const;

        void init_handler();

        void run();
    };

    extern Runner runner;
    extern Server main_server;
    extern AliveAnswer<nlohmann::json> alive_answer;
}

#endif //SERVER_H
