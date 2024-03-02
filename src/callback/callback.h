#pragma once

#include <string>

#include "../packet/packet.h"

class Callback {
public:
    std::string name;
    std::string tooltip;
    int port_start;
    int port_end;
    DIRECTION direction;

    bool enabled = false;

    virtual bool call(HANDLE hdl, Packet& pkt) = 0;
};