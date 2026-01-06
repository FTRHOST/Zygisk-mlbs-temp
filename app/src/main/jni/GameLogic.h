#pragma once

#include "GlobalState.h"
#include "feature/BattleData.h"
#include <mutex>

void MonitorBattleState();
void UpdatePlayerInfo();
BattleStats GetBattleStats();

// Initialize hooks, offsets, etc.
void InitGameLogic();
void InitUIRankHeroHook();
