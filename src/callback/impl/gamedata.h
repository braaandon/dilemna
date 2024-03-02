#pragma once

#include "../callback.h"

class GameData : public Callback {
public:
    GameData() {
        name = "Game";
        tooltip = "Drop incoming game data";
        port_start = 3074;
        port_end = 3074;
        direction = DIRECTION::INGRESS;
    }

    bool call(HANDLE hdl, Packet& pkt) override {
        return true;
    }
};