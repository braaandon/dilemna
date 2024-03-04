#pragma once

#include <tuple>
#include <vector>
#include <memory>
#include <stop_token>

#include "callback/callback.h"

struct State {
    std::stop_source stopper;
    std::vector<std::shared_ptr<Callback>> callbacks;

    template<std::derived_from<Callback> T>
    void add_callback() {
        callbacks.emplace_back(std::make_shared<T>());
    }

    [[nodiscard]] std::shared_ptr<Callback> find_callback(DWORD keycode) {
        for (auto& callback : callbacks) {
            if (callback->keybind == keycode)
                return callback;
        }

        return nullptr;
    }

    [[nodiscard]] std::shared_ptr<Callback> find_callback(int src, int dst) {
        for (auto& callback : callbacks) {
            if ((callback->direction == Direction::Ingress && callback->port_start <= src && callback->port_end >= src) ||
                (callback->direction == Direction::Egress && callback->port_start <= dst && callback->port_end >= dst))
                return callback;
        }

        return nullptr;
    }
};