#pragma once

#include "msg.hpp"

class FramebufferUpdatePacket {
    public:
        uint32_t fb[64 * 32];
};