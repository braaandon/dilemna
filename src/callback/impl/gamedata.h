#pragma once

#include "../callback.h"

class GameData : public Callback {
public:
    GameData() : Callback("Game", "Drop incoming game data", 3074, 3074, Direction::INGRESS) {}

    bool call(HANDLE hdl, Packet& pkt) override {
        return true;
    }
};