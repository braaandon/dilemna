#pragma once

#include <string>

#include "../packet/packet.h"

class Callback {
public:
    const std::string name;
    const std::string tooltip;
    const int port_start;
    const int port_end;
    const Direction direction;

    int keybind = 0x0;
    bool enabled = false;

    Callback(std::string_view name, std::string_view tooltip, int port_start, int port_end, Direction direction) : name(name), tooltip(tooltip), port_start(port_start), port_end(port_end), direction(direction) {}
    virtual ~Callback() = default;

    virtual bool call(HANDLE hdl, Packet& pkt) = 0;
};