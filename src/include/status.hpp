//
// Created by ubuntu on 24-3-28.
//

#ifndef SANDBOX_STATUS_HPP
#define SANDBOX_STATUS_HPP

/**
 * 沙箱状态
 */
enum class SandBoxStatus{
    NoError = 0,
    ForkError,
    ExecError,
    RestrictError,
};

/**
 * 代码状态
 */
enum class CodeStatus {
    Accepted = 0,

    // 编译阶段的错误
    CompilationError,

    // 运行阶段的错误
    RuntimeError,
    TimeLimitExceedError,
    MemoryLimitExceedError,
    RestrictedError,    //调用了不该调用的函数

    // 验证阶段的错误
    WrongAnswerError,
    PresentationError,
    OutputExceedError,

    // 内部错误
    InternalError,
};

#endif //SANDBOX_STATUS_HPP
