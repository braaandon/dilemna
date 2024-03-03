#pragma once

#include "../callback.h"

class Character : public Callback {
public:
    Character() : Callback("Character", "Drop incoming character data", 7501, 7509, Direction::Ingress) {}

    bool call(HANDLE hdl, Packet& pkt) override {
        return true;
    }
};