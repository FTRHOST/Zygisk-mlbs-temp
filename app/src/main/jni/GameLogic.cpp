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

// Helper Macros for Safe Memory Reading
#define READ_FIELD(ptr, offset, type) \
    (ptr ? *(type*)((uintptr_t)ptr + offset) : 0)

#define READ_PTR(ptr, offset) \
    (ptr ? *(void**)((uintptr_t)ptr + offset) : nullptr)

// Helper to read C# string (Mono/Il2Cpp style: length + chars)
std::string ReadCSharpString(void* strPtr) {
    if (!strPtr) return "";
    int len = READ_FIELD(strPtr, 0x10, int); // Length is usually at 0x10 for string
    if (len <= 0 || len > 1024) return "";

    // Characters start at 0x14 (UTF-16)
    std::string result;
    const char16_t* chars = (const char16_t*)((uintptr_t)strPtr + 0x14);
    for (int i = 0; i < len; i++) {
        if (chars[i] < 128) {
            result += (char)chars[i];
        } else {
            result += "?";
        }
    }
    return result;
}

// Global State Instance
GlobalState g_State;

// Pointers to Game Singletons/Managers
void* g_SystemData_RoomData_List = nullptr; // List<SystemData.RoomData>
void* g_BattleData_Instance = nullptr;
void* g_ShowFightDataTiny_Instance = nullptr;
void* g_UIRankHero_Instance = nullptr;

// Hooks
void (*orig_UIRankHero_OnUpdate)(void* instance) = nullptr;
void Hook_UIRankHero_OnUpdate(void* instance) {
    g_UIRankHero_Instance = instance;
    if (orig_UIRankHero_OnUpdate) orig_UIRankHero_OnUpdate(instance);
}

// Initialization
void InitGameLogic() {
    Il2CppAttach("libil2cpp.so");

    // Hook UIRankHero Update to get instance for BanPick
    void* updateMethod = Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "UIRankHero", "Update", 0);
    if (updateMethod) {
        DobbyHook(updateMethod, (void*)Hook_UIRankHero_OnUpdate, (void**)&orig_UIRankHero_OnUpdate);
    }
}

// Helper to iterate List<T>
// Returns vector of pointers to T
template <typename T = void*>
std::vector<T> IterateList(void* listPtr) {
    std::vector<T> result;
    if (!listPtr) return result;

    void* itemsArr = READ_PTR(listPtr, 0x10);
    int size = READ_FIELD(listPtr, 0x18, int);

    if (itemsArr && size > 0 && size < 100) {
        for (int i = 0; i < size; i++) {
            void* item = READ_PTR(itemsArr, 0x20 + (i * sizeof(void*)));
            if (item) result.push_back((T)item);
        }
    }
    return result;
}

// Helper for List<uint32_t>
std::vector<uint32_t> IterateIntList(void* listPtr) {
    std::vector<uint32_t> result;
    if (!listPtr) return result;

    void* itemsArr = READ_PTR(listPtr, 0x10);
    int size = READ_FIELD(listPtr, 0x18, int);

    if (itemsArr && size > 0 && size < 100) {
        for (int i = 0; i < size; i++) {
            uint32_t item = READ_FIELD(itemsArr, 0x20 + (i * 4), uint32_t);
            result.push_back(item);
        }
    }
    return result;
}

// Helper for Dictionary<uint, T> iteration
template <typename TValue>
std::vector<TValue> IterateDictionaryValues(void* dictPtr) {
    std::vector<TValue> result;
    if (!dictPtr) return result;

    void* entriesPtr = READ_PTR(dictPtr, 0x18); // entries array
    int count = READ_FIELD(dictPtr, 0x20, int); // count

    if (entriesPtr && count > 0 && count < 100) {
        // Entry size: 0x18 (24 bytes)
        // 0x0: hashCode, 0x4: next, 0x8: key, 0x10: value
        uintptr_t dataStart = (uintptr_t)entriesPtr + 0x20;
        for (int i = 0; i < count; i++) {
             uintptr_t entryAddr = dataStart + (i * 0x18);
             TValue val = READ_FIELD((void*)entryAddr, 0x10, TValue);
             if (val) result.push_back(val);
        }
    }
    return result;
}


// Feature: InfoRoom
void UpdatePlayerInfo() {
    // 1. Find List<SystemData.RoomData>
    if (!g_SystemData_RoomData_List) {
        // Try to find m_quickMatchRoomPayerList in SystemData
        Il2CppGetStaticFieldValue("Assembly-CSharp.dll", "", "SystemData", "m_quickMatchRoomPayerList", &g_SystemData_RoomData_List);
    }

    if (!g_SystemData_RoomData_List) return;

    std::lock_guard<std::mutex> lock(g_State.stateMutex);
    auto& room = g_State.roomState;

    room.players.clear();

    // Iterate the list
    std::vector<void*> roomPlayers = IterateList<void*>(g_SystemData_RoomData_List);

    for (void* playerObj : roomPlayers) {
        if (!playerObj) continue;
        
        PlayerData p;
        p.ulUid = READ_FIELD(playerObj, SystemData_RoomData_lUid, uint64_t);
        p.iPos = READ_FIELD(playerObj, SystemData_RoomData_iPos, int);
        p.uiCampType = READ_FIELD(playerObj, SystemData_RoomData_iCamp, uint32_t);
        
        // Name
        void* namePtr = READ_PTR(playerObj, SystemData_RoomData_sName);
        p.sName = ReadCSharpString(namePtr);

        p.uiHeroIDChoose = READ_FIELD(playerObj, SystemData_RoomData_heroid, uint32_t);
        p.uiRankLevel = READ_FIELD(playerObj, SystemData_RoomData_uiRankLevel, uint32_t);

        room.players.push_back(p);
    }
}

// Feature: BanPick
void UpdateBanPickState() {
    if (!g_UIRankHero_Instance) return;

    std::lock_guard<std::mutex> lock(g_State.stateMutex);
    auto& bp = g_State.banPickState;
    void* inst = g_UIRankHero_Instance;

    void* banOrderList = READ_PTR(inst, UIRankHero_banOrder);
    void* pickOrderList = READ_PTR(inst, UIRankHero_pickOrder);

    bp.banList = IterateIntList(banOrderList);
    bp.pickList = IterateIntList(pickOrderList);
}

// Feature: InfoBattle
void GetBattleStats() {
    if (!g_BattleData_Instance) {
         Il2CppGetStaticFieldValue("Assembly-CSharp.dll", "", "BattleData", "Instance", &g_BattleData_Instance);
    }
    
    if (!g_ShowFightDataTiny_Instance) {
        Il2CppGetStaticFieldValue("Assembly-CSharp.dll", "", "ShowFightDataTiny", "Instance", &g_ShowFightDataTiny_Instance);
    }

    std::lock_guard<std::mutex> lock(g_State.stateMutex);
    auto& stats = g_State.battleStats;

    // 0. Game Time (from BattleData)
    if (g_BattleData_Instance) {
        stats.m_uiGameTime = READ_FIELD(g_BattleData_Instance, BattleData_fGameTime, uint32_t);
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

    // 2. Individual Player Stats from BattleData.heroInfoList
    if (g_BattleData_Instance) {
        void* heroDict = READ_PTR(g_BattleData_Instance, BattleData_heroInfoList);
        if (heroDict) {
            std::vector<void*> heroes = IterateDictionaryValues<void*>(heroDict);
            stats.playerStats.clear();

            for (void* hero : heroes) {
                if (!hero) continue;
                PlayerBattleData p;
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
}

std::string SerializeState() {
    std::lock_guard<std::mutex> lock(g_State.stateMutex);
    std::stringstream ss;
    ss << "{";

    // Room
    ss << "\"room\":{";
    ss << "\"players\":[";
    for (size_t i = 0; i < g_State.roomState.players.size(); ++i) {
        auto& p = g_State.roomState.players[i];
        ss << "{";
        ss << "\"ulUid\":" << p.ulUid << ",";
        ss << "\"sName\":\"" << p.sName << "\",";
        ss << "\"uiHeroIDChoose\":" << p.uiHeroIDChoose;
        ss << "}";
        if (i < g_State.roomState.players.size() - 1) ss << ",";
    }
    ss << "]},";

    // BanPick
    ss << "\"banpick\":{";
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
    ss << "\"battle\":{";
    ss << "\"m_uiGameTime\":" << g_State.battleStats.m_uiGameTime << ",";
    ss << "\"m_CampAKill\":" << g_State.battleStats.m_CampAKill << ",";
    ss << "\"m_CampBKill\":" << g_State.battleStats.m_CampBKill << ",";
    ss << "\"m_CampAGold\":" << g_State.battleStats.m_CampAGold << ",";
    ss << "\"m_CampBGold\":" << g_State.battleStats.m_CampBGold << ",";
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
        ss << "\"m_Gold\":" << p.m_Gold;
        ss << "}";
        if (i < g_State.battleStats.playerStats.size() - 1) ss << ",";
    }
    ss << "]";
    ss << "}";

    ss << "}";
    return ss.str();
}

void GameLogicLoop() {
    while (true) {
        UpdatePlayerInfo();
        UpdateBanPickState();
        GetBattleStats();

        std::string json = SerializeState();
        IpcServer::GetInstance().Broadcast(json);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
