#pragma once
#include <cstdint>

// A clean struct to hold the battle statistics after they are read from memory.
struct BattleStats {
    int32_t iCampAKill;
    int32_t iCampBKill;
    uint32_t CampAGold;
    uint32_t CampBGold;
    uint32_t CampAExp;
    uint32_t CampBExp;
    uint32_t CampAKillTower;
    uint32_t CampBKillTower;
    uint32_t CampAKillLingZhu;
    uint32_t CampBKillLingZhu;
    uint32_t CampAKillShenGui;
    uint32_t CampBKillShenGui;
};

// This struct is a memory layout representation of the ShowFightDataTiny class
// based on the provided C# dump. It is used to correctly access the fields via offsets.
struct ShowFightDataTiny_Layout {
    char pad_0x0_0xe8[0xe8];
    int32_t m_iCampAKill;         // 0xe8
    int32_t m_iCampBKill;         // 0xec
    uint32_t m_CampAGold;         // 0xf0
    uint32_t m_CampBGold;         // 0xf4
    uint32_t m_CampAExp;          // 0xf8
    uint32_t m_CampBExp;          // 0xfc
    uint32_t m_CampAKillTower;    // 0x100
    uint32_t m_CampBKillTower;    // 0x104
    uint32_t m_CampAKillLingZhu;  // 0x108
    uint32_t m_CampBKillLingZhu;  // 0x10c
    uint32_t m_CampAKillShenGui;  // 0x110
    uint32_t m_CampBKillShenGui;  // 0x114
};

// Forward declaration for the function that reads the stats
BattleStats GetBattleStats();
