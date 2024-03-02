#pragma once

#include <vector>
#include <memory>
#include <optional>
#include <stop_token>

#include "callback/callback.h"

struct State {
    std::stop_source stopper;
    std::vector<std::shared_ptr<Callback>> callbacks;

    template<typename T>
    void add_callback() {
        callbacks.emplace_back(std::make_shared<T>());
    }

    std::optional<std::shared_ptr<Callback>> find_callback(int src, int dst) {
        for (auto& callback : callbacks) {
            if (callback->enabled) {
                if ((callback->direction == DIRECTION::INGRESS && callback->port_start <= src && callback->port_end >= src) ||
                    (callback->direction == DIRECTION::EGRESS && callback->port_start <= dst && callback->port_end >= dst)) {
                    return callback;
                }
            }
        }

        return std::nullopt;
    }
};