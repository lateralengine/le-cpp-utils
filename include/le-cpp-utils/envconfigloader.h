#pragma once

#include <libenvpp/env.hpp>
#include <string>
#include <algorithm>

namespace le
{
    template<typename ConfigType, typename RegisterFn, typename ExtractFn>
    bool load_env_config(const std::string& prefix_str,
                        const RegisterFn& register_fn,
                        const ExtractFn& extract_fn,
                        ConfigType& config_out,
                        std::string* error_msg_out = nullptr)
    {
        auto prefix = env::prefix(prefix_str);

        register_fn(prefix);  // Register variables

        const auto parsed = prefix.parse_and_validate();
        if (!parsed.ok()) {
            if (error_msg_out) {
                *error_msg_out = parsed.warning_message() + parsed.error_message();
            }
            return false;
        }

        extract_fn(parsed, config_out);  // Fill the config struct
        return true;
    }

} // namespace le
