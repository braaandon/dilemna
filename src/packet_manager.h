#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <optional>

#include <winsock.h>

#include <windivert.h>

#include "packet.h"
#include "callback/callback.h"

class PacketManager {
public:
    void listen();

    template <typename LIMIT>
    void add_callback() {
        callbacks.emplace_back(std::make_unique<LIMIT>());
    }

private:
    bool initialise();

    std::optional<std::shared_ptr<Callback>> find_callback(int src, int dst);

    HANDLE handle;
    std::vector<std::shared_ptr<Callback>> callbacks;
};

