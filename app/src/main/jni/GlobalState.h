#pragma once

#include <string>
#include <vector>
#include <mutex>
#include <map>

// Info untuk satu pemain
struct PlayerData {
    bool bAutoConditionNew;
    bool bShowSeasonAchieve;
    uint32_t iStyleBoardId;
    uint32_t iMatchEffectId;
    uint32_t iDayBreakNo1Count;
    uint64_t lUid;
    uint64_t bUid;
    uint32_t iCamp;
    uint32_t iPos;
    bool bAutoReadySelect;
    std::string _sName;
    bool bRobot;
    uint32_t heroid;
    uint32_t heroskin;
    uint32_t headID;
    uint32_t uiSex;
    uint32_t country;
    uint32_t uiZoneId;
    int32_t summonSkillId;
    int32_t runeId;
    std::map<uint32_t, uint32_t> mapTalentTree;
    std::map<uint32_t, uint32_t> mRuneSkill2023;
    int32_t runeLv;
    std::vector<uint32_t> skinlist;
    std::string facePath;
    uint32_t faceBorder;
    bool bStarVip;
    bool bMCStarVip;
    bool bMCStarVipPlus;
    uint64_t ulRoomID;
    uint64_t iConBlackRoomId;
    uint32_t banHero;
    std::vector<uint32_t> vCanSelectHero;
    std::vector<uint32_t> vCanPickHero;
    uint32_t uiBattlePlayerType; // MTTDProto.BattlePlayerType
    std::string sThisLoginCountry;
    std::string sCreateRoleCountry;
    uint32_t uiLanguage;
    bool bIsOpenLive;
    uint64_t iTeamId;
    uint64_t iTeamNationId;
    std::string _steamName;
    std::string _steamSimpleName;
    uint32_t iCertify;
    std::vector<uint32_t> lsEffectSkins;
    std::vector<uint32_t> lsComEffSkins;
    std::vector<uint32_t> vMissions;
    uint32_t uiRankLevel;
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
    std::vector<std::string> vTitle;
    uint32_t mHeroMission; // MTTDProto.HeroMissionInfos
    std::vector<uint32_t> vEmoji;
    std::vector<uint32_t> vItemBuff;
    std::vector<uint32_t> vMapPaint;
    uint32_t mSkinPaint; // MTTDProto.BattleSkinPaintInfos
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
    std::map<uint32_t, uint32_t> mapBattleAttr;
    uint32_t iHeroUseCount;
    uint32_t iMythPoint;
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
    std::vector<uint32_t> vFastChat;
    std::vector<uint32_t> vWantS;

    // --- Legacy fields for compatibility ---
    std::string name;
    std::string uid;
    std::string rank;
    std::string spell;
    std::string heroName;
    int camp;
    int heroId;
    int spellId;
    int rankLevel;
};

// State global aplikasi
struct GlobalState {
    std::mutex stateMutex;
    int battleState = 0; // 0: Lobby, 2: Draft, 3: In-Game
    bool roomInfoEnabled = true;
    bool webServerEnabled = true;
    bool showMenu = true;
    std::vector<PlayerData> players;

    // Ban/Pick Data
    std::vector<int> banList;
    std::vector<int> pickList;
    int bpTimer = 0;
};

// Deklarasi instance global
extern GlobalState g_State;
extern bool g_IsWebServerReady;

// Fungsi untuk mengonversi state ke JSON
std::string StateToJson();

// Fungsi untuk mendapatkan waktu pertandingan
float GetBattleTime();
