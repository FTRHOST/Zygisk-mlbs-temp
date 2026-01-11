#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include "IpcServer.h"

enum EventType {
    EVENT_GAME_START = 0,
    EVENT_GOLD_UPDATE = 1,
    EVENT_KILL_UPDATE = 2,
    EVENT_TOWER_DESTROY = 3,
    EVENT_BAN_PICK = 4
};

class Broadcaster {
public:
    static void SendEvent(EventType type, std::string data) {
        std::stringstream ss;
        ss << "{\"type\":\"event\",\"event_id\":" << type << ",\"data\":" << data << "}";
        BroadcastData(ss.str());
    }
};
