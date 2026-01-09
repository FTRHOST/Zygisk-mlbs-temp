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
    std::string _sName; // This is the "fungsi asli" name field requested
    uint32_t heroid;
    uint32_t heroskin;
    int32_t summonSkillId; // Spell
    int32_t runeId;        // Emblem
    int32_t runeLv;
    uint32_t uiRankLevel;
    uint32_t iMythPoint;
    uint32_t uiZoneId;

    // --- FULL RAW FIELDS FROM USER REQUEST ---
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
    // Maps/Lists (Stored as addresses or simplified vectors if possible)
    uintptr_t mapTalentTree_Ptr;
    uintptr_t mRuneSkill2023_Ptr;
    uintptr_t skinlist_Ptr;
    std::string facePath;
    uint32_t faceBorder;
    bool bStarVip;
    bool bMCStarVip;
    bool bMCStarVipPlus;
    uint64_t ulRoomID;
    uint64_t iConBlackRoomId;
    uint32_t banHero;
    uintptr_t vCanSelectHero_Ptr;
    uintptr_t vCanPickHero_Ptr;
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
    uintptr_t lsEffectSkins_Ptr;
    uintptr_t lsComEffSkins_Ptr;
    uintptr_t vMissions_Ptr;
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
    uintptr_t vTitle_Ptr;
    uint32_t mHeroMission;
    uintptr_t vEmoji_Ptr;
    uintptr_t vItemBuff_Ptr;
    uintptr_t vMapPaint_Ptr;
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
    uintptr_t mapBattleAttr_Ptr;
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
    uintptr_t vFastChat_Ptr;
    uintptr_t vWantSelectHero_Ptr;
    uint32_t mapHeroCareerNumInfo_Ptr; // Struct/Class pointer
    uint32_t vRecentRoadInfo_Ptr;      // Struct/Class pointer
    bool bTeamMCLChampion;
    uintptr_t vUnLockHeroInRank_Ptr;
    uint32_t uiEquipSuit;
    uintptr_t vOperBanHero_Ptr;
    uint32_t iHeroEnhanceLevel;
    uint32_t uiMagicChessCupNum;
    uint32_t uiMagicRankID;
    uintptr_t chessHeroSkin_Ptr;
    uint32_t mChessHeroSkin2_Ptr;
    uint32_t uiAerocraftId;
    uint32_t uiAerocraftSkinId;
    uint32_t uiCupCount;
    uint32_t uiBattleCount;
    uint32_t heroShowSkin;
    uint32_t uiMatchScore;
    uint32_t iStarVipSkinRank;
    uintptr_t vSkinAction_Ptr;
    uint32_t uiBattleAIPerformanceScore;
    uint32_t uiLocalAIPerformanceScore;
    uint32_t uiTemperatureGrade;
    uint32_t mBattleAILimitInfos_Ptr;
    uint32_t mAutoEmojiInfos_Ptr;
    bool bBattleAIWhilteList;
    bool bOpenTryRune;
    uint32_t uiBattleAIDiffculty;
    uint32_t uiBattleAIDiffcultyMin;
    uint32_t uiBattleAIDiffcultyMax;
    bool bWarmBattleAIRobot;
    uint32_t uiRobotAICompany;
    uint32_t uiPlayerFov;
    uint32_t uiCameraProjection;
    uint32_t uiArenaRankID;
    uint32_t uiArenaCupNum;
    bool bFaceHD;
    bool bLuckyDogFreeHero;
    bool bLuckyDogFreeSkin;
    uint32_t iHistoryMaxBigRankId;
    uintptr_t mCommanderSkinAnimationChooose_Ptr;
    uint32_t mMagicChessInteractiveGift_Ptr;
    uint32_t mArenaCard_Ptr;
    uint32_t uiArenaBigRankID;
    bool bDropSpecialItemInDayBreak;
    uint32_t stCompanyBattleAI_Ptr;
    bool bFakeRole;
    bool bVP;
    uint32_t uiCloneImbaDisorderSpecialResource;
    uint32_t mFriendIntimacy_Ptr;
    uintptr_t vFriendShareHero_Ptr;
    uintptr_t vPartyEffect_Ptr;
    bool bInspire;
    uint32_t uiRandomScroeItem;
    uint32_t iPartyTitle;
    bool bRoomLeader;
    uintptr_t vExtraSummons_Ptr;
    uint32_t newRuneAttr_Ptr;
    bool bForbidUseFaceName;
    std::string sClientIp;
    uint32_t iRoomOrder;
    uintptr_t vRougeTotalSkill_Ptr;
    uintptr_t vRougeOMGSkill_Ptr;
    uintptr_t vRecommendEquipList_Ptr;
    std::string sRecommendEquipVersion;
    uintptr_t vPingParamDetail_Ptr;
    uint32_t uiPlayerPing;
    uint32_t mSkinRankSeasonTag_Ptr;
    uint32_t mSkinNumTag_Ptr;
    bool bFullSkillaber;
    uint32_t uiCommanderSkinAttackEffect;
    uint32_t uiDailyFreeRandomNum;
    bool bIllustrateCornerEffectClose;
    bool bTagedBackOf2022;
    uint32_t iTapConflictTipNum;
    uint32_t iNameShowType;
    bool bOpenHighLight;
    uint32_t mMCBanPickCommander_Ptr;
    uintptr_t vForbidBanCommander_Ptr;
    uint32_t iTeamLevel;
    uintptr_t vAdditionalHero_Ptr;
    uint32_t uiDisorderPublicHeroScore;
    bool bPlayerBirthdayToday;
    uint32_t iTeamHeadId;
    uintptr_t mapHeroBattleNum_Ptr;
    uintptr_t vCurSeasonRealRoadInfo_Ptr;
    uintptr_t vCultivateRoadShow_Ptr;
    uint32_t uiCommanderLevel;
    bool bOpenSubRankID;
    uint32_t iSubRankID;
    uint32_t iSingleLv;
    uint32_t stArenaMatchBattleInfo_Ptr;
    uint32_t stArenaMatchShowInfo_Ptr;
    uint32_t stSkinAttach_Ptr;
    uint64_t iMatchTeamId;
    uint32_t iFlowBackTYpe;
    bool bRoadAdditionCover;
    uint32_t iRoadAdditionCoverTimes;
    uint32_t iRoomPos;
    uint32_t stEasterEggInfo_Ptr;
    std::string sMatchTeamName;
    uint32_t iMatchTeamFaceId;
    uint32_t iMatchPhaseId;
    uint32_t stLaneMatch_Ptr;
    uintptr_t vAllPlayerTitle_Ptr;
    uint32_t iTeamMatchBONum;
    uint32_t iTeamMatchRank;
    uint32_t stMCTreeAILineup_Ptr;
    int32_t dragonCrystalId;
    uint64_t iBoolHolder;
    uint32_t iRoguelikeTeamID;
    uint32_t stHonorAbout_Ptr;
    uint32_t stBattleChuoChuoInfo_Ptr;
    uint32_t iChatEffectId;
    uint64_t iMirrorBattleID;
    uintptr_t vFiveFreeSkin_Ptr;
    uint32_t uiCurtainId;
    uint64_t ulFakeRoleRoomID;
    uintptr_t vDisOrderUnlockHero_Ptr;
    uint32_t stNameDisplay_Ptr;
    uint32_t iForbidBattleMessageTime;
    uint32_t stBattleCollectionSkinInfo_Ptr;
    uint32_t iButtonSkin;

    // Legacy/Computed fields
    std::string name;
    std::string uid;
    int camp;
    int heroId;
    int spellId;
    int rankLevel;

    // Legacy support
    std::string rank;
    std::string spell;
    std::string heroName;
};

