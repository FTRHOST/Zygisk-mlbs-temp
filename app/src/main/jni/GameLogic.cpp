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

// --- HELPER FUNCTIONS ---
std::string HeroToString(int id) { return std::to_string(id); } // Mock
std::string RankToString(int id, int star) { return std::to_string(id); } // Mock
std::string SpellToString(int id) { return std::to_string(id); } // Mock

// Helper to safely read a field
#define READ_FIELD(target, type, offset) \
    if(offset > 0) target = *(type*)((uintptr_t)pawn + offset);

#define READ_STRING(target, offset) \
    if(offset > 0) { \
        MonoString* str = *(MonoString**)((uintptr_t)pawn + offset); \
        if(str) target = str->CString(); \
    }

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

// --- LOGIC IMPLEMENTATION ---

void UpdateBanPickState() {
    if (!g_UIRankHero_Instance) {
        std::lock_guard<std::mutex> lock(g_State.stateMutex);
        g_State.banPickState.isOpen = false;
        return;
    }

    // UIRankHero Instance is Valid
    void* pawn = g_UIRankHero_Instance;

    BanPickState bpState = {};
    bpState.isOpen = true;

    // Read Timers
    // READ_FIELD(bpState.banTime, uint32_t, UIRankHero_iBanTimeSpan); // Check offsets
    // READ_FIELD(bpState.pickTime, uint32_t, UIRankHero_iPickTimeSpan);

    // Read Lists (Ban Order / Pick Order)
    // Using helper functions or manual reading for List<T>
    // Assuming we have offsets for UIRankHero_banOrder (List<uint32>)
    // If not in GameClass.h, we need to add them. They were added.

    if (UIRankHero_banOrder > 0) {
        void* banOrderList = *(void**)((uintptr_t)pawn + UIRankHero_banOrder);
        if (banOrderList) {
            auto* list = (monoList<uint32_t>*)banOrderList;
            int size = list->getSize();
            // Don't read excessively
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

    // Populate Global State
    {
        std::lock_guard<std::mutex> lock(g_State.stateMutex);
        g_State.banPickState = bpState;
    }
}

void UpdateBattleStats() {
    // 1. Get Game Time
    float time = 0.0f;
    void (*GetTime)() = (void (*)())BattleStaticInit_GetTime;
    if (GetTime) {
         // It returns float, cast function pointer correctly
         float (*GetTimeFunc)() = (float (*)())BattleStaticInit_GetTime;
         time = GetTimeFunc();
    }

    // 2. Get Team Stats
    BattleStats stats = GetBattleStats();

    std::lock_guard<std::mutex> lock(g_State.stateMutex);
    g_State.battleStats.gameTime = time;
    g_State.battleStats.campAScore = stats.iCampAKill;
    g_State.battleStats.campBScore = stats.iCampBKill;
    g_State.battleStats.campAGold = stats.CampAGold;
    g_State.battleStats.campBGold = stats.CampBGold;
    g_State.battleStats.campAKillTower = stats.CampAKillTower;
    g_State.battleStats.campBKillTower = stats.CampBKillTower;
    g_State.battleStats.campAKillLord = stats.CampAKillLingZhu;
    g_State.battleStats.campBKillLord = stats.CampBKillLingZhu;
    g_State.battleStats.campAKillTurtle = stats.CampAKillShenGui;
    g_State.battleStats.campBKillTurtle = stats.CampBKillShenGui;
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
        READ_FIELD(p.uiHeroIDChoose, uint32_t, SystemData_RoomData_uiHeroIDChoose); // Picked Hero ID
        
        // Legacy/Computed
        p.name = p._sName;
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
                 ss << "\"name\":\"" << p.name << "\",";
                 ss << "\"rank\":\"" << p.rank << "\",";
                 ss << "\"hero\":\"" << p.heroName << "\",";
                 ss << "\"camp\":" << p.camp << ",";
                 ss << "\"uid\":\"" << p.uid << "\",";
                 ss << "\"spell\":\"" << p.spell << "\",";
                 ss << "\"ban_hero\":" << p.banHero << ",";
                 ss << "\"pick_hero\":" << p.uiHeroIDChoose; // Using heroId or uiHeroIDChoose
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
             ss << "\"camp_a\":{";
             ss << "\"score\":" << g_State.battleStats.campAScore << ",";
             ss << "\"gold\":" << g_State.battleStats.campAGold << ",";
             ss << "\"tower\":" << g_State.battleStats.campAKillTower << ",";
             ss << "\"lord\":" << g_State.battleStats.campAKillLord << ",";
             ss << "\"turtle\":" << g_State.battleStats.campAKillTurtle;
             ss << "},";
             ss << "\"camp_b\":{";
             ss << "\"score\":" << g_State.battleStats.campBScore << ",";
             ss << "\"gold\":" << g_State.battleStats.campBGold << ",";
             ss << "\"tower\":" << g_State.battleStats.campBKillTower << ",";
             ss << "\"lord\":" << g_State.battleStats.campBKillLord << ",";
             ss << "\"turtle\":" << g_State.battleStats.campBKillTurtle;
             ss << "}";
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
