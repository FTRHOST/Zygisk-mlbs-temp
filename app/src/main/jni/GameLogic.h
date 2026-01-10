#pragma once

#include "GlobalState.h"
#include "feature/BattleData.h"
#include <mutex>

// Helper struct for local calculation/retrieval in GameLogic.cpp
// Re-using BattleGlobalStats logic essentially, but making sure the return type matches
struct BattleStats {
    uint32_t m_levelOnSixMin;
    uint32_t m_LevelOnTwelveMin;
    uint32_t m_KillNumCrossTower;
    uint32_t m_RevengeKillNum;
    uint32_t m_ExtremeBackHomeNum;
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

// Core Logic Functions
void MonitorBattleState();
void UpdatePlayerInfo();
void UpdateBattleStats(void* logicBattleManager); // Updated signature
void UpdateBanPickState();

// Helper to convert IDs to Strings (Mock or Real)
std::string HeroToString(int id);
std::string RankToString(int id, int star);
std::string SpellToString(int id);

// External global pointers
extern void* g_UIRankHero_Instance;

// Initialize hooks, offsets, etc.
void InitGameLogic();
