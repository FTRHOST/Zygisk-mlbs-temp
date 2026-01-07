#pragma once

#include "GlobalState.h"
#include "feature/BattleData.h"
#include <mutex>

// Core Logic Functions
void MonitorBattleState();
void UpdatePlayerInfo();
void UpdateBattleStats(); // New: For /infobattle and /timebattle
void UpdateBanPickState(); // New: For /banpick

// Helper to convert IDs to Strings (Mock or Real)
std::string HeroToString(int id);
std::string RankToString(int id, int star);
std::string SpellToString(int id);

// External global pointers
extern void* g_UIRankHero_Instance;

// Initialize hooks, offsets, etc.
void InitGameLogic();
