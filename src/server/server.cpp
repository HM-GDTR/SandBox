//
// Created by rainbowx on 24-6-10.
//

#include "server.h"

#include "json.hpp"
#include "spdlog/spdlog.h"

namespace SandBox {
    Server::Server(int port, std::string host_addr)
                : port_(port),
                  host_addr_(std::move(host_addr)) {
    }

    void Server::port(int port) { port_ = port; }
    [[nodiscard]] int Server::port() const { return port_; }

    void Server::host_addr (std::string host_addr) { host_addr_ = std::move(host_addr); }
    [[nodiscard]] const std::string& Server::host_addr() const { return host_addr_; }

    void Server::init_handler() {
        // handlers_.emplace_back(SandBox::ReqTy::GET_Ty, R"(/api/(\d+))", [](const httplib::Request& req, httplib::Response& res) -> void {
        //     std::string id = req.matches[1];
        //
        //     // 构建响应内容
        //     std::string response_text = "ID: " + id;
        //
        //     spdlog::error(response_text);
        //
        //     // 发送响应
        //     res.set_content(response_text, "text/plain");
        // });

        // /api/run 运行代码所需要的参数
        handlers_.emplace_back(
            SandBox::ReqTy::POST_Ty,
            "/api/run",
            [](const httplib::Request& req, httplib::Response& res) -> void {
                nlohmann::json request = nlohmann::json::parse(req.body);
                task_t tid;
                std::thread{[&](){tid = runner.add_task(request);}}.join();
                res.set_content(alive_answer[tid].dump(4), "application/json");
            });

        // /api/result 运行代码所需要的参数
        handlers_.emplace_back(
            SandBox::ReqTy::GET_Ty,
            R"(/api/result/(\d+))",
            [](const httplib::Request& req, httplib::Response& res) -> void {
                // 代码运行标识id
                std::string rid = req.matches[1];

                // 发送响应
                res.set_content("hello", "text/plain");
            });
    }

    void Server::run() {
        init_handler();

        web_server_.reset();
        web_server_ = std::make_unique<SandBox::WebServer>(handlers_, host_addr_, port_);
        web_server_->run();
    }

    Runner runner{};
    Server main_server{};
    AliveAnswer<nlohmann::json> alive_answer{};
}
