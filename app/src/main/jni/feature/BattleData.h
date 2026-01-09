#pragma once
#include <cstdint>
#include <vector>
#include <map>
#include <string>

// Raw fields from ShowFightDataTiny as requested
struct BattleStats {
    uint32_t m_levelOnSixMin;
    uint32_t m_LevelOnTwelveMin;
    // Lists/Maps are complex to read directly via simple layout cast,
    // so we will skip complex containers in the simple layout struct
    // and just include primitives that are safe to read via offsets.
    // If needed, we can read them via Il2Cpp helper functions later.

    // Primitive fields from dump:
    uint32_t m_KillNumCrossTower;
    uint32_t m_RevengeKillNum;
    uint32_t m_ExtremeBackHomeNum;

    // Boolean fields (layout depends on packing, usually 1 byte)
    bool bLockGuidChanged;

    uint32_t m_BackHomeCount;
    uint32_t m_RecoverSuccessfullyCount;

    uint32_t m_BuyEquipCount;
    float m_BuyEquipTime;

    uint32_t m_uSurvivalCount;
    uint32_t m_uPlayerCount;

    int32_t m_iCampAKill;
    int32_t m_iCampBKill;
    uint32_t m_CampAGold;
    uint32_t m_CampBGold;
    uint32_t m_CampAExp;
    uint32_t m_CampBExp;
    uint32_t m_CampAKillTower;
    uint32_t m_CampBKillTower;
    uint32_t m_CampAKillLingZhu;
    uint32_t m_CampBKillLingZhu;
    uint32_t m_CampAKillShenGui;
    uint32_t m_CampBKillShenGui;
    uint32_t m_CampAKillLingzhuOnSuperior;
    uint32_t m_CampBKillLingzhuOnSuperior;
    uint32_t m_CampASuperiorTime;
    uint32_t m_CampBSuperiorTime;
    uint32_t m_iFirstBldTime;
    uint32_t m_iFirstBldKiller;
};

// Layout for ShowFightDataTiny based on offsets provided in dump.
// We only map the primitive fields we need.
// Complex types (Lists/Dicts) are just pointers (void*) in the layout.
struct ShowFightDataTiny_Layout {
    // Fields start
    uint32_t m_levelOnSixMin;           // 0x10
    uint32_t m_LevelOnTwelveMin;        // 0x14
    void* m_EmojiCarryList;             // 0x18
    void* m_TDFighteData;               // 0x20
    void* m_DeathInfoList;              // 0x28
    void* m_DeathAttackInfoDict;        // 0x30
    void* m_lNotLinkEffect;             // 0x38
    void* m_dicKeyCancelDis;            // 0x40
    void* m_KillerCount;                // 0x48
    void* m_FighterDyData;              // 0x50
    uint32_t m_KillNumCrossTower;       // 0x58
    uint32_t m_RevengeKillNum;          // 0x5c
    uint32_t m_ExtremeBackHomeNum;      // 0x60
    void* m_selfBeAttackTIme;           // 0x68
    void* m_heroNumAroundSelf;          // 0x70
    void* m_EnemyhurtSelf;              // 0x78
    uint32_t lastLockGuid;              // 0x80
    bool bLockGuidChanged;              // 0x84
    char pad_0x85[3];                   // Padding alignment
    uint32_t m_BackHomeCount;           // 0x88
    uint32_t m_RecoverSuccessfullyCount;// 0x8c
    void* m_ReplaceHeroSkill;           // 0x90
    void* m_arenaWinVoice;              // 0x98
    void* m_arenaLoseVoice;             // 0xa0
    uint32_t m_BuyEquipCount;           // 0xa8
    float m_BuyEquipTime;               // 0xac
    void* m_BannedList;                 // 0xb0
    void* m_VoiceBannedList;            // 0xb8
    void* m_ForbidTalkList;             // 0xc0
    void* m_BuyEquipTimes;              // 0xc8
    void* m_GreatIDs;                   // 0xd0
    void* m_FighterSplitEnergyBar;      // 0xd8
    // Note: Offsets jump here in dump, checking alignment
    // Dump says 0xd8 is ptr (8 bytes on 64bit), so next is 0xe0
    uint32_t m_uSurvivalCount;          // 0xe0
    uint32_t m_uPlayerCount;            // 0xe4
    int32_t m_iCampAKill;               // 0xe8
    int32_t m_iCampBKill;               // 0xec
    uint32_t m_CampAGold;               // 0xf0
    uint32_t m_CampBGold;               // 0xf4
    uint32_t m_CampAExp;                // 0xf8
    uint32_t m_CampBExp;                // 0xfc
    uint32_t m_CampAKillTower;          // 0x100
    uint32_t m_CampBKillTower;          // 0x104
    uint32_t m_CampAKillLingZhu;        // 0x108
    uint32_t m_CampBKillLingZhu;        // 0x10c
    uint32_t m_CampAKillShenGui;        // 0x110
    uint32_t m_CampBKillShenGui;        // 0x114
    uint32_t m_CampAKillLingzhuOnSuperior; // 0x118
    uint32_t m_CampBKillLingzhuOnSuperior; // 0x11c
    uint32_t m_CampASuperiorTime;       // 0x120
    uint32_t m_CampBSuperiorTime;       // 0x124
    uint32_t m_iFirstBldTime;           // 0x128
    uint32_t m_iFirstBldKiller;         // 0x12c
};

BattleStats GetBattleStats();