// Real-time Battle Info for a Player (Dynamic)
struct PlayerBattleData {
    uint64_t uGuid;
    std::string playerName; // This should come from _sName equivalent
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
};

// Global Battle Stats (Expanded with all requested raw fields)
struct BattleGlobalStats {
    float gameTime; // In seconds

    // Primitive fields from ShowFightDataTiny
    uint32_t m_levelOnSixMin;
    uint32_t m_LevelOnTwelveMin;
    uintptr_t m_EmojiCarryList_Ptr;
    uintptr_t m_TDFighteData_Ptr;
    uintptr_t m_DeathInfoList_Ptr;
    uintptr_t m_DeathAttackInfoDict_Ptr;
    uintptr_t m_lNotLinkEffect_Ptr;
    uintptr_t m_dicKeyCancelDis_Ptr;
    uintptr_t m_KillerCount_Ptr;
    uintptr_t m_FighterDyData_Ptr;
    uint32_t m_KillNumCrossTower;
    uint32_t m_RevengeKillNum;
    uint32_t m_ExtremeBackHomeNum;
    uintptr_t m_selfBeAttackTIme_Ptr;
    uintptr_t m_heroNumAroundSelf_Ptr;
    uintptr_t m_EnemyhurtSelf_Ptr;
    uint32_t lastLockGuid;
    bool bLockGuidChanged;
    uint32_t m_BackHomeCount;
    uint32_t m_RecoverSuccessfullyCount;
    uintptr_t m_ReplaceHeroSkill_Ptr;
    uintptr_t m_arenaWinVoice_Ptr;
    uintptr_t m_arenaLoseVoice_Ptr;
    uint32_t m_BuyEquipCount;
    float m_BuyEquipTime;
    uintptr_t m_BannedList_Ptr;
    uintptr_t m_VoiceBannedList_Ptr;
    uintptr_t m_ForbidTalkList_Ptr;
    uintptr_t m_BuyEquipTimes_Ptr;
    uintptr_t m_GreatIDs_Ptr;
    uintptr_t m_FighterSplitEnergyBar_Ptr;
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

    // Legacy support fields (for compatibility)
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
