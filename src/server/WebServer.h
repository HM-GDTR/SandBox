//
// Created by rainbowx on 24-6-8.
//

#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <string>
#include <functional>

#include "httplib.h"

namespace SandBox {

    enum class ReqTy {
        UNKNOWN_Ty = 0,
        GET_Ty,             // 请求服务器发送某个资源, GET 请求不应该对服务器上的资源做出任何更改, 并且应该是幂等的
        HEAD_Ty,            // 类似于 GET 请求, 但服务器不返回请求的资源主体，只返回响应头                                                    (不支持)
        POST_Ty,            // 用于向服务器提交数据, POST 请求可能会导致服务器上的状态更改，并且不一定是幂等
        PUT_Ty,             // 用于向服务器上传资源, PUT 请求应该是幂等的
        DELETE_Ty,          // 用于请求服务器删除指定的资源, DELETE 请求应该是幂等的
        CONNECT_Ty,         // 用于建立到服务器上指定端口的隧道                                                                           (不支持)
        OPTIONS_Ty,         // 用于请求服务器返回支持的 HTTP 方法和其他选项。例如，客户端可以发送 OPTIONS 请求来确定服务器支持哪些 CORS 策略。       (不支持)
        TRACE_Ty,           // 用于回显服务器收到的请求
        PATCH_Ty            // 用于在请求-响应链上的每个节点获取传输路径
    };

    inline const char *to_string(ReqTy e);

    class UrlHandler {
    private:
        ReqTy req_ty;
        std::string url_pattern;
        std::function<void(const httplib::Request &, httplib::Response &)> handler;

    public:
        UrlHandler(ReqTy req_ty, std::string url_pattern,
            const std::function<void(const httplib::Request &, httplib::Response &)> &handler);

        [[nodiscard]] ReqTy get_req_ty() const;

        [[nodiscard]] std::string get_url_pattern() const;

        [[nodiscard]] std::function<void(const httplib::Request &, httplib::Response &)> get_handler() const;
    };

    class WebServer {
    private:
        int port_;
        httplib::Server svr_;
        std::string host_addr_;
        std::vector<UrlHandler> handlers_;

    protected:
        void apply_handler();
    public:
        explicit WebServer(std::string host_addr, int port);
        explicit WebServer(const std::vector<UrlHandler>& handlers, std::string host_addr, int port);

        void add_handler(UrlHandler handler);

        void port(int port);
        [[nodiscard]] int port() const;

        void host_addr(std::string host_addr);
        [[nodiscard]] std::string host_addr() const;

        void handlers(std::vector<UrlHandler> handlers);
        [[nodiscard]] const std::vector<UrlHandler>& handlers() const;

        void run();
    };



} // namespace SandBox

#endif // WEBSERVER_H
