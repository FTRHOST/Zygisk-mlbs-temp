#pragma once

#include "GlobalState.h"
#include "feature/BattleData.h"
#include <mutex>

// Core Logic Functions
void MonitorBattleState();
void UpdatePlayerInfo();
void UpdateBattleStats(); // New: For /infobattle and /timebattle

// Helper to convert IDs to Strings (Mock or Real)
std::string HeroToString(int id);
std::string RankToString(int id, int star);
std::string SpellToString(int id);

// Initialize hooks, offsets, etc.
void InitGameLogic();
