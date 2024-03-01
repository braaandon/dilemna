#pragma once

#include "../packet.h"

class Callback {
public:
    int port_start{};
    int port_end{};
    DIRECTION direction{};

    bool enabled = false;

    virtual bool call(HANDLE hdl, Packet& pkt) = 0;
};