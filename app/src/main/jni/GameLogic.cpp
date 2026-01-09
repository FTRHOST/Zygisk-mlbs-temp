#include "GameLogic.h"
#include <jni.h>
#include <thread>
#include <chrono>
#include <android/log.h>
#include <vector>
#include <string>
#include <mutex>
#include <sstream>
#include <map>

#include "Include.h"
#include "Il2Cpp/il2cpp_dump.h"
#include "feature/GameClass.h"
#include "feature/ToString.h"
#include "feature/ToString2.h"
#include "feature/BattleData.h" // Include this for ShowFightDataTiny_Layout
#include "IpcServer.h"
#include "obfuscate.h"
#include "dobby.h" // For Hooking

// Define Global State
GlobalState g_State;

// Timer variables
std::chrono::steady_clock::time_point g_battleStartTime;
std::chrono::duration<float> g_elapsedBattleTime(0);
std::atomic<bool> g_isBattleTimerRunning(false);

// Cache pointers (Singletons) to avoid repeated lookups
void* g_LogicBattleManager_Instance = nullptr;
void* g_BattleData_Instance = nullptr;
void* g_UIRankHero_Instance = nullptr; // For BanPick

#define LOG_TAG "MLBS_CORE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// Helper to safely read a field
#define READ_FIELD(target, type, offset) \
    if(offset > 0) target = *(type*)((uintptr_t)pawn + offset);

#define READ_STRING(target, offset) \
    if(offset > 0) { \
        MonoString* str = *(MonoString**)((uintptr_t)pawn + offset); \
        if(str) target = str->CString(); \
    }

// Helper to read simple Pointer/Address
#define READ_PTR(target, offset) \
    if(offset > 0) target = (uintptr_t)(*(void**)((uintptr_t)pawn + offset));

// --- HOOKING UIRankHero ---
void (*old_UIRankHero_OnUpdate)(void* instance);
void new_UIRankHero_OnUpdate(void* instance) {
    if (instance != nullptr) {
        g_UIRankHero_Instance = instance;
    }
    if (old_UIRankHero_OnUpdate) {
        old_UIRankHero_OnUpdate(instance);
    }
}

// --- Implementation of GetBattleStats ---
BattleStats GetBattleStats() {
    BattleStats stats = {};
    void* showFightDataInstance = nullptr;

    Il2CppGetStaticFieldValue(OBFUSCATE("Assembly-CSharp.dll"), "", OBFUSCATE("ShowFightData"), OBFUSCATE("Instance"), &showFightDataInstance);

    if (showFightDataInstance) {
        auto* pData = static_cast<ShowFightDataTiny_Layout*>(showFightDataInstance);

        // Read all fields mapped in ShowFightDataTiny_Layout
        stats.m_levelOnSixMin = pData->m_levelOnSixMin;
        stats.m_LevelOnTwelveMin = pData->m_LevelOnTwelveMin;
        stats.m_KillNumCrossTower = pData->m_KillNumCrossTower;
        stats.m_RevengeKillNum = pData->m_RevengeKillNum;
        stats.m_ExtremeBackHomeNum = pData->m_ExtremeBackHomeNum;
        stats.bLockGuidChanged = pData->bLockGuidChanged;
        stats.m_BackHomeCount = pData->m_BackHomeCount;
        stats.m_RecoverSuccessfullyCount = pData->m_RecoverSuccessfullyCount;
        stats.m_BuyEquipCount = pData->m_BuyEquipCount;
        stats.m_BuyEquipTime = pData->m_BuyEquipTime;
        stats.m_uSurvivalCount = pData->m_uSurvivalCount;
        stats.m_uPlayerCount = pData->m_uPlayerCount;

        stats.m_iCampAKill = pData->m_iCampAKill;
        stats.m_iCampBKill = pData->m_iCampBKill;
        stats.m_CampAGold = pData->m_CampAGold;
        stats.m_CampBGold = pData->m_CampBGold;
        stats.m_CampAExp = pData->m_CampAExp;
        stats.m_CampBExp = pData->m_CampBExp;
        stats.m_CampAKillTower = pData->m_CampAKillTower;
        stats.m_CampBKillTower = pData->m_CampBKillTower;
        stats.m_CampAKillLingZhu = pData->m_CampAKillLingZhu;
        stats.m_CampBKillLingZhu = pData->m_CampBKillLingZhu;
        stats.m_CampAKillShenGui = pData->m_CampAKillShenGui;
        stats.m_CampBKillShenGui = pData->m_CampBKillShenGui;
        stats.m_CampAKillLingzhuOnSuperior = pData->m_CampAKillLingzhuOnSuperior;
        stats.m_CampBKillLingzhuOnSuperior = pData->m_CampBKillLingzhuOnSuperior;
        stats.m_CampASuperiorTime = pData->m_CampASuperiorTime;
        stats.m_CampBSuperiorTime = pData->m_CampBSuperiorTime;
        stats.m_iFirstBldTime = pData->m_iFirstBldTime;
        stats.m_iFirstBldKiller = pData->m_iFirstBldKiller;
    }

    return stats;
}

