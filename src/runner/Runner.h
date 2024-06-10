//
// Created by rainbowx on 24-6-9.
//

#ifndef RUNNER_H
#define RUNNER_H

#include <mutex>

#include <json.hpp>

namespace SandBox {

constexpr size_t DEFAULT_MAX_TASK = 10;

using task_t = size_t;

class Runner {
private:
    // num of runing task
    size_t n_task_{};
    // max num of runing task
    size_t max_task_;
    // mutex to protect share variable
    std::mutex mutex_;

protected:
    /**
     * @brief Runs a task immediately using the specified task description.
     *
     * This function executes a single task immediately based on the provided task description.
     *
     * @param task_describe The description of the task to run, typically provided as a JSON object.
     *
     * @return The task info contains task_id, resource_usage etc.
     */
    static nlohmann::json run_single_task(const nlohmann::json& task_describe);

    /**
     * @brief Runs a child process according to the provided configuration.
     *
     * This function starts a child process based on the settings specified in the provided configuration.
     *
     * @param config A JSON object containing the configuration for the child process.
     *
     * @return The function should never return if the child process is successfully started, or -1 if the operation fails.
     */
    static int run_child(const nlohmann::json& config);

public:
    /**
     * @brief Constructs a Runner object with a specified maximum number of running tasks.
     *
     * This constructor initializes a Runner object, setting the maximum number of tasks
     * that can run concurrently to the given value. If no value is provided, the default
     * maximum number of tasks is used.
     *
     * @param max_task The maximum number of running tasks allowed. Defaults to DEFAULT_MAX_TASK.
     */
    explicit Runner(size_t max_task = DEFAULT_MAX_TASK);

    /**
     * @brief Adds a task to the wait queue and runs it.
     *
     * This function takes a JSON object that describes the task, including the
     * executable file path, arguments, and other relevant information. It adds
     * this task to the waiting queue and starts its execution.
     *
     * @param task_describe A JSON object containing the description of the task.
     *                       This includes details such as the executable file path
     *                       and its arguments. Default is an empty JSON object.
     *
     * @return An integer representing the task ID if successful, or -1 if the
     *         operation fails.
     */
    task_t add_task(nlohmann::json task_describe = {});

    /**
     * @brief Stops the task with the specified ID.
     *
     * This function stops the execution of the task identified by the given task ID.
     *
     * @param task_id The ID of the task to be stopped.
     *
     * @return 0 if the operation is successful, or -1 if it fails.
     */
    int stop_task(int task_id);

};

} // SandBox

#endif //RUNNER_H
