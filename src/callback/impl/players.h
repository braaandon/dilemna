#pragma once

#include "../callback.h"

class Players : public Callback {
public:
    Players() : Callback("Players", "Drop incoming data from other players", 27015, 27200, Direction::INGRESS) {}

    bool call(HANDLE hdl, Packet& pkt) override {
        return true;
    }
};