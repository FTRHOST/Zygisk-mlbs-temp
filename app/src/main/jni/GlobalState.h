#pragma once
#include <vector>
#include <string>
#include <mutex>
#include <stdint.h>

struct PlayerData {
    uint64_t ulUid;
    int iPos;
    uint32_t uiCampType;
    std::string sName;
    std::string sIconUrl;
    uint32_t uiHeroIDChoose;
    uint32_t uiRankLevel;

    // Backwards compatibility
    std::string heroName;
    std::string rank;
};

struct RoomState {
    int iRoomType;
    int iRoomState;
    int iActiveType;
    uint32_t uiCampType;
    uint64_t ulRoomID;
    uint64_t ulLeaderID;
    uint64_t ulCreatorID;
    int iMaxUserCount;
    std::vector<PlayerData> players;
};

struct BanPickState {
    std::vector<uint32_t> banList;
    std::vector<uint32_t> pickList;
};

struct PlayerBattleData {
    uint32_t m_uGuid;
    uint32_t m_KillNum;
    uint32_t m_DeadNum;
    uint32_t m_AssistNum;
    uint32_t m_Gold;
    uint32_t m_TotalGold;
    uint32_t m_Level;
    uint32_t m_CampType;
    uint32_t m_HurtHeroValue;
    uint32_t m_HurtTowerValue;
    uint32_t m_InjuredValue;
    uint32_t m_DestroyTowerNum;
    uint32_t m_MonsterCoin;
    uint32_t iKill3;
    uint32_t iKill4;
    uint32_t iKill5;
};

struct BattleGlobalStats {
    uint32_t m_uiGameTime;
    uint32_t m_CampAKill;
    uint32_t m_CampBKill;
    uint32_t m_CampAGold;
    uint32_t m_CampBGold;
    uint32_t m_CampAExp;
    uint32_t m_CampBExp;
    uint32_t m_CampAKillTower;
    uint32_t m_CampBKillTower;
    uint32_t m_CampAKillLingZhu; // Lord
    uint32_t m_CampBKillLingZhu;
    uint32_t m_CampAKillShenGui; // Turtle
    uint32_t m_CampBKillShenGui;

    std::vector<PlayerBattleData> playerStats;
};

struct GlobalState {
    RoomState roomState;
    BanPickState banPickState;
    BattleGlobalStats battleStats;
    std::mutex stateMutex;
    bool roomInfoEnabled = true;
};

extern GlobalState g_State;
