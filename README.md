# SandBox (not complete)
A code SandBox for Online Judge.

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
**English** | [中文](https://github.com/HM-GDTR/SandBox/blob/main/README_cn.md)


## Overview
A secure, high-performance, and stable code sandbox designed to serve the OnlineJudge for course projects.

## Features
- (TODO) **Basic Functionality**: Restricts program execution permissions using seccomp and namespace mechanisms provided by the Linux kernel.
- (TODO) **Flexibility**: Can be run as a service or standalone.
- (TODO) **Ease of Use**: Adaptable to major programming languages.
- (TODO) **Scalability**: Written in C++, easy distribution with precompiled versions.
- (TODO) **Logging and Monitoring**: Detailed logging to help identify error types and locations.

## Dependencies
- [boost.asio](https://github.com/boostorg/asio)
- [nlohmann](https://github.com/nlohmann/json)
- [spdlog](https://github.com/gabime/spdlog)
- [tclap](https://github.com/mirror/tclap)

## Installation
```bash
$ sudo apt install libboost-all-dev
$ git clone --recursive https://github.com/HM-GDTR/SandBox.git
$ cd SandBox
$ cmake -DCMAKE_BUILD_TYPE=Debug -S . -B ./build
```

## Usage Example
<!-- Provide usage examples here -->

## Documentation
For detailed documentation, please refer to the documentation within the project or a provided link.

## Contribution
You can contribute to this project by submitting bug reports, suggesting new features, or participating in development.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Author
- [RainbowX](https://h-blog.nkdns.cn/)

## Contact
For any inquiries, please contact yan3260325524@outlook.com.