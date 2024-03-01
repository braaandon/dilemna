#include "packet_manager.h"

#include "callback/impl/gamedata.h"
#include "callback/impl/reconnect.h"

int main() {
    PacketManager manager{};
    manager.add_callback<GameData>();
    manager.add_callback<Reconnect>();
    manager.listen();
    return 0;
}
