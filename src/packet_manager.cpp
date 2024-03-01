#include "packet_manager.h"

bool PacketManager::initialise() {
    handle = WinDivertOpen("(remotePort >= 7500 && remotePort <= 7509) || (remotePort >= 30000 && remotePort <= 30009) || (remotePort >= 27015 && remotePort <= 27200) || (remotePort == 3074)", WINDIVERT_LAYER_NETWORK, 1, 0);
    return handle != INVALID_HANDLE_VALUE;
}

void PacketManager::listen() {
    if (!initialise()) {
        std::cout << "init failed\n";
        return;
    }

    Packet pkt;

    while(WinDivertRecv(handle, pkt.buffer, sizeof(pkt.buffer), &pkt.buffer_len, &pkt.addr)) {
        WinDivertHelperParsePacket(pkt.buffer,
                                   pkt.buffer_len,
                                   &pkt.ip_header,
                                   nullptr,
                                   nullptr,
                                   nullptr,
                                   nullptr,
                                   &pkt.tcp_header,
                                   &pkt.udp_header,
                                   nullptr,
                                   nullptr,
                                   nullptr,
                                   nullptr);

        if (pkt.ip_header == nullptr) {
            WinDivertSend(handle, pkt.buffer, pkt.buffer_len, nullptr, &pkt.addr);
            continue;
        }

        bool cancelled = false;
        std::optional<std::shared_ptr<Callback>> cb{};

        if (pkt.tcp_header != nullptr) cb = find_callback(htons(pkt.tcp_header->SrcPort), htons(pkt.tcp_header->DstPort));
        if (pkt.udp_header != nullptr) cb = find_callback(htons(pkt.udp_header->SrcPort), htons(pkt.udp_header->DstPort));
        if (cb.has_value()) cancelled = (*cb)->call(handle, pkt);
        if (!cancelled) WinDivertSend(handle, pkt.buffer, sizeof(pkt.buffer), nullptr, &pkt.addr);
    }
}

std::optional<std::shared_ptr<Callback>> PacketManager::find_callback(int src, int dst) {
    for (auto& callback : callbacks) {
        if (callback->enabled) {
            if (callback->direction == DIRECTION::INGRESS && callback->port_start <= src && callback->port_end >= src) {
                return callback;
            }

            if (callback->direction == DIRECTION::EGRESS && callback->port_start <= dst && callback->port_end >= dst) {
                return callback;
            }
        }
    }

    return std::nullopt;
}
