//
// Created by rainbowx on 24-6-10.
//

#ifndef ALIVEANSWER_HPP
#define ALIVEANSWER_HPP

#include <deque>
#include <mutex>

#include "SandBox.hpp"

namespace SandBox {
    constexpr time_t DEFAULT_TTL_MS = 60 * 60 * 1000;

    template<typename Ty>
    class AliveAnswer {
    private:
        time_t ttl_;
        size_t begin_idx_;
        std::mutex mutex_;
        std::deque<std::pair<time_t, Ty>> alive_list_;

        time_t getTime() {
            // std::scoped_lock<std::mutex> scoped_lock{mutex_};
            return time(nullptr);
        }

    public:
        explicit AliveAnswer(time_t ttl = DEFAULT_TTL_MS): begin_idx_(0), alive_list_(), ttl_(ttl) { }

        void cleanUp() {
            std::scoped_lock<std::mutex> scoped_lock{mutex_};

            time_t cur_time = getTime();

            while (not alive_list_.empty() and cur_time - alive_list_.front().first >= ttl_) alive_list_.pop_front(), begin_idx_++;
        }

        size_t append(Ty val) {
            std::scoped_lock<std::mutex> scoped_lock{mutex_};

            time_t cur_time = getTime();

            alive_list_.emplace_back(cur_time, std::move(val));

            return alive_list_.size() + begin_idx_ - 1;
        }

        Ty operator[] (size_t idx) {
            std::scoped_lock<std::mutex> scoped_lock{mutex_};
            ensure(idx >= begin_idx_ and idx < begin_idx_+alive_list_.size());

            auto [time, data] = alive_list_.at(idx - begin_idx_);
            return data;
        }
    };

} // SandBox

#endif //ALIVEANSWER_HPP
