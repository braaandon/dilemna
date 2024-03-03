#pragma once

#include <windivert.h>

enum Direction {
    Ingress,
    Egress
};

struct Packet {
    UINT8 buffer[WINDIVERT_MTU_MAX];
    UINT buffer_len;
    WINDIVERT_ADDRESS addr;
    PWINDIVERT_IPHDR ip_header;
    PWINDIVERT_TCPHDR tcp_header;
    PWINDIVERT_UDPHDR udp_header;
};