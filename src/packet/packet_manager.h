#pragma once

#include <iostream>

#include <winsock.h>

#include <windivert.h>

#include "packet.h"
#include "../state.h"

class PacketManager {
public:
    PacketManager();
    ~PacketManager();

    void listen(State& state);

private:
    HANDLE handle;
};

