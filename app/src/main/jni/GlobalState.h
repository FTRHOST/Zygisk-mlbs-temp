#pragma once
#include <vector>
#include <string>
#include <mutex>
#include <stdint.h>

struct PlayerData {
    // Basic
    uint64_t ulUid;
    int iPos;
    uint32_t uiCampType;
    std::string sName;
    std::string sIconUrl;
    uint32_t uiHeroIDChoose;
    uint32_t uiRankLevel;

    // Detailed SystemData.RoomData fields
    bool bAutoConditionNew;
    bool bShowSeasonAchieve;
    uint32_t iStyleBoardId;
    uint32_t iMatchEffectId;
    uint32_t iDayBreakNo1Count;
    uint64_t bUid;
    bool bAutoReadySelect;
    bool bRobot;
    uint32_t heroid;
    uint32_t heroskin;
    uint32_t headID;
    uint32_t uiSex;
    uint32_t country;
    uint32_t uiZoneId;
    int32_t summonSkillId;
    int32_t runeId;
    // mapTalentTree, mRuneSkill2023, skinlist skipped for now (complex structures)
    int32_t runeLv;
    std::string facePath;
    uint32_t faceBorder;
    bool bStarVip;
    bool bMCStarVip;
    bool bMCStarVipPlus;
    uint64_t ulRoomID;
    uint64_t iConBlackRoomId;
    uint32_t banHero;
    // vCanSelectHero, vCanPickHero skipped
    int uiBattlePlayerType; // Enum as int
    std::string sThisLoginCountry;
    std::string sCreateRoleCountry;
    uint32_t uiLanguage;
    bool bIsOpenLive;
    uint64_t iTeamId;
    uint64_t iTeamNationId;
    std::string steamName;
    std::string steamSimpleName;
    uint32_t iCertify;
    // Lists skipped
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
    // mHeroMission etc skipped
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
    uint32_t uiHeroSkinIDChoose;
    uint32_t uiMapIDChoose;
    uint32_t uiMapSkinIDChoose;
    uint32_t uiDefenceRankScore;
    bool bBanChat;
    uint32_t iChatBanFinishTime;
    uint32_t iChatBanBattleNum;
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
