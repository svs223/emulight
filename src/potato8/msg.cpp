#include "msg.hpp"
#include <cstdint>
#include <cstring>

Message::Message(eventvar packet, MsgType msgType, MsgTarget msgTarget)
{
    data = packet;
    type = msgType;
    target = msgTarget;
}

Message::Message()
{
}
