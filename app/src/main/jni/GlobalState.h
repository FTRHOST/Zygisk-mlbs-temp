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

    // Raw Fields Requested by User
    bool bAutoConditionNew;
    bool bShowSeasonAchieve;
    uint32_t iStyleBoardId;
    uint32_t iMatchEffectId;
    uint32_t iDayBreakNo1Count;
    bool bAutoReadySelect;
    bool bRobot;
    uint32_t headID;
    uint32_t uiSex;
    uint32_t country;
    std::string facePath;
    uint32_t faceBorder;
    bool bStarVip;
    bool bMCStarVip;
    bool bMCStarVipPlus;
    uint64_t ulRoomID;
    uint64_t iConBlackRoomId;
    uint32_t banHero;
    uint32_t uiBattlePlayerType;
    std::string sThisLoginCountry;
    std::string sCreateRoleCountry;
    uint32_t uiLanguage;
    bool bIsOpenLive;
    uint64_t iTeamId;
    uint64_t iTeamNationId;
    std::string _steamName;
    std::string _steamSimpleName;
    uint32_t iCertify;
    uint32_t uiPVPRank;
    bool bRankReview;
    uint32_t iElo;
    uint32_t uiRoleLevel;
    bool bNewPlayer;
    uint32_t iRoad;
    uint32_t uiSkinSource;
    uint32_t iFighterType;
    uint32_t iWorldCupSupportCountry;
    uint32_t iHeroLevel;
    uint32_t iHeroSubLevel;
    uint32_t iHeroPowerLevel;
    uint32_t iActCamp;
    // Lists are hard to serialize in C++ raw struct, we'll skip complex lists for now unless critical
    uint32_t mHeroMission;
    uint32_t mSkinPaint;
    std::string sClientVersion;
    uint32_t uiHolyStatue;
    uint32_t uiKamon;
    uint32_t uiUserMapID;
    uint32_t iSurviveRank;
    uint32_t iDefenceRankID;
    uint32_t iLeagueWCNum;
    uint32_t iLeagueFCNum;
    uint32_t iMPLCertifyTime;
    uint32_t iMPLCertifyID;
    uint32_t iHeroUseCount;
    bool bMythEvaled;
    uint32_t iDefenceFlag;
    uint32_t iDefenPoint;
    uint32_t iDefenceMap;
    uint32_t iAIType;
    uint32_t iAISeed;
    std::string sAiName;
    uint32_t iWarmValue;
    uint32_t uiAircraftIDChooose;
    uint32_t uiHeroIDChoose;
    uint32_t uiHeroSkinIDChoose;
    uint32_t uiMapIDChoose;
    uint32_t uiMapSkinIDChoose;
    uint32_t uiDefenceRankScore;
    bool bBanChat;
    uint32_t iChatBanFinishTime;
    uint32_t iChatBanBattleNum;

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
    uint64_t uGuid; // To match with RoomData
    std::string playerName;
    int32_t campType;
    uint32_t kill;
    uint32_t death;
    uint32_t assist;
    uint32_t gold;
    uint32_t totalGold;
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
    std::vector<PlayerBattleData> battlePlayers; // From BattleData.heroInfoList
    BanPickState banPickState;
    BattleGlobalStats battleStats;

    // Internal Timers
    std::chrono::steady_clock::time_point battleStartTime;
    std::chrono::duration<float> elapsedBattleTime;
};

// Deklarasi instance global
extern GlobalState g_State;
