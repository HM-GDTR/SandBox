//
// Created by RainbowX on 24-4-9.
//

#ifndef SANDBOX_THREADINFO_HPP
#define SANDBOX_THREADINFO_HPP

#include <mutex>
#include <string>
#include <pthread.h>
#include <shared_mutex>
#include <unordered_map>
#include <source_location>

class ThreadInfo {
public:
    ThreadInfo(): mutex_lock(), pid_name_map() { }

    template<typename Ty>
    void regThread(pid_t pid, Ty name) {
        std::scoped_lock<std::shared_mutex> _(mutex_lock);
        pid_name_map[pid].assign(name);
    }

    std::string getThreadName(pid_t pid) {
        mutex_lock.lock_shared();
        if(pid_name_map.contains(pid)) return pid_name_map[pid];
        mutex_lock.unlock_shared();
        return "Unnamed Thread@" + std::to_string(pid);
        //return "Unknown Thread";
    }
private:
    std::shared_mutex mutex_lock;
    std::unordered_map<pid_t, std::string> pid_name_map;
};
extern ThreadInfo threadInfo;

#endif //SANDBOX_THREADINFO_HPP
