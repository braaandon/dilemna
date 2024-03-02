#pragma once

#include "../callback.h"

class Players : public Callback {
public:
    Players() {
        name = "Players";
        tooltip = "Drop incoming data from other players";
        port_start = 27015;
        port_end = 27200;
        direction = DIRECTION::INGRESS;
    }

    bool call(HANDLE hdl, Packet& pkt) override {
        return true;
    }
};