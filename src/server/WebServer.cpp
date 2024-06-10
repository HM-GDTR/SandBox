#include "WebServer.h"
#include "spdlog/spdlog.h"

namespace SandBox {

    inline const char *to_string(const ReqTy e) {
        switch (e) {
            case ReqTy::UNKNOWN_Ty: return "UNKNOWN";
            case ReqTy::GET_Ty: return "GET";
            case ReqTy::HEAD_Ty: return "HEAD";
            case ReqTy::POST_Ty: return "POST";
            case ReqTy::PUT_Ty: return "PUT";
            case ReqTy::DELETE_Ty: return "DELETE";
            case ReqTy::CONNECT_Ty: return "CONNECT";
            case ReqTy::OPTIONS_Ty: return "OPTIONS";
            case ReqTy::TRACE_Ty: return "TRACE";
            case ReqTy::PATCH_Ty: return "PATCH";
            default: return "UNKNOWN";
        }
    }

    UrlHandler::UrlHandler(ReqTy req_ty, std::string url_pattern,
        const std::function<void(const httplib::Request &, httplib::Response &)> &handler)
        : req_ty(req_ty),
          url_pattern(std::move(url_pattern)),
          handler(handler) {
    }

    ReqTy UrlHandler::get_req_ty() const {
        return req_ty;
    }

    std::string UrlHandler::get_url_pattern() const {
        return url_pattern;
    }

    std::function<void(const httplib::Request &, httplib::Response &)> UrlHandler::get_handler() const {
        return handler;
    }

    void WebServer::apply_handler() {
        for(const auto& handler: handlers_) {
            auto req_ty = handler.get_req_ty();
            switch (req_ty) {
                case ReqTy::UNKNOWN_Ty:
                {
                    spdlog::error("Fail to handle request with type of {}.", to_string(req_ty));
                    throw std::runtime_error("Fail to support HTTP type.");
                }
                case ReqTy::GET_Ty:
                {
                    spdlog::trace("Add handler for {}.", to_string(req_ty));
                    svr_.Get(handler.get_url_pattern(), handler.get_handler());
                    break;
                }
                case ReqTy::HEAD_Ty:
                {
                    spdlog::error("Fail to handle request with type of {}.", to_string(req_ty));
                    throw std::runtime_error("Fail to support HTTP type.");
                }
                case ReqTy::POST_Ty:
                {
                    spdlog::trace("Add handler for {}.", to_string(req_ty));
                    svr_.Post(handler.get_url_pattern(), handler.get_handler());
                    break;
                }
                case ReqTy::PUT_Ty:
                {
                    spdlog::trace("Add handler for {}.", to_string(req_ty));
                    svr_.Put(handler.get_url_pattern(), handler.get_handler());
                    break;
                }
                case ReqTy::DELETE_Ty:
                {
                    spdlog::trace("Add handler for {}.", to_string(req_ty));
                    svr_.Delete(handler.get_url_pattern(), handler.get_handler());
                    break;
                }
                case ReqTy::CONNECT_Ty:
                {
                    spdlog::error("Fail to handle request with type of {}.", to_string(req_ty));
                    throw std::runtime_error("Fail to support HTTP type.");
                }
                case ReqTy::OPTIONS_Ty:
                {
                    spdlog::trace("Add handler for {}.", to_string(req_ty));
                    svr_.Options(handler.get_url_pattern(), handler.get_handler());
                    break;
                }
                case ReqTy::TRACE_Ty:
                {
                    spdlog::error("Fail to handle request with type of {}.", to_string(req_ty));
                    throw std::runtime_error("Fail to support HTTP type.");
                }
                case ReqTy::PATCH_Ty:
                {
                    spdlog::trace("Add handler for {}.", to_string(req_ty));
                    svr_.Patch(handler.get_url_pattern(), handler.get_handler());
                    break;
                }
                default: {
                    spdlog::error("Fail to handle request with type of {}.", to_string(req_ty));
                    throw std::runtime_error("Fail to support HTTP type.");
                }
            }
        }
    }

    WebServer::WebServer(std::string host_addr, int port): host_addr_(std::move(host_addr)), port_(port), handlers_() {}

    WebServer::WebServer(const std::vector<UrlHandler>& handlers, std::string host_addr, int port): host_addr_(std::move(host_addr)), port_(port), handlers_(handlers) {}

    void WebServer::add_handler(UrlHandler handler) {
        handlers_.emplace_back(std::move(handler));
    }

    void WebServer::port(int port) { port_ = port; }
    int WebServer::port() const { return port_; }

    void WebServer::host_addr(std::string host_addr) { host_addr_ = std::move(host_addr); }
    std::string WebServer::host_addr() const { return host_addr_; }

    void WebServer::handlers(std::vector<UrlHandler> handlers) { handlers_ = std::move(handlers); }
    const std::vector<UrlHandler>& WebServer::handlers() const { return handlers_; }

    void WebServer::run() {
        apply_handler();
        svr_.listen(host_addr_, port_);
    }



} // namespace SandBox