// --- LOGIC IMPLEMENTATION ---

void UpdateBanPickState() {
    if (!g_UIRankHero_Instance) {
        std::lock_guard<std::mutex> lock(g_State.stateMutex);
        g_State.banPickState.isOpen = false;
        return;
    }

    void* pawn = g_UIRankHero_Instance;

    BanPickState bpState = {};
    bpState.isOpen = true;

    if (UIRankHero_banOrder > 0) {
        void* banOrderList = *(void**)((uintptr_t)pawn + UIRankHero_banOrder);
        if (banOrderList) {
            auto* list = (monoList<uint32_t>*)banOrderList;
            int size = list->getSize();
            if (size > 20) size = 20;
            for (int i = 0; i < size; i++) {
                bpState.banOrder.push_back(list->getItems()[i]);
            }
        }
    }

    if (UIRankHero_pickOrder > 0) {
        void* pickOrderList = *(void**)((uintptr_t)pawn + UIRankHero_pickOrder);
        if (pickOrderList) {
            auto* list = (monoList<uint32_t>*)pickOrderList;
            int size = list->getSize();
            if (size > 20) size = 20;
            for (int i = 0; i < size; i++) {
                bpState.pickOrder.push_back(list->getItems()[i]);
            }
        }
    }

    {
        std::lock_guard<std::mutex> lock(g_State.stateMutex);
        g_State.banPickState = bpState;
    }
}

void UpdateBattleStats() {
    // 1. Get Game Time
    float time = 0.0f;
    if (BattleStaticInit_GetTime) {
         float (*GetTimeFunc)() = (float (*)())BattleStaticInit_GetTime;
         time = GetTimeFunc();
    }

    // 2. Get Team Stats (Using the expanded struct)
    BattleStats stats = GetBattleStats();

    // 3. Get Individual Player Stats from BattleData.heroInfoList
    std::vector<PlayerBattleData> localBattlePlayers;
    void* battleDataInstance = nullptr;
    Il2CppGetStaticFieldValue(OBFUSCATE("Assembly-CSharp.dll"), "", OBFUSCATE("BattleData"), OBFUSCATE("Instance"), &battleDataInstance);

    if (battleDataInstance && BattleData_heroInfoList > 0) {
        void* dictPtr = *(void**)((uintptr_t)battleDataInstance + BattleData_heroInfoList);
        if (dictPtr) {
            // Dictionary<uint32_t, FightHeroInfo>
            auto* dictionary = (Dictionary<uint32_t, void*>*)dictPtr;

            if (dictionary->entries && dictionary->count > 0) {
                auto entries = dictionary->entries->toCPPlist();
                for (auto& entry : entries) {
                    void* fightHeroInfo = entry.value;
                    if (!fightHeroInfo) continue;

                    PlayerBattleData pb = {};
                    void* pawn = fightHeroInfo; // Alias for READ_FIELD macro

                    READ_FIELD(pb.uGuid, uint32_t, FightHeroInfo_m_uGuid);
                    READ_STRING(pb.playerName, FightHeroInfo_m_PlayerName); // Uses original function name retrieval
                    READ_FIELD(pb.campType, int32_t, FightHeroInfo_m_CampType);
                    READ_FIELD(pb.kill, uint32_t, FightHeroInfo_m_KillNum);
                    READ_FIELD(pb.death, uint32_t, FightHeroInfo_m_DeadNum);
                    READ_FIELD(pb.assist, uint32_t, FightHeroInfo_m_AssistNum);
                    READ_FIELD(pb.gold, uint32_t, FightHeroInfo_m_Gold);
                    READ_FIELD(pb.totalGold, uint32_t, FightHeroInfo_m_TotalGold);

                    localBattlePlayers.push_back(pb);
                }
            }
        }
    }

    {
        std::lock_guard<std::mutex> lock(g_State.stateMutex);
        g_State.battleStats.gameTime = time;

        // Copy all raw fields to Global State (Ensure names match GlobalState struct members)
        g_State.battleStats.m_levelOnSixMin = stats.m_levelOnSixMin;
        g_State.battleStats.m_LevelOnTwelveMin = stats.m_LevelOnTwelveMin;
        g_State.battleStats.m_KillNumCrossTower = stats.m_KillNumCrossTower;
        g_State.battleStats.m_RevengeKillNum = stats.m_RevengeKillNum;
        g_State.battleStats.m_ExtremeBackHomeNum = stats.m_ExtremeBackHomeNum;
        g_State.battleStats.bLockGuidChanged = stats.bLockGuidChanged;
        g_State.battleStats.m_BackHomeCount = stats.m_BackHomeCount;
        g_State.battleStats.m_RecoverSuccessfullyCount = stats.m_RecoverSuccessfullyCount;
        g_State.battleStats.m_BuyEquipCount = stats.m_BuyEquipCount;
        g_State.battleStats.m_BuyEquipTime = stats.m_BuyEquipTime;
        g_State.battleStats.m_uSurvivalCount = stats.m_uSurvivalCount;
        g_State.battleStats.m_uPlayerCount = stats.m_uPlayerCount;

        g_State.battleStats.m_iCampAKill = stats.m_iCampAKill;
        g_State.battleStats.m_iCampBKill = stats.m_iCampBKill;
        g_State.battleStats.m_CampAGold = stats.m_CampAGold;
        g_State.battleStats.m_CampBGold = stats.m_CampBGold;
        g_State.battleStats.m_CampAExp = stats.m_CampAExp;
        g_State.battleStats.m_CampBExp = stats.m_CampBExp;
        g_State.battleStats.m_CampAKillTower = stats.m_CampAKillTower;
        g_State.battleStats.m_CampBKillTower = stats.m_CampBKillTower;
        g_State.battleStats.m_CampAKillLingZhu = stats.m_CampAKillLingZhu;
        g_State.battleStats.m_CampBKillLingZhu = stats.m_CampBKillLingZhu;
        g_State.battleStats.m_CampAKillShenGui = stats.m_CampAKillShenGui;
        g_State.battleStats.m_CampBKillShenGui = stats.m_CampBKillShenGui;
        g_State.battleStats.m_CampAKillLingzhuOnSuperior = stats.m_CampAKillLingzhuOnSuperior;
        g_State.battleStats.m_CampBKillLingzhuOnSuperior = stats.m_CampBKillLingzhuOnSuperior;
        g_State.battleStats.m_CampASuperiorTime = stats.m_CampASuperiorTime;
        g_State.battleStats.m_CampBSuperiorTime = stats.m_CampBSuperiorTime;
        g_State.battleStats.m_iFirstBldTime = stats.m_iFirstBldTime;
        g_State.battleStats.m_iFirstBldKiller = stats.m_iFirstBldKiller;

        // Also map legacy fields if relay server expects them (though we are moving to raw)
        g_State.battleStats.campAScore = stats.m_iCampAKill;
        g_State.battleStats.campBScore = stats.m_iCampBKill;
        g_State.battleStats.campAGold = stats.m_CampAGold;
        g_State.battleStats.campBGold = stats.m_CampBGold;
        g_State.battleStats.campAKillTower = stats.m_CampAKillTower;
        g_State.battleStats.campBKillTower = stats.m_CampBKillTower;
        g_State.battleStats.campAKillLord = stats.m_CampAKillLingZhu;
        g_State.battleStats.campBKillLord = stats.m_CampBKillLingZhu;
        g_State.battleStats.campAKillTurtle = stats.m_CampAKillShenGui;
        g_State.battleStats.campBKillTurtle = stats.m_CampBKillShenGui;

        g_State.battlePlayers = localBattlePlayers;
    }
}

void UpdatePlayerInfo() {
    auto battlePlayerList = ((monoList<void *> *(*)(uintptr_t))SystemData_GetBattlePlayerInfo)((uintptr_t)0);
    if (!battlePlayerList) {
        std::lock_guard<std::mutex> lock(g_State.stateMutex);
        if (!g_State.players.empty()) g_State.players.clear();
        return;
    }
    
    std::lock_guard<std::mutex> lock(g_State.stateMutex);
    g_State.players.clear();

    for (int i = 0; i < battlePlayerList->getSize(); i++) {
        void *pawn = battlePlayerList->getItems()[i];
        if (!pawn) continue;

        PlayerData p = {};
        
        // --- KEY IDENTIFIERS ---
        READ_FIELD(p.lUid, uint64_t, SystemData_RoomData_lUid);
        READ_FIELD(p.bUid, uint64_t, SystemData_RoomData_bUid);
        READ_FIELD(p.iCamp, uint32_t, SystemData_RoomData_iCamp);
        READ_FIELD(p.iPos, uint32_t, SystemData_RoomData_iPos);
        READ_STRING(p._sName, SystemData_RoomData__sName);
        READ_FIELD(p.heroid, uint32_t, SystemData_RoomData_heroid);
        READ_FIELD(p.heroskin, uint32_t, SystemData_RoomData_heroskin);
        READ_FIELD(p.summonSkillId, int32_t, SystemData_RoomData_summonSkillId);
        READ_FIELD(p.runeId, int32_t, SystemData_RoomData_runeId);
        READ_FIELD(p.runeLv, int32_t, SystemData_RoomData_runeLv);
        READ_FIELD(p.uiRankLevel, uint32_t, SystemData_RoomData_uiRankLevel);
        READ_FIELD(p.iMythPoint, uint32_t, SystemData_RoomData_iMythPoint);
        READ_FIELD(p.uiZoneId, uint32_t, SystemData_RoomData_uiZoneId);
        READ_FIELD(p.banHero, uint32_t, SystemData_RoomData_banHero);
        READ_FIELD(p.bRobot, bool, SystemData_RoomData_bRobot);
        READ_FIELD(p.bNewPlayer, bool, SystemData_RoomData_bNewPlayer);
        READ_FIELD(p.uiHeroIDChoose, uint32_t, SystemData_RoomData_uiHeroIDChoose);

        // --- MASSIVE RAW FIELD DUMP ---
        READ_FIELD(p.bAutoConditionNew, bool, SystemData_RoomData_bAutoConditionNew);
        READ_FIELD(p.bShowSeasonAchieve, bool, SystemData_RoomData_bShowSeasonAchieve);
        READ_FIELD(p.iStyleBoardId, uint32_t, SystemData_RoomData_iStyleBoardId);
        READ_FIELD(p.iMatchEffectId, uint32_t, SystemData_RoomData_iMatchEffectId);
        READ_FIELD(p.iDayBreakNo1Count, uint32_t, SystemData_RoomData_iDayBreakNo1Count);
        READ_FIELD(p.bAutoReadySelect, bool, SystemData_RoomData_bAutoReadySelect);
        READ_FIELD(p.headID, uint32_t, SystemData_RoomData_headID);
        READ_FIELD(p.uiSex, uint32_t, SystemData_RoomData_uiSex);
        READ_FIELD(p.country, uint32_t, SystemData_RoomData_country);
        READ_STRING(p.facePath, SystemData_RoomData_facePath);
        READ_FIELD(p.faceBorder, uint32_t, SystemData_RoomData_faceBorder);
        READ_FIELD(p.bStarVip, bool, SystemData_RoomData_bStarVip);
        READ_FIELD(p.bMCStarVip, bool, SystemData_RoomData_bMCStarVip);
        READ_FIELD(p.bMCStarVipPlus, bool, SystemData_RoomData_bMCStarVipPlus);
        READ_FIELD(p.ulRoomID, uint64_t, SystemData_RoomData_ulRoomID);
        READ_FIELD(p.iConBlackRoomId, uint64_t, SystemData_RoomData_iConBlackRoomId);
        READ_FIELD(p.uiBattlePlayerType, uint32_t, SystemData_RoomData_uiBattlePlayerType);
        READ_STRING(p.sThisLoginCountry, SystemData_RoomData_sThisLoginCountry);
        READ_STRING(p.sCreateRoleCountry, SystemData_RoomData_sCreateRoleCountry);
        READ_FIELD(p.uiLanguage, uint32_t, SystemData_RoomData_uiLanguage);
        READ_FIELD(p.bIsOpenLive, bool, SystemData_RoomData_bIsOpenLive);
        READ_FIELD(p.iTeamId, uint64_t, SystemData_RoomData_iTeamId);
        READ_FIELD(p.iTeamNationId, uint64_t, SystemData_RoomData_iTeamNationId);
        READ_STRING(p._steamName, SystemData_RoomData__steamName);
        READ_STRING(p._steamSimpleName, SystemData_RoomData__steamSimpleName);
        READ_FIELD(p.iCertify, uint32_t, SystemData_RoomData_iCertify);
        READ_FIELD(p.uiPVPRank, uint32_t, SystemData_RoomData_uiPVPRank);
        READ_FIELD(p.bRankReview, bool, SystemData_RoomData_bRankReview);
        READ_FIELD(p.iElo, uint32_t, SystemData_RoomData_iElo);
        READ_FIELD(p.uiRoleLevel, uint32_t, SystemData_RoomData_uiRoleLevel);
        READ_FIELD(p.iRoad, uint32_t, SystemData_RoomData_iRoad);
        READ_FIELD(p.uiSkinSource, uint32_t, SystemData_RoomData_uiSkinSource);
        READ_FIELD(p.iFighterType, uint32_t, SystemData_RoomData_iFighterType);
        READ_FIELD(p.iWorldCupSupportCountry, uint32_t, SystemData_RoomData_iWorldCupSupportCountry);
        READ_FIELD(p.iHeroLevel, uint32_t, SystemData_RoomData_iHeroLevel);
        READ_FIELD(p.iHeroSubLevel, uint32_t, SystemData_RoomData_iHeroSubLevel);
        READ_FIELD(p.iHeroPowerLevel, uint32_t, SystemData_RoomData_iHeroPowerLevel);
        READ_FIELD(p.iActCamp, uint32_t, SystemData_RoomData_iActCamp);
        READ_FIELD(p.mHeroMission, uint32_t, SystemData_RoomData_mHeroMission);
        READ_FIELD(p.mSkinPaint, uint32_t, SystemData_RoomData_mSkinPaint);
        READ_STRING(p.sClientVersion, SystemData_RoomData_sClientVersion);
        READ_FIELD(p.uiHolyStatue, uint32_t, SystemData_RoomData_uiHolyStatue);
        READ_FIELD(p.uiKamon, uint32_t, SystemData_RoomData_uiKamon);
        READ_FIELD(p.uiUserMapID, uint32_t, SystemData_RoomData_uiUserMapID);
        READ_FIELD(p.iSurviveRank, uint32_t, SystemData_RoomData_iSurviveRank);
        READ_FIELD(p.iDefenceRankID, uint32_t, SystemData_RoomData_iDefenceRankID);
        READ_FIELD(p.iLeagueWCNum, uint32_t, SystemData_RoomData_iLeagueWCNum);
        READ_FIELD(p.iLeagueFCNum, uint32_t, SystemData_RoomData_iLeagueFCNum);
        READ_FIELD(p.iMPLCertifyTime, uint32_t, SystemData_RoomData_iMPLCertifyTime);
        READ_FIELD(p.iMPLCertifyID, uint32_t, SystemData_RoomData_iMPLCertifyID);
        READ_FIELD(p.iHeroUseCount, uint32_t, SystemData_RoomData_iHeroUseCount);
        READ_FIELD(p.bMythEvaled, bool, SystemData_RoomData_bMythEvaled);
        READ_FIELD(p.iDefenceFlag, uint32_t, SystemData_RoomData_iDefenceFlag);
        READ_FIELD(p.iDefenPoint, uint32_t, SystemData_RoomData_iDefenPoint);
        READ_FIELD(p.iDefenceMap, uint32_t, SystemData_RoomData_iDefenceMap);
        READ_FIELD(p.iAIType, uint32_t, SystemData_RoomData_iAIType);
        READ_FIELD(p.iAISeed, uint32_t, SystemData_RoomData_iAISeed);
        READ_STRING(p.sAiName, SystemData_RoomData_sAiName);
        READ_FIELD(p.iWarmValue, uint32_t, SystemData_RoomData_iWarmValue);
        READ_FIELD(p.uiAircraftIDChooose, uint32_t, SystemData_RoomData_uiAircraftIDChooose);
        READ_FIELD(p.uiHeroIDChoose, uint32_t, SystemData_RoomData_uiHeroIDChoose);
        READ_FIELD(p.uiHeroSkinIDChoose, uint32_t, SystemData_RoomData_uiHeroSkinIDChoose);
        READ_FIELD(p.uiMapIDChoose, uint32_t, SystemData_RoomData_uiMapIDChoose);
        READ_FIELD(p.uiMapSkinIDChoose, uint32_t, SystemData_RoomData_uiMapSkinIDChoose);
        READ_FIELD(p.uiDefenceRankScore, uint32_t, SystemData_RoomData_uiDefenceRankScore);
        READ_FIELD(p.bBanChat, bool, SystemData_RoomData_bBanChat);
        READ_FIELD(p.iChatBanFinishTime, uint32_t, SystemData_RoomData_iChatBanFinishTime);
        READ_FIELD(p.iChatBanBattleNum, uint32_t, SystemData_RoomData_iChatBanBattleNum);
        
        // Pointers/Complex Types (Just grabbing address for 'raw' info as requested)
        READ_PTR(p.mapTalentTree_Ptr, SystemData_RoomData_mapTalentTree);
        READ_PTR(p.mRuneSkill2023_Ptr, SystemData_RoomData_mRuneSkill2023);
        READ_PTR(p.skinlist_Ptr, SystemData_RoomData_skinlist);
        READ_PTR(p.vCanSelectHero_Ptr, SystemData_RoomData_vCanSelectHero);
        READ_PTR(p.vCanPickHero_Ptr, SystemData_RoomData_vCanPickHero);
        READ_PTR(p.lsEffectSkins_Ptr, SystemData_RoomData_lsEffectSkins);
        READ_PTR(p.lsComEffSkins_Ptr, SystemData_RoomData_lsComEffSkins);
        READ_PTR(p.vMissions_Ptr, SystemData_RoomData_vMissions);
        READ_PTR(p.vTitle_Ptr, SystemData_RoomData_vTitle);
        READ_PTR(p.vEmoji_Ptr, SystemData_RoomData_vEmoji);
        READ_PTR(p.vItemBuff_Ptr, SystemData_RoomData_vItemBuff);
        READ_PTR(p.vMapPaint_Ptr, SystemData_RoomData_vMapPaint);
        READ_PTR(p.mapBattleAttr_Ptr, SystemData_RoomData_mapBattleAttr);
        READ_PTR(p.vFastChat_Ptr, SystemData_RoomData_vFastChat);
        READ_PTR(p.vWantSelectHero_Ptr, SystemData_RoomData_vWantSelectHero);

        // Legacy/Computed
        p.name = p._sName; // Ensure _sName is used
        p.uid = std::to_string(p.lUid) + "(" + std::to_string(p.uiZoneId) + ")";
        p.rank = RankToString(p.uiRankLevel, p.iMythPoint);
        p.spell = SpellToString(p.summonSkillId);
        p.heroName = HeroToString(p.heroid);
        p.camp = p.iCamp;
        p.heroId = p.heroid;
        p.spellId = p.summonSkillId;
        p.rankLevel = p.uiRankLevel;

        g_State.players.push_back(p);
    }
}

