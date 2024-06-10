//
// Created by rainbowx on 24-6-10.
//

#ifndef SECCOMP_RULE_H
#define SECCOMP_RULE_H
#include <string>
#include <functional>
#include <json.hpp>
#include <seccomp.h>
#include <utility>
#include <unordered_map>

namespace SandBox {
    class Rule {
        friend class SeccompRule;
        std::string rule_name;
        std::function<int(scmp_filter_ctx&)> rule_descriptor;

    public:
        Rule(std::string rule_name, const std::function<int(scmp_filter_ctx&)> &rule_descriptor)
            : rule_name(std::move(rule_name)),
              rule_descriptor(rule_descriptor) {
        }
    };

    class SeccompRule {
    private:
        scmp_filter_ctx ctx;
        nlohmann::json context;
        std::unordered_map<std::string, std::function<int(scmp_filter_ctx&)>> rule_map;

    public:
        explicit SeccompRule(nlohmann::json context = {}, uint32_t default_act = SCMP_ACT_KILL);

        int rule_register(const Rule& rule);

        int apply_rule(std::string rule_name);
    };

} // SandBox

#endif //SECCOMP_RULE_H
