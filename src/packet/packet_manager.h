#pragma once

#include <iostream>

#include <winsock.h>

#include <windivert.h>

#include "packet.h"
#include "../state.h"

class PacketManager {
public:
    void listen(State& state);

private:
    bool initialise();
    void shutdown();

    HANDLE handle;
};