void MonitorBattleState() {
    void *logicBattleManager = nullptr;
    Il2CppGetStaticFieldValue(OBFUSCATE("Assembly-CSharp.dll"), "", OBFUSCATE("LogicBattleManager"), OBFUSCATE("Instance"), &logicBattleManager);

    bool isManagerValid = (logicBattleManager != nullptr);
    int currentBattleState = -1;

    if (isManagerValid) {
        currentBattleState = GetBattleState(logicBattleManager);

        {
            std::lock_guard<std::mutex> lock(g_State.stateMutex);
            if (currentBattleState != g_State.battleState) {
                g_State.battleState = currentBattleState;
            }
        }

        // Logic for specific states
        if (currentBattleState == 2) { // Draft/BanPick
             UpdateBanPickState();
             UpdatePlayerInfo(); // Player info is valid in Draft
        }
        else if (currentBattleState == 3) { // In-Game
             UpdateBattleStats(); // Update Score, Gold, Time
             UpdatePlayerInfo();  // Player info (static part)
        }
    }
    
    // --- BROADCAST HEARTBEAT & DEBUG (Setiap ~60 Frame / 1 Detik) ---
    static int frameTick = 0;
    frameTick++;

    if (frameTick % 60 == 0) {
        std::stringstream ss;
        ss << "{";
        ss << "\"type\":\"heartbeat\",";

        // Debug Section
        ss << "\"debug\":{";
        ss << "\"manager_found\":" << (isManagerValid ? "true" : "false") << ",";
        ss << "\"game_state\":" << currentBattleState << ",";
        ss << "\"feature_enabled\":" << (g_State.roomInfoEnabled ? "true" : "false");
        ss << "},";

        // Data Section
        ss << "\"data\":{";

        // --- 1. Room Info (/inforoom) ---
        {
             std::lock_guard<std::mutex> lock(g_State.stateMutex);
             ss << "\"room_info\":{";
             ss << "\"player_count\":" << g_State.players.size() << ",";
             ss << "\"players\":[";
             for (size_t i = 0; i < g_State.players.size(); ++i) {
                 const auto& p = g_State.players[i];
                 ss << "{";
                 // Key Data
                 ss << "\"lUid\":" << p.lUid << ",";
                 ss << "\"_sName\":\"" << p._sName << "\",";
                 ss << "\"iCamp\":" << p.iCamp << ",";
                 ss << "\"heroid\":" << p.heroid << ",";

                 // Expanded Raw Data
                 ss << "\"bAutoConditionNew\":" << p.bAutoConditionNew << ",";
                 ss << "\"bShowSeasonAchieve\":" << p.bShowSeasonAchieve << ",";
                 ss << "\"iStyleBoardId\":" << p.iStyleBoardId << ",";
                 ss << "\"iMatchEffectId\":" << p.iMatchEffectId << ",";
                 ss << "\"iDayBreakNo1Count\":" << p.iDayBreakNo1Count << ",";
                 ss << "\"bAutoReadySelect\":" << p.bAutoReadySelect << ",";
                 ss << "\"bRobot\":" << p.bRobot << ",";
                 ss << "\"headID\":" << p.headID << ",";
                 ss << "\"uiSex\":" << p.uiSex << ",";
                 ss << "\"country\":" << p.country << ",";
                 ss << "\"uiZoneId\":" << p.uiZoneId << ",";
                 ss << "\"summonSkillId\":" << p.summonSkillId << ",";
                 ss << "\"runeId\":" << p.runeId << ",";
                 ss << "\"runeLv\":" << p.runeLv << ",";
                 ss << "\"facePath\":\"" << p.facePath << "\",";
                 ss << "\"faceBorder\":" << p.faceBorder << ",";
                 ss << "\"bStarVip\":" << p.bStarVip << ",";
                 ss << "\"bMCStarVip\":" << p.bMCStarVip << ",";
                 ss << "\"bMCStarVipPlus\":" << p.bMCStarVipPlus << ",";
                 ss << "\"ulRoomID\":" << p.ulRoomID << ",";
                 ss << "\"iConBlackRoomId\":" << p.iConBlackRoomId << ",";
                 ss << "\"banHero\":" << p.banHero << ",";
                 ss << "\"uiBattlePlayerType\":" << p.uiBattlePlayerType << ",";
                 ss << "\"sThisLoginCountry\":\"" << p.sThisLoginCountry << "\",";
                 ss << "\"sCreateRoleCountry\":\"" << p.sCreateRoleCountry << "\",";
                 ss << "\"uiLanguage\":" << p.uiLanguage << ",";
                 ss << "\"bIsOpenLive\":" << p.bIsOpenLive << ",";
                 ss << "\"iTeamId\":" << p.iTeamId << ",";
                 ss << "\"iTeamNationId\":" << p.iTeamNationId << ",";
                 ss << "\"_steamName\":\"" << p._steamName << "\",";
                 ss << "\"_steamSimpleName\":\"" << p._steamSimpleName << "\",";
                 ss << "\"iCertify\":" << p.iCertify << ",";
                 ss << "\"uiRankLevel\":" << p.uiRankLevel << ",";
                 ss << "\"uiPVPRank\":" << p.uiPVPRank << ",";
                 ss << "\"bRankReview\":" << p.bRankReview << ",";
                 ss << "\"iElo\":" << p.iElo << ",";
                 ss << "\"uiRoleLevel\":" << p.uiRoleLevel << ",";
                 ss << "\"bNewPlayer\":" << p.bNewPlayer << ",";
                 ss << "\"iRoad\":" << p.iRoad << ",";
                 ss << "\"uiSkinSource\":" << p.uiSkinSource << ",";
                 ss << "\"iFighterType\":" << p.iFighterType << ",";
                 ss << "\"iWorldCupSupportCountry\":" << p.iWorldCupSupportCountry << ",";
                 ss << "\"iHeroLevel\":" << p.iHeroLevel << ",";
                 ss << "\"iHeroSubLevel\":" << p.iHeroSubLevel << ",";
                 ss << "\"iHeroPowerLevel\":" << p.iHeroPowerLevel << ",";
                 ss << "\"iActCamp\":" << p.iActCamp << ",";
                 ss << "\"mHeroMission\":" << p.mHeroMission << ",";
                 ss << "\"mSkinPaint\":" << p.mSkinPaint << ",";
                 ss << "\"sClientVersion\":\"" << p.sClientVersion << "\",";
                 ss << "\"uiHolyStatue\":" << p.uiHolyStatue << ",";
                 ss << "\"uiKamon\":" << p.uiKamon << ",";
                 ss << "\"uiUserMapID\":" << p.uiUserMapID << ",";
                 ss << "\"iSurviveRank\":" << p.iSurviveRank << ",";
                 ss << "\"iDefenceRankID\":" << p.iDefenceRankID << ",";
                 ss << "\"iLeagueWCNum\":" << p.iLeagueWCNum << ",";
                 ss << "\"iLeagueFCNum\":" << p.iLeagueFCNum << ",";
                 ss << "\"iMPLCertifyTime\":" << p.iMPLCertifyTime << ",";
                 ss << "\"iMPLCertifyID\":" << p.iMPLCertifyID << ",";
                 ss << "\"iHeroUseCount\":" << p.iHeroUseCount << ",";
                 ss << "\"iMythPoint\":" << p.iMythPoint << ",";
                 ss << "\"bMythEvaled\":" << p.bMythEvaled << ",";
                 ss << "\"iDefenceFlag\":" << p.iDefenceFlag << ",";
                 ss << "\"iDefenPoint\":" << p.iDefenPoint << ",";
                 ss << "\"iDefenceMap\":" << p.iDefenceMap << ",";
                 ss << "\"iAIType\":" << p.iAIType << ",";
                 ss << "\"iAISeed\":" << p.iAISeed << ",";
                 ss << "\"sAiName\":\"" << p.sAiName << "\",";
                 ss << "\"iWarmValue\":" << p.iWarmValue << ",";
                 ss << "\"uiAircraftIDChooose\":" << p.uiAircraftIDChooose << ",";
                 ss << "\"uiHeroIDChoose\":" << p.uiHeroIDChoose << ",";
                 ss << "\"uiHeroSkinIDChoose\":" << p.uiHeroSkinIDChoose << ",";
                 ss << "\"uiMapIDChoose\":" << p.uiMapIDChoose << ",";
                 ss << "\"uiMapSkinIDChoose\":" << p.uiMapSkinIDChoose << ",";
                 ss << "\"uiDefenceRankScore\":" << p.uiDefenceRankScore << ",";
                 ss << "\"bBanChat\":" << p.bBanChat << ",";
                 ss << "\"iChatBanFinishTime\":" << p.iChatBanFinishTime << ",";
                 ss << "\"iChatBanBattleNum\":" << p.iChatBanBattleNum;

                 ss << "}";
                 if (i < g_State.players.size() - 1) ss << ",";
             }
             ss << "]";
             ss << "},";
        }

        // --- 2. Battle Stats (/infobattle & /timebattle) ---
        {
             std::lock_guard<std::mutex> lock(g_State.stateMutex);
             ss << "\"battle_stats\":{";
             ss << "\"time\":" << g_State.battleStats.gameTime << ",";

             // RAW FIELDS FROM ShowFightDataTiny (as requested)
             ss << "\"m_levelOnSixMin\":" << g_State.battleStats.m_levelOnSixMin << ",";
             ss << "\"m_LevelOnTwelveMin\":" << g_State.battleStats.m_LevelOnTwelveMin << ",";
             ss << "\"m_KillNumCrossTower\":" << g_State.battleStats.m_KillNumCrossTower << ",";
             ss << "\"m_RevengeKillNum\":" << g_State.battleStats.m_RevengeKillNum << ",";
             ss << "\"m_ExtremeBackHomeNum\":" << g_State.battleStats.m_ExtremeBackHomeNum << ",";
             ss << "\"bLockGuidChanged\":" << (g_State.battleStats.bLockGuidChanged ? "true" : "false") << ",";
             ss << "\"m_BackHomeCount\":" << g_State.battleStats.m_BackHomeCount << ",";
             ss << "\"m_RecoverSuccessfullyCount\":" << g_State.battleStats.m_RecoverSuccessfullyCount << ",";
             ss << "\"m_BuyEquipCount\":" << g_State.battleStats.m_BuyEquipCount << ",";
             ss << "\"m_BuyEquipTime\":" << g_State.battleStats.m_BuyEquipTime << ",";
             ss << "\"m_uSurvivalCount\":" << g_State.battleStats.m_uSurvivalCount << ",";
             ss << "\"m_uPlayerCount\":" << g_State.battleStats.m_uPlayerCount << ",";
             ss << "\"m_iCampAKill\":" << g_State.battleStats.m_iCampAKill << ",";
             ss << "\"m_iCampBKill\":" << g_State.battleStats.m_iCampBKill << ",";
             ss << "\"m_CampAGold\":" << g_State.battleStats.m_CampAGold << ",";
             ss << "\"m_CampBGold\":" << g_State.battleStats.m_CampBGold << ",";
             ss << "\"m_CampAExp\":" << g_State.battleStats.m_CampAExp << ",";
             ss << "\"m_CampBExp\":" << g_State.battleStats.m_CampBExp << ",";
             ss << "\"m_CampAKillTower\":" << g_State.battleStats.m_CampAKillTower << ",";
             ss << "\"m_CampBKillTower\":" << g_State.battleStats.m_CampBKillTower << ",";
             ss << "\"m_CampAKillLingZhu\":" << g_State.battleStats.m_CampAKillLingZhu << ",";
             ss << "\"m_CampBKillLingZhu\":" << g_State.battleStats.m_CampBKillLingZhu << ",";
             ss << "\"m_CampAKillShenGui\":" << g_State.battleStats.m_CampAKillShenGui << ",";
             ss << "\"m_CampBKillShenGui\":" << g_State.battleStats.m_CampBKillShenGui << ",";
             ss << "\"m_CampAKillLingzhuOnSuperior\":" << g_State.battleStats.m_CampAKillLingzhuOnSuperior << ",";
             ss << "\"m_CampBKillLingzhuOnSuperior\":" << g_State.battleStats.m_CampBKillLingzhuOnSuperior << ",";
             ss << "\"m_CampASuperiorTime\":" << g_State.battleStats.m_CampASuperiorTime << ",";
             ss << "\"m_CampBSuperiorTime\":" << g_State.battleStats.m_CampBSuperiorTime << ",";
             ss << "\"m_iFirstBldTime\":" << g_State.battleStats.m_iFirstBldTime << ",";
             ss << "\"m_iFirstBldKiller\":" << g_State.battleStats.m_iFirstBldKiller << ",";

             // Individual Player Stats
             ss << "\"players\":[";
             for (size_t i = 0; i < g_State.battlePlayers.size(); ++i) {
                 const auto& bp = g_State.battlePlayers[i];
                 ss << "{";
                 ss << "\"uGuid\":" << bp.uGuid << ",";
                 ss << "\"playerName\":\"" << bp.playerName << "\","; // Uses original name
                 ss << "\"camp\":" << bp.campType << ",";
                 ss << "\"kill\":" << bp.kill << ",";
                 ss << "\"death\":" << bp.death << ",";
                 ss << "\"assist\":" << bp.assist << ",";
                 ss << "\"gold\":" << bp.gold << ",";
                 ss << "\"totalGold\":" << bp.totalGold;
                 ss << "}";
                 if (i < g_State.battlePlayers.size() - 1) ss << ",";
             }
             ss << "]";
             ss << "},";
        }

        // --- 3. Ban Pick (/banpick) ---
        {
             std::lock_guard<std::mutex> lock(g_State.stateMutex);
             ss << "\"ban_pick\":{";
             ss << "\"is_open\":" << (g_State.banPickState.isOpen ? "true" : "false") << ",";
             ss << "\"ban_order\":[";
             for(size_t i=0; i<g_State.banPickState.banOrder.size(); i++) {
                 ss << g_State.banPickState.banOrder[i];
                 if(i < g_State.banPickState.banOrder.size() - 1) ss << ",";
             }
             ss << "],";
             ss << "\"pick_order\":[";
             for(size_t i=0; i<g_State.banPickState.pickOrder.size(); i++) {
                 ss << g_State.banPickState.pickOrder[i];
                 if(i < g_State.banPickState.pickOrder.size() - 1) ss << ",";
             }
             ss << "]";
             ss << "}";
        }

        ss << "}"; // End data
        ss << "}"; // End JSON

        // Send to Relay Server
        BroadcastData(ss.str());
    }
}

void InitGameLogic() {
    // Resolve UIRankHero.OnUpdate offset dynamically
    // 0xffffffff9bbe766c was in dump, but offsets change. Use dynamic resolution.
    void* pUIRankHero_OnUpdate = Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("UIRankHero"), OBFUSCATE("OnUpdate"), 0);

    if (pUIRankHero_OnUpdate) {
        DobbyHook(pUIRankHero_OnUpdate, (void*)new_UIRankHero_OnUpdate, (void**)&old_UIRankHero_OnUpdate);
        LOGI("Hooked UIRankHero.OnUpdate at %p", pUIRankHero_OnUpdate);
    } else {
        LOGI("Failed to find UIRankHero.OnUpdate");
    }
}
