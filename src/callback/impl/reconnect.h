#pragma once

#include "../callback.h"

class Reconnect : public Callback {
public:
    struct {
        WINDIVERT_IPHDR ip;
        WINDIVERT_TCPHDR tcp;
    } reset;

    Reconnect() {
        name = "Reconnect";
        tooltip = "Reconnect to the instance";
        port_start = 30000;
        port_end = 30009;
        direction = DIRECTION::INGRESS;

        ZeroMemory(&reset, sizeof(reset));

        reset.ip.Version = 4;
        reset.ip.HdrLength = sizeof(WINDIVERT_IPHDR) / sizeof(UINT32);
        reset.ip.Length = htons(sizeof(reset));
        reset.ip.TTL = 64;
        reset.ip.Protocol = IPPROTO_TCP;
        reset.tcp.HdrLength = sizeof(WINDIVERT_TCPHDR) / sizeof(UINT32);

        reset.tcp.Rst = 1;
        reset.tcp.Ack = 1;
    }

    bool call(HANDLE hdl, Packet& pkt) override {
        enabled = false;

        reset.ip.SrcAddr = pkt.ip_header->SrcAddr;
        reset.ip.DstAddr = pkt.ip_header->DstAddr;
        reset.tcp.SrcPort = pkt.tcp_header->SrcPort;
        reset.tcp.DstPort = pkt.tcp_header->DstPort;
        reset.tcp.SeqNum = pkt.tcp_header->SeqNum;
        reset.tcp.AckNum = pkt.tcp_header->AckNum;

        WinDivertHelperCalcChecksums((PVOID)&reset, sizeof(reset), &pkt.addr, 0);
        WinDivertSend(hdl, (PVOID)&reset, sizeof(reset), nullptr, &pkt.addr);
        return true;
    }
};