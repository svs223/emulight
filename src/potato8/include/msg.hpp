#pragma once
#include <cstdint>
#include <variant>

#include "packet/key_press_packet.hpp"
#include "packet/framebuffer_update.hpp"

enum class MsgType {
    KeyStatusUpdate,
    FramebufferUpdate
};

enum class MsgTarget {
    CpuEvent,
    GpuEvent
};

typedef std::variant<KeyStatusPacket, FramebufferUpdatePacket> eventvar;

class Message {
    public:
        Message(eventvar, MsgType, MsgTarget);
        Message();
        //~Message();

        MsgType type;
        MsgTarget target;
        eventvar data;
};