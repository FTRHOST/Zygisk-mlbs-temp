#include "GameLogic.h"
#include <jni.h>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <map>
#include <dlfcn.h>
#include <android/log.h>
#include "GlobalState.h"
#include "feature/GameClass.h"
#include "IpcServer.h"
#include "dobby.h"
#include "Il2Cpp.h"

// Logging
#define TAG "MLBS_LOGIC"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

// Macros for safe field reading
#define READ_FIELD(ptr, offset, type) \
    ((ptr != nullptr) ? *(type*)((uintptr_t)ptr + offset) : type{})

#define READ_PTR(ptr, offset) \
    ((ptr != nullptr) ? *(void**)((uintptr_t)ptr + offset) : nullptr)

// Global State Instance
GlobalState g_State;

// Global Pointers
void* g_SystemData_RoomData_List = nullptr; // List<SystemData.RoomData>
void* g_BattleData_Instance = nullptr;
void* g_ShowFightDataTiny_Instance = nullptr;

// Helper to convert IL2CPP String to std::string
std::string Il2CppStringToString(String* str) {
    if (!str) return "";
    return str->toString();
}

// Helper to get static instance safely
void* GetStaticInstance(const char* dll, const char* namespaze, const char* klassName, const char* fieldName) {
    void* instance = nullptr;
    Il2CppGetStaticFieldValue(dll, namespaze, klassName, fieldName, &instance);
    return instance;
}

// Helper to iterate List<T>
// Menggunakan struct List template dari Il2Cpp.h
// Tidak menggunakan offset 0x10/0x18 manual, melainkan akses field struct.
// Ini lebih aman jika versi Unity menggunakan layout standar yang didefinisikan di Il2Cpp.h.
template <typename T = void*>
std::vector<T> IterateList(void* listPtr) {
    std::vector<T> result;
    if (!listPtr) return result;

    // Casting ke struct List<T> yang didefinisikan di Il2Cpp.h
    // Struct ini merefleksikan layout memori internal List C#
    List<T>* list = (List<T>*)listPtr;

    // Validasi pointer items (Array)
    if (!list || !list->items) return result;

    // Mengambil size dari field struct, bukan offset manual
    int size = list->getSize();

    // Safety cap untuk mencegah loop infinit jika memori korup
    if (size > 100) size = 100;

    for (int i = 0; i < size; i++) {
        // Akses array vector melalui struct Array
        // Array->vector adalah array C murni dari T
        // Il2Cpp.h mendefinisikan Array struct dengan header + vector[]
        if (i < list->items->max_length) {
            T item = list->items->vector[i];
            if (item) result.push_back(item);
        }
    }
    return result;
}

// Helper for Dictionary<uint, T> iteration
// Sama seperti List, menggunakan struct Dictionari dari Il2Cpp.h
template <typename TValue>
std::vector<TValue> IterateDictionaryValues(void* dictPtr) {
    std::vector<TValue> result;
    if (!dictPtr) return result;

    Dictionari<uint32_t, TValue>* dict = (Dictionari<uint32_t, TValue>*)dictPtr;
    if (!dict || !dict->values) return result;

    // Menggunakan nilai count dari struct
    int count = dict->getSize();
    if (count > 100) count = 100;

    for (int i = 0; i < count; i++) {
        if (i < dict->values->max_length) {
            TValue val = dict->values->vector[i];
            if (val) result.push_back(val);
        }
    }
    return result;
}


// Feature: InfoRoom
// Mengambil data dari SystemData.m_quickMatchRoomPayerList
// Menggunakan Il2CppGetStaticFieldValue untuk mendapatkan root pointer
void UpdatePlayerInfo() {
    // Refresh pointer setiap update untuk menangani perubahan state game/re-login
    // Menggunakan nama field string "m_quickMatchRoomPayerList" yang lebih stabil daripada offset
    Il2CppGetStaticFieldValue("Assembly-CSharp.dll", "", "SystemData", "m_quickMatchRoomPayerList", &g_SystemData_RoomData_List);

    if (!g_SystemData_RoomData_List) return;

    std::lock_guard<std::mutex> lock(g_State.stateMutex);
    auto& room = g_State.roomState;
    room.players.clear();

    // Menggunakan helper IterateList yang aman
    std::vector<void*> roomPlayers = IterateList<void*>(g_SystemData_RoomData_List);

    for (void* playerObj : roomPlayers) {
        if (!playerObj) continue;

        PlayerData p = {}; // Zero initialization

        // Membaca field menggunakan offset dari Dump (GameClass.h)
        // Offset ini spesifik untuk versi game saat dump diambil.
        // Penggunaan macro READ_FIELD memastikan pointer valid sebelum dereference.

        // --- Basic Fields ---
        p.ulUid = READ_FIELD(playerObj, SystemData_RoomData_lUid, uint64_t);
        p.iPos = READ_FIELD(playerObj, SystemData_RoomData_iPos, int);
        p.uiCampType = READ_FIELD(playerObj, SystemData_RoomData_iCamp, uint32_t);

        // String dibaca menggunakan helper Il2CppStringToString
        // Helper ini memanggil str->toString() yang menangani konversi UTF-16 ke std::string
        String* namePtr = READ_PTR(playerObj, SystemData_RoomData_sName);
        p.sName = Il2CppStringToString(namePtr);

        p.heroid = READ_FIELD(playerObj, SystemData_RoomData_heroid, uint32_t);
        p.uiHeroIDChoose = p.heroid;
        p.uiRankLevel = READ_FIELD(playerObj, SystemData_RoomData_uiRankLevel, uint32_t);

        // --- Detailed Fields ---
        p.bAutoConditionNew = READ_FIELD(playerObj, SystemData_RoomData_bAutoConditionNew, bool);
        p.bShowSeasonAchieve = READ_FIELD(playerObj, SystemData_RoomData_bShowSeasonAchieve, bool);
        p.iStyleBoardId = READ_FIELD(playerObj, SystemData_RoomData_iStyleBoardId, uint32_t);
        p.iMatchEffectId = READ_FIELD(playerObj, SystemData_RoomData_iMatchEffectId, uint32_t);
        p.iDayBreakNo1Count = READ_FIELD(playerObj, SystemData_RoomData_iDayBreakNo1Count, uint32_t);
        p.bUid = READ_FIELD(playerObj, SystemData_RoomData_bUid, uint64_t);
        p.bAutoReadySelect = READ_FIELD(playerObj, SystemData_RoomData_bAutoReadySelect, bool);
        p.bRobot = READ_FIELD(playerObj, SystemData_RoomData_bRobot, bool);
        p.heroskin = READ_FIELD(playerObj, SystemData_RoomData_heroskin, uint32_t);
        p.headID = READ_FIELD(playerObj, SystemData_RoomData_headID, uint32_t);
        p.uiSex = READ_FIELD(playerObj, SystemData_RoomData_uiSex, uint32_t);
        p.country = READ_FIELD(playerObj, SystemData_RoomData_country, uint32_t);
        p.uiZoneId = READ_FIELD(playerObj, SystemData_RoomData_uiZoneId, uint32_t);
        p.summonSkillId = READ_FIELD(playerObj, SystemData_RoomData_summonSkillId, int32_t);
        p.runeId = READ_FIELD(playerObj, SystemData_RoomData_runeId, int32_t);
        p.runeLv = READ_FIELD(playerObj, SystemData_RoomData_runeLv, int32_t);

        String* facePtr = READ_PTR(playerObj, SystemData_RoomData_facePath);
        p.facePath = Il2CppStringToString(facePtr);

        p.faceBorder = READ_FIELD(playerObj, SystemData_RoomData_faceBorder, uint32_t);
        p.bStarVip = READ_FIELD(playerObj, SystemData_RoomData_bStarVip, bool);
        p.bMCStarVip = READ_FIELD(playerObj, SystemData_RoomData_bMCStarVip, bool);
        p.bMCStarVipPlus = READ_FIELD(playerObj, SystemData_RoomData_bMCStarVipPlus, bool);
        p.ulRoomID = READ_FIELD(playerObj, SystemData_RoomData_ulRoomID, uint64_t);
        p.iConBlackRoomId = READ_FIELD(playerObj, SystemData_RoomData_iConBlackRoomId, uint64_t);
        p.banHero = READ_FIELD(playerObj, SystemData_RoomData_banHero, uint32_t);
        p.uiBattlePlayerType = READ_FIELD(playerObj, SystemData_RoomData_uiBattlePlayerType, int);

        String* sThisLoginCountryPtr = READ_PTR(playerObj, SystemData_RoomData_sThisLoginCountry);
        p.sThisLoginCountry = Il2CppStringToString(sThisLoginCountryPtr);

        String* sCreateRoleCountryPtr = READ_PTR(playerObj, SystemData_RoomData_sCreateRoleCountry);
        p.sCreateRoleCountry = Il2CppStringToString(sCreateRoleCountryPtr);

        p.uiLanguage = READ_FIELD(playerObj, SystemData_RoomData_uiLanguage, uint32_t);
        p.bIsOpenLive = READ_FIELD(playerObj, SystemData_RoomData_bIsOpenLive, bool);
        p.iTeamId = READ_FIELD(playerObj, SystemData_RoomData_iTeamId, uint64_t);
        p.iTeamNationId = READ_FIELD(playerObj, SystemData_RoomData_iTeamNationId, uint64_t);

        String* steamNamePtr = READ_PTR(playerObj, SystemData_RoomData_steamName);
        p.steamName = Il2CppStringToString(steamNamePtr);

        String* steamSimpleNamePtr = READ_PTR(playerObj, SystemData_RoomData_steamSimpleName);
        p.steamSimpleName = Il2CppStringToString(steamSimpleNamePtr);

        p.iCertify = READ_FIELD(playerObj, SystemData_RoomData_iCertify, uint32_t);
        p.uiPVPRank = READ_FIELD(playerObj, SystemData_RoomData_uiPVPRank, uint32_t);
        p.bRankReview = READ_FIELD(playerObj, SystemData_RoomData_bRankReview, bool);
        p.iElo = READ_FIELD(playerObj, SystemData_RoomData_iElo, uint32_t);
        p.uiRoleLevel = READ_FIELD(playerObj, SystemData_RoomData_uiRoleLevel, uint32_t);
        p.bNewPlayer = READ_FIELD(playerObj, SystemData_RoomData_bNewPlayer, bool);
        p.iRoad = READ_FIELD(playerObj, SystemData_RoomData_iRoad, uint32_t);
        p.uiSkinSource = READ_FIELD(playerObj, SystemData_RoomData_uiSkinSource, uint32_t);
        p.iFighterType = READ_FIELD(playerObj, SystemData_RoomData_iFighterType, uint32_t);
        p.iWorldCupSupportCountry = READ_FIELD(playerObj, SystemData_RoomData_iWorldCupSupportCountry, uint32_t);
        p.iHeroLevel = READ_FIELD(playerObj, SystemData_RoomData_iHeroLevel, uint32_t);
        p.iHeroSubLevel = READ_FIELD(playerObj, SystemData_RoomData_iHeroSubLevel, uint32_t);
        p.iHeroPowerLevel = READ_FIELD(playerObj, SystemData_RoomData_iHeroPowerLevel, uint32_t);
        p.iActCamp = READ_FIELD(playerObj, SystemData_RoomData_iActCamp, uint32_t);

        String* sClientVersionPtr = READ_PTR(playerObj, SystemData_RoomData_sClientVersion);
        p.sClientVersion = Il2CppStringToString(sClientVersionPtr);

        p.uiHolyStatue = READ_FIELD(playerObj, SystemData_RoomData_uiHolyStatue, uint32_t);
        p.uiKamon = READ_FIELD(playerObj, SystemData_RoomData_uiKamon, uint32_t);
        p.uiUserMapID = READ_FIELD(playerObj, SystemData_RoomData_uiUserMapID, uint32_t);
        p.iSurviveRank = READ_FIELD(playerObj, SystemData_RoomData_iSurviveRank, uint32_t);
        p.iDefenceRankID = READ_FIELD(playerObj, SystemData_RoomData_iDefenceRankID, uint32_t);
        p.iLeagueWCNum = READ_FIELD(playerObj, SystemData_RoomData_iLeagueWCNum, uint32_t);
        p.iLeagueFCNum = READ_FIELD(playerObj, SystemData_RoomData_iLeagueFCNum, uint32_t);
        p.iMPLCertifyTime = READ_FIELD(playerObj, SystemData_RoomData_iMPLCertifyTime, uint32_t);
        p.iMPLCertifyID = READ_FIELD(playerObj, SystemData_RoomData_iMPLCertifyID, uint32_t);
        p.iHeroUseCount = READ_FIELD(playerObj, SystemData_RoomData_iHeroUseCount, uint32_t);
        p.iMythPoint = READ_FIELD(playerObj, SystemData_RoomData_iMythPoint, uint32_t);
        p.bMythEvaled = READ_FIELD(playerObj, SystemData_RoomData_bMythEvaled, bool);
        p.iDefenceFlag = READ_FIELD(playerObj, SystemData_RoomData_iDefenceFlag, uint32_t);
        p.iDefenPoint = READ_FIELD(playerObj, SystemData_RoomData_iDefenPoint, uint32_t);
        p.iDefenceMap = READ_FIELD(playerObj, SystemData_RoomData_iDefenceMap, uint32_t);
        p.iAIType = READ_FIELD(playerObj, SystemData_RoomData_iAIType, uint32_t);
        p.iAISeed = READ_FIELD(playerObj, SystemData_RoomData_iAISeed, uint32_t);

        String* sAiNamePtr = READ_PTR(playerObj, SystemData_RoomData_sAiName);
        p.sAiName = Il2CppStringToString(sAiNamePtr);

        p.iWarmValue = READ_FIELD(playerObj, SystemData_RoomData_iWarmValue, uint32_t);
        p.uiAircraftIDChooose = READ_FIELD(playerObj, SystemData_RoomData_uiAircraftIDChooose, uint32_t);
        p.uiHeroSkinIDChoose = READ_FIELD(playerObj, SystemData_RoomData_uiHeroSkinIDChoose, uint32_t);
        p.uiMapIDChoose = READ_FIELD(playerObj, SystemData_RoomData_uiMapIDChoose, uint32_t);
        p.uiMapSkinIDChoose = READ_FIELD(playerObj, SystemData_RoomData_uiMapSkinIDChoose, uint32_t);
        p.uiDefenceRankScore = READ_FIELD(playerObj, SystemData_RoomData_uiDefenceRankScore, uint32_t);
        p.bBanChat = READ_FIELD(playerObj, SystemData_RoomData_bBanChat, bool);
        p.iChatBanFinishTime = READ_FIELD(playerObj, SystemData_RoomData_iChatBanFinishTime, uint32_t);
        p.iChatBanBattleNum = READ_FIELD(playerObj, SystemData_RoomData_iChatBanBattleNum, uint32_t);

        room.players.push_back(p);
    }
}

// Feature: BanPick
// Menggunakan SystemData.m_quickMatchRoomPayerList sebagai sumber data
// Menghindari Hook UIRankHero yang tidak stabil.
void UpdateBanPickState() {
    if (!g_SystemData_RoomData_List) return;

    List<void*>* list = (List<void*>*)g_SystemData_RoomData_List;
    if (!list || !list->items) return;

    std::lock_guard<std::mutex> lock(g_State.stateMutex);
    auto& bp = g_State.banPickState;
    bp.banList.clear();
    bp.pickList.clear();

    int size = list->getSize();
    if (size > 50) size = 50;

    for (int i = 0; i < size; i++) {
        void* playerObj = list->items->vector[i];
        if (!playerObj) continue;

        // banHero field di RoomData (0xb0)
        uint32_t ban = READ_FIELD(playerObj, SystemData_RoomData_banHero, uint32_t);
        if (ban > 0) bp.banList.push_back(ban);

        // Pick = heroid (0x4c)
        uint32_t pick = READ_FIELD(playerObj, SystemData_RoomData_heroid, uint32_t);
        if (pick > 0) bp.pickList.push_back(pick);
    }
}

// Feature: InfoBattle
// Mengakses Singleton BattleData dan ShowFightDataTiny via Il2Cpp API
void UpdateBattleStats() {
    // Refresh singleton pointers
    Il2CppGetStaticFieldValue("Assembly-CSharp.dll", "", "BattleData", "Instance", &g_BattleData_Instance);
    Il2CppGetStaticFieldValue("Assembly-CSharp.dll", "", "ShowFightDataTiny", "Instance", &g_ShowFightDataTiny_Instance);

    std::lock_guard<std::mutex> lock(g_State.stateMutex);
    auto& stats = g_State.battleStats;

    // 0. Game Time (from BattleData)
    if (g_BattleData_Instance) {
        stats.m_uiGameTime = READ_FIELD(g_BattleData_Instance, BattleData_fGameTime, uint32_t);

        // Individual Stats from BattleData.heroInfoList (Dictionary)
        void* heroDictPtr = READ_PTR(g_BattleData_Instance, BattleData_heroInfoList);
        if (heroDictPtr) {
            std::vector<void*> heroes = IterateDictionaryValues<void*>(heroDictPtr);
            stats.playerStats.clear();

            for (void* hero : heroes) {
                if (!hero) continue;
                PlayerBattleData p = {};
                p.m_uGuid = READ_FIELD(hero, FightHeroInfo_m_uGuid, uint32_t);
                p.m_KillNum = READ_FIELD(hero, FightHeroInfo_m_KillNum, uint32_t);
                p.m_DeadNum = READ_FIELD(hero, FightHeroInfo_m_DeadNum, uint32_t);
                p.m_AssistNum = READ_FIELD(hero, FightHeroInfo_m_AssistNum, uint32_t);
                p.m_Gold = READ_FIELD(hero, FightHeroInfo_m_Gold, uint32_t);
                p.m_TotalGold = READ_FIELD(hero, FightHeroInfo_m_TotalGold, uint32_t);
                p.m_Level = READ_FIELD(hero, FightHeroInfo_m_Level, uint32_t);
                p.m_CampType = READ_FIELD(hero, FightHeroInfo_m_CampType, uint32_t);
                p.m_HurtHeroValue = READ_FIELD(hero, FightHeroInfo_m_HurtHeroValue, uint32_t);
                p.m_HurtTowerValue = READ_FIELD(hero, FightHeroInfo_m_HurtTowerValue, uint32_t);
                p.m_InjuredValue = READ_FIELD(hero, FightHeroInfo_m_InjuredValue, uint32_t);
                p.m_DestroyTowerNum = READ_FIELD(hero, FightHeroInfo_iDestroyTowerNum, uint32_t);
                p.m_MonsterCoin = READ_FIELD(hero, FightHeroInfo_iMonsterCoin, uint32_t);
                p.iKill3 = READ_FIELD(hero, FightHeroInfo_iKill3, uint32_t);
                p.iKill4 = READ_FIELD(hero, FightHeroInfo_iKill4, uint32_t);
                p.iKill5 = READ_FIELD(hero, FightHeroInfo_iKill5, uint32_t);

                stats.playerStats.push_back(p);
            }
        }
    }

    // 1. Team Stats from ShowFightDataTiny
    if (g_ShowFightDataTiny_Instance) {
        void* inst = g_ShowFightDataTiny_Instance;
        // Using verified offsets
        stats.m_CampAKill = READ_FIELD(inst, ShowFightDataTiny_m_iCampAKill, int32_t); // dump says int32
        stats.m_CampBKill = READ_FIELD(inst, ShowFightDataTiny_m_iCampBKill, int32_t);
        stats.m_CampAGold = READ_FIELD(inst, ShowFightDataTiny_m_CampAGold, uint32_t);
        stats.m_CampBGold = READ_FIELD(inst, ShowFightDataTiny_m_CampBGold, uint32_t);
        stats.m_CampAExp = READ_FIELD(inst, ShowFightDataTiny_m_CampAExp, uint32_t);
        stats.m_CampBExp = READ_FIELD(inst, ShowFightDataTiny_m_CampBExp, uint32_t);
        stats.m_CampAKillTower = READ_FIELD(inst, ShowFightDataTiny_m_CampAKillTower, uint32_t);
        stats.m_CampBKillTower = READ_FIELD(inst, ShowFightDataTiny_m_CampBKillTower, uint32_t);
        stats.m_CampAKillLingZhu = READ_FIELD(inst, ShowFightDataTiny_m_CampAKillLingZhu, uint32_t);
        stats.m_CampBKillLingZhu = READ_FIELD(inst, ShowFightDataTiny_m_CampBKillLingZhu, uint32_t);
        stats.m_CampAKillShenGui = READ_FIELD(inst, ShowFightDataTiny_m_CampAKillShenGui, uint32_t);
        stats.m_CampBKillShenGui = READ_FIELD(inst, ShowFightDataTiny_m_CampBKillShenGui, uint32_t);
    }
}

