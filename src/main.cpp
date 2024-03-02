#include <thread>

#include "menu.h"
#include "packet_manager.h"
#include "callback/impl/gamedata.h"
#include "callback/impl/reconnect.h"


int main() {
    Menu menu;
    State state;
    PacketManager manager;

    state.add_callback<GameData>();
    state.add_callback<Reconnect>();

    std::jthread renderer(&Menu::render, menu, std::ref(state));
    std::jthread listener(&PacketManager::listen, manager, std::ref(state));
    renderer.join();
    listener.join();
    return EXIT_SUCCESS;
}

