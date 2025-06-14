#pragma once

#include <cstdint>

class KeyStatusPacket {
    public:
        uint8_t key;
        bool status;
};