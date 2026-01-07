#pragma once

#include <string>
#include <vector>
#include <mutex>
#include <map>
#include <chrono>

// Info untuk satu pemain (Room Data - Static/Pre-Game)
struct PlayerData {
    // Basic Info
    uint64_t lUid;
    uint64_t bUid;
    uint32_t iCamp;
    uint32_t iPos;
    std::string _sName;
    uint32_t heroid;
    uint32_t heroskin;
    int32_t summonSkillId; // Spell
    int32_t runeId;        // Emblem
    int32_t runeLv;
    uint32_t uiRankLevel;
    uint32_t iMythPoint;
    uint32_t uiZoneId;

    // Additional Details
    uint32_t banHero;
    bool bRobot;
    bool bNewPlayer;
    std::string rank;      // String representation
    std::string heroName;  // String representation
    std::string spell;     // String representation

    // Ban/Pick
    uint32_t uiHeroIDChoose; // Picked Hero ID

    // Legacy/Computed fields
    std::string name;
    std::string uid;
    int camp;
    int heroId;
    int spellId;
    int rankLevel;
};

// Real-time Battle Info for a Player (Dynamic)
struct PlayerBattleData {
    uint32_t heroid;
    int32_t iCamp;
    int32_t iPos;
    int32_t gold;
    int32_t kill;
    int32_t death;
    int32_t assist;
    // Potentially add items here later if needed
};

// Ban/Pick State
struct BanPickState {
    bool isOpen;
    uint32_t currentPhase; // 0: None, 1: Ban, 2: Pick
    uint32_t banTime;
    uint32_t pickTime;
    std::vector<uint32_t> banOrder; // Hero IDs
    std::vector<uint32_t> pickOrder; // Hero IDs
    std::map<uint32_t, uint32_t> banList; // HeroID -> ?
    std::map<uint32_t, uint32_t> pickList; // HeroID -> ?
    // Map to identify WHO banned/picked:
    // We will correlate index in list with iPos/iCamp from PlayerData
};

// Global Battle Stats (Team scores, Time)
struct BattleGlobalStats {
    float gameTime; // In seconds
    int32_t campAScore;
    int32_t campBScore;
    int32_t campAGold;
    int32_t campBGold;
    int32_t campAKillTower;
    int32_t campBKillTower;
    int32_t campAKillLord;
    int32_t campBKillLord;
    int32_t campAKillTurtle;
    int32_t campBKillTurtle;

    // Event Countdowns
    float lordRespawnTime;
    float turtleRespawnTime;
};

// State global aplikasi
struct GlobalState {
    std::mutex stateMutex;

    int battleState = 0; // 0: Lobby, 2: Draft, 3: In-Game, 6/7: Loading/Battle
    bool roomInfoEnabled = true;

    // Data Stores
    std::vector<PlayerData> players; // From SystemData.RoomData
    std::vector<PlayerBattleData> battlePlayers; // From Battle.FightPlayerData
    BanPickState banPickState;
    BattleGlobalStats battleStats;

    // Internal Timers
    std::chrono::steady_clock::time_point battleStartTime;
    std::chrono::duration<float> elapsedBattleTime;
};

// Deklarasi instance global
extern GlobalState g_State;
