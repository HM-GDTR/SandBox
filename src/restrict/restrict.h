//
// Created by rainbowx on 24-6-10.
//

#ifndef RESTRICT_H
#define RESTRICT_H

#include <json.hpp>
#include "spdlog/spdlog.h"

namespace SandBox {

    #define ERROR_EXIT(error_code)\
        {\
            spdlog::critical("Error: System errno: {}; Internal errno: "#error_code, strerror(errno)); \
            raise(SIGUSR2);  \
            exit(EXIT_FAILURE); \
        }

    /**
     * @brief Applies restrictions based on the provided configuration.
     *
     * This function enforces certain restrictions as specified in the provided configuration.
     *
     * @param config A JSON object containing the configuration settings for the restrictions.
     *
     * @return 0 if the restrictions are successfully applied, or -1 if the operation fails.
     */
    int do_restricts(const nlohmann::json& config);

} // SandBox

#endif //RESTRICT_H
