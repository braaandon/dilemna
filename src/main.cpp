#include <thread>

#include "menu.h"
#include "keyhandler.h"
#include "packet/packet_manager.h"
#include "callback/impl/players.h"
#include "callback/impl/gamedata.h"
#include "callback/impl/reconnect.h"
#include "callback/impl/character.h"

int main() {
    Menu menu;
    State state;
    PacketManager manager;

    state.add_callback<GameData>();
    state.add_callback<Character>();
    state.add_callback<Players>();
    state.add_callback<Reconnect>();

    KeyHandler::state = &state;

    std::jthread renderer(&Menu::render, menu, std::ref(state));
    std::jthread listener(&PacketManager::listen, manager, std::ref(state));
    renderer.join();
    listener.join();
    return EXIT_SUCCESS;
}