std::string SerializeState() {
    std::lock_guard<std::mutex> lock(g_State.stateMutex);
    std::stringstream ss;
    ss << "{";
    ss << "\"type\":\"heartbeat\",";
    ss << "\"data\": {";

    // Room
    ss << "\"room_info\":{";
    ss << "\"players\":[";
    for (size_t i = 0; i < g_State.roomState.players.size(); ++i) {
        auto& p = g_State.roomState.players[i];
        ss << "{";
        ss << "\"ulUid\":" << p.ulUid << ",";
        ss << "\"sName\":\"" << p.sName << "\",";
        ss << "\"heroid\":" << p.heroid << ",";
        ss << "\"uiRankLevel\":" << p.uiRankLevel << ",";

        ss << "\"bRobot\":" << (p.bRobot ? "true" : "false") << ",";
        ss << "\"country\":" << p.country << ",";
        ss << "\"iPos\":" << p.iPos << ",";
        ss << "\"iCamp\":" << p.uiCampType << ",";
        ss << "\"bStarVip\":" << (p.bStarVip ? "true" : "false") << ",";
        ss << "\"bIsOpenLive\":" << (p.bIsOpenLive ? "true" : "false") << ",";
        ss << "\"iTeamId\":" << p.iTeamId << ",";
        ss << "\"iMythPoint\":" << p.iMythPoint << ",";
        ss << "\"uiHeroSkinIDChoose\":" << p.uiHeroSkinIDChoose;

        ss << "}";
        if (i < g_State.roomState.players.size() - 1) ss << ",";
    }
    ss << "]},";

    // BanPick
    ss << "\"ban_pick\":{";
    // Timers placeholder for compatibility
    ss << "\"timers\":{},";
    ss << "\"banList\":[";
    for (size_t i = 0; i < g_State.banPickState.banList.size(); ++i) {
        ss << g_State.banPickState.banList[i];
        if (i < g_State.banPickState.banList.size() - 1) ss << ",";
    }
    ss << "],";
    ss << "\"pickList\":[";
    for (size_t i = 0; i < g_State.banPickState.pickList.size(); ++i) {
        ss << g_State.banPickState.pickList[i];
        if (i < g_State.banPickState.pickList.size() - 1) ss << ",";
    }
    ss << "]},";

    // Battle
    ss << "\"battle_stats\":{";
    ss << "\"m_uiGameTime\":" << g_State.battleStats.m_uiGameTime << ",";
    ss << "\"time\":" << g_State.battleStats.m_uiGameTime << ",";

    ss << "\"m_CampAKill\":" << g_State.battleStats.m_CampAKill << ",";
    ss << "\"m_CampBKill\":" << g_State.battleStats.m_CampBKill << ",";
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

    ss << "\"players\":[";
    for (size_t i = 0; i < g_State.battleStats.playerStats.size(); ++i) {
        auto& p = g_State.battleStats.playerStats[i];
        ss << "{";
        ss << "\"m_uGuid\":" << p.m_uGuid << ",";
        ss << "\"m_KillNum\":" << p.m_KillNum << ",";
        ss << "\"m_DeadNum\":" << p.m_DeadNum << ",";
        ss << "\"m_AssistNum\":" << p.m_AssistNum << ",";
        ss << "\"m_Gold\":" << p.m_Gold << ",";

        ss << "\"iKill3\":" << p.iKill3 << ",";
        ss << "\"iKill4\":" << p.iKill4 << ",";
        ss << "\"iKill5\":" << p.iKill5 << ",";
        ss << "\"m_HurtHeroValue\":" << p.m_HurtHeroValue << ",";
        ss << "\"m_HurtTowerValue\":" << p.m_HurtTowerValue << ",";
        ss << "\"m_InjuredValue\":" << p.m_InjuredValue << ",";
        ss << "\"m_DestroyTowerNum\":" << p.m_DestroyTowerNum << ",";
        ss << "\"m_MonsterCoin\":" << p.m_MonsterCoin;

        ss << "}";
        if (i < g_State.battleStats.playerStats.size() - 1) ss << ",";
    }
    ss << "]";
    ss << "}";

    ss << "}"; // Close Data
    ss << "}"; // Close Root
    return ss.str();
}

// Satisfy linker error AND provide update loop on Main Thread
// Dipanggil dari hack.cpp hook_eglSwapBuffers (Render Thread)
// Memastikan pointer valid karena berada di thread yang sama dengan engine.
void MonitorBattleState() {
    static int frameCount = 0;
    frameCount++;

    // Update setiap 30 frame (approx 0.5 - 1s)
    if (frameCount % 30 == 0) {
        UpdatePlayerInfo();
        UpdateBanPickState();
        UpdateBattleStats();

        std::string json = SerializeState();
        BroadcastData(json);
    }
}

// Initialization
void InitGameLogic() {
    Il2CppAttach("libil2cpp.so");
    // Tidak menjalankan thread terpisah. Logic dipanggil oleh MonitorBattleState di main thread.
    // Menghindari race condition dan pointer yang belum siap.
    LOGI("GameLogic initialized. Waiting for eglSwapBuffers to trigger MonitorBattleState.");
}
