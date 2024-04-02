# SandBox (未完成)
一个服务于OnlineJudge的代码沙箱

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[English](https://github.com/HM-GDTR/SandBox/blob/main/README.md) | **中文**


## 概述

一个安全，高性能，稳定的代码沙箱，将服务于课程设计的OnlineJudge。

## 功能

- (TODO) **基本功能**: 使用linux内核提供的seccomp和namespace机制, 限制程序运行权限
- (TODO) **灵活性**: 可以作为服务运行, 也可以单独运行
- (TODO) **易用性**: 适配各大主流语言
- (TODO) **可扩展性**: 使用c++编写, 预编译版本分发简便
- (TODO) **日志和监控**: 详细记录的日志,帮助分辨错误类型和位置

## 依赖
- [boost.asio](https://github.com/boostorg/asio)
- [nlohmann](https://github.com/nlohmann/json)
- [spdlog](https://github.com/gabime/spdlog)
- [tclap](https://github.com/mirror/tclap)

## 安装

```bash
$ sudo apt install libboost-all-dev
$ git clone --recursive https://github.com/HM-GDTR/SandBox.git
$ cd SandBox
$ cmake -DCMAKE_BUILD_TYPE=Debug -S . -B ./build
```

## 使用示例



## 文档

在此处提供更详细的文档链接或说明。

## 贡献

描述如何贡献给这个项目，包括但不限于如何提交 bug 报告、建议新功能或者参与开发。

## 许可证

该项目基于 MIT 许可证。详情请参阅 [LICENSE](LICENSE) 文件。

## 作者

- [RainbowX](https://h-blog.nkdns.cn/)

## 联系方式

- 如果有任何问题，可以联系 yan3260325524@outlook.com。
