#include "WebServer.h"
#include "GlobalState.h"
#include <thread>
#include <memory>
#include <atomic>
#include "include/httplib.h"
#include <android/log.h>
#include "include/nlohmann/json.hpp"
#include "ConfigManager.h"
#include "feature/BattleData.h"

// Global server instance and thread
std::unique_ptr<httplib::Server> svr;
std::thread server_thread;
std::atomic<bool> is_running(false);

// Fungsi untuk menghentikan server
void StopWebServer() {
    if (svr && is_running) {
        __android_log_print(ANDROID_LOG_INFO, "MLBS_WEB_SERVER", "Stopping web server...");
        svr->stop();
        is_running = false;
        if (server_thread.joinable()) {
            server_thread.join();
        }
        __android_log_print(ANDROID_LOG_INFO, "MLBS_WEB_SERVER", "Web server stopped.");
        g_IsWebServerReady = false;
    }
}

// Implementasi StateToJson
std::string StateToJson() {
    __android_log_print(ANDROID_LOG_INFO, "MLBS_WEB_SERVER", "StateToJson: Acquiring lock...");
    std::lock_guard<std::mutex> lock(g_State.stateMutex);
    __android_log_print(ANDROID_LOG_INFO, "MLBS_WEB_SERVER", "StateToJson: Lock acquired. Serializing state.");
    
    nlohmann::json j;
    j["battleState"] = g_State.battleState;
    j["roomInfoEnabled"] = g_State.roomInfoEnabled;
    j["webServerEnabled"] = g_State.webServerEnabled;

    for (const auto& p : g_State.players) {
        j["players"].push_back({
            {"name", p.name},
            {"uid", p.uid},
            {"rank", p.rank},
            {"spell", p.spell},
            {"hero", p.heroName},
            {"camp", p.camp},
            {"heroId", p.heroId},
            {"spellId", p.spellId},
            {"rankLevel", p.rankLevel}
        });
    }
    
    __android_log_print(ANDROID_LOG_INFO, "MLBS_WEB_SERVER", "StateToJson: Serialization complete. Releasing lock.");
    return j.dump(4);
}

// Fungsi baru untuk serialisasi data room info lengkap
std::string RoomDataToJson() {
    __android_log_print(ANDROID_LOG_INFO, "MLBS_WEB_SERVER", "RoomDataToJson: Acquiring lock...");
    std::lock_guard<std::mutex> lock(g_State.stateMutex);
    __android_log_print(ANDROID_LOG_INFO, "MLBS_WEB_SERVER", "RoomDataToJson: Lock acquired. Serializing full room data.");

    nlohmann::json j;
    j["players"] = nlohmann::json::array();

    for (const auto& p : g_State.players) {
        nlohmann::json player_j;
        player_j["bAutoConditionNew"] = p.bAutoConditionNew;
        player_j["bShowSeasonAchieve"] = p.bShowSeasonAchieve;
        player_j["iStyleBoardId"] = p.iStyleBoardId;
        player_j["iMatchEffectId"] = p.iMatchEffectId;
        player_j["iDayBreakNo1Count"] = p.iDayBreakNo1Count;
        player_j["lUid"] = p.lUid;
        player_j["bUid"] = p.bUid;
        player_j["iCamp"] = p.iCamp;
        player_j["iPos"] = p.iPos;
        player_j["bAutoReadySelect"] = p.bAutoReadySelect;
        player_j["_sName"] = p._sName;
        player_j["bRobot"] = p.bRobot;
        player_j["heroid"] = p.heroid;
        player_j["heroskin"] = p.heroskin;
        player_j["headID"] = p.headID;
        player_j["uiSex"] = p.uiSex;
        player_j["country"] = p.country;
        player_j["uiZoneId"] = p.uiZoneId;
        player_j["summonSkillId"] = p.summonSkillId;
        player_j["runeId"] = p.runeId;
        player_j["mapTalentTree"] = p.mapTalentTree;
        player_j["mRuneSkill2023"] = p.mRuneSkill2023;
        player_j["runeLv"] = p.runeLv;
        player_j["skinlist"] = p.skinlist;
        player_j["facePath"] = p.facePath;
        player_j["faceBorder"] = p.faceBorder;
        player_j["bStarVip"] = p.bStarVip;
        player_j["bMCStarVip"] = p.bMCStarVip;
        player_j["bMCStarVipPlus"] = p.bMCStarVipPlus;
        player_j["ulRoomID"] = p.ulRoomID;
        player_j["iConBlackRoomId"] = p.iConBlackRoomId;
        player_j["banHero"] = p.banHero;
        player_j["vCanSelectHero"] = p.vCanSelectHero;
        player_j["vCanPickHero"] = p.vCanPickHero;
        player_j["uiBattlePlayerType"] = p.uiBattlePlayerType;
        player_j["sThisLoginCountry"] = p.sThisLoginCountry;
        player_j["sCreateRoleCountry"] = p.sCreateRoleCountry;
        player_j["uiLanguage"] = p.uiLanguage;
        player_j["bIsOpenLive"] = p.bIsOpenLive;
        player_j["iTeamId"] = p.iTeamId;
        player_j["iTeamNationId"] = p.iTeamNationId;
        player_j["_steamName"] = p._steamName;
        player_j["_steamSimpleName"] = p._steamSimpleName;
        player_j["iCertify"] = p.iCertify;
        player_j["lsEffectSkins"] = p.lsEffectSkins;
        player_j["lsComEffSkins"] = p.lsComEffSkins;
        player_j["vMissions"] = p.vMissions;
        player_j["uiRankLevel"] = p.uiRankLevel;
        player_j["uiPVPRank"] = p.uiPVPRank;
        player_j["bRankReview"] = p.bRankReview;
        player_j["iElo"] = p.iElo;
        player_j["uiRoleLevel"] = p.uiRoleLevel;
        player_j["bNewPlayer"] = p.bNewPlayer;
        player_j["iRoad"] = p.iRoad;
        player_j["uiSkinSource"] = p.uiSkinSource;
        player_j["iFighterType"] = p.iFighterType;
        player_j["iWorldCupSupportCountry"] = p.iWorldCupSupportCountry;
        player_j["iHeroLevel"] = p.iHeroLevel;
        player_j["iHeroSubLevel"] = p.iHeroSubLevel;
        player_j["iHeroPowerLevel"] = p.iHeroPowerLevel;
        player_j["iActCamp"] = p.iActCamp;
        player_j["vTitle"] = p.vTitle;
        player_j["mHeroMission"] = p.mHeroMission;
        player_j["vEmoji"] = p.vEmoji;
        player_j["vItemBuff"] = p.vItemBuff;
        player_j["vMapPaint"] = p.vMapPaint;
        player_j["mSkinPaint"] = p.mSkinPaint;
        player_j["sClientVersion"] = p.sClientVersion;
        player_j["uiHolyStatue"] = p.uiHolyStatue;
        player_j["uiKamon"] = p.uiKamon;
        player_j["uiUserMapID"] = p.uiUserMapID;
        player_j["iSurviveRank"] = p.iSurviveRank;
        player_j["iDefenceRankID"] = p.iDefenceRankID;
        player_j["iLeagueWCNum"] = p.iLeagueWCNum;
        player_j["iLeagueFCNum"] = p.iLeagueFCNum;
        player_j["iMPLCertifyTime"] = p.iMPLCertifyTime;
        player_j["iMPLCertifyID"] = p.iMPLCertifyID;
        player_j["mapBattleAttr"] = p.mapBattleAttr;
        player_j["iHeroUseCount"] = p.iHeroUseCount;
        player_j["iMythPoint"] = p.iMythPoint;
        player_j["bMythEvaled"] = p.bMythEvaled;
        player_j["iDefenceFlag"] = p.iDefenceFlag;
        player_j["iDefenPoint"] = p.iDefenPoint;
        player_j["iDefenceMap"] = p.iDefenceMap;
        player_j["iAIType"] = p.iAIType;
        player_j["iAISeed"] = p.iAISeed;
        player_j["sAiName"] = p.sAiName;
        player_j["iWarmValue"] = p.iWarmValue;
        player_j["uiAircraftIDChooose"] = p.uiAircraftIDChooose;
        player_j["uiHeroIDChoose"] = p.uiHeroIDChoose;
        player_j["uiHeroSkinIDChoose"] = p.uiHeroSkinIDChoose;
        player_j["uiMapIDChoose"] = p.uiMapIDChoose;
        player_j["uiMapSkinIDChoose"] = p.uiMapSkinIDChoose;
        player_j["uiDefenceRankScore"] = p.uiDefenceRankScore;
        player_j["bBanChat"] = p.bBanChat;
        player_j["iChatBanFinishTime"] = p.iChatBanFinishTime;
        player_j["iChatBanBattleNum"] = p.iChatBanBattleNum;
        player_j["vFastChat"] = p.vFastChat;
        player_j["vWantS"] = p.vWantS;

        j["players"].push_back(player_j);
    }

    __android_log_print(ANDROID_LOG_INFO, "MLBS_WEB_SERVER", "RoomDataToJson: Serialization complete. Releasing lock.");
    return j.dump(4);
}

// Fungsi baru untuk serialisasi data battle dan info room
std::string BattleDataToJson() {
    __android_log_print(ANDROID_LOG_INFO, "MLBS_WEB_SERVER", "BattleDataToJson: Starting serialization.");
    
    // 1. Dapatkan statistik pertempuran real-time
    BattleStats stats = GetBattleStats();

    // 2. Buat objek JSON utama
    nlohmann::json j;

    // 3. Isi sub-objek battle_stats
    j["battle_stats"]["camp_a_kills"] = stats.iCampAKill;
    j["battle_stats"]["camp_b_kills"] = stats.iCampBKill;
    j["battle_stats"]["camp_a_gold"] = stats.CampAGold;
    j["battle_stats"]["camp_b_gold"] = stats.CampBGold;
    j["battle_stats"]["camp_a_exp"] = stats.CampAExp;
    j["battle_stats"]["camp_b_exp"] = stats.CampBExp;
    j["battle_stats"]["camp_a_tower_kills"] = stats.CampAKillTower;
    j["battle_stats"]["camp_b_tower_kills"] = stats.CampBKillTower;
    j["battle_stats"]["camp_a_lord_kills"] = stats.CampAKillLingZhu;
    j["battle_stats"]["camp_b_lord_kills"] = stats.CampBKillLingZhu;
    j["battle_stats"]["camp_a_turtle_kills"] = stats.CampAKillShenGui;
    j["battle_stats"]["camp_b_turtle_kills"] = stats.CampBKillShenGui;

    // 4. Kunci mutex dan isi array pemain
    {
        std::lock_guard<std::mutex> lock(g_State.stateMutex);
        j["players"] = nlohmann::json::array();
        for (const auto& p : g_State.players) {
            j["players"].push_back({
                {"name", p.name},
                {"uid", p.uid},
                {"rank", p.rank},
                {"spell", p.spell},
                {"hero", p.heroName},
                {"camp", p.camp},
                {"heroId", p.heroId},
                {"spellId", p.spellId},
                {"rankLevel", p.rankLevel}
            });
        }
    }
    
    // 5. Kembalikan string JSON
    __android_log_print(ANDROID_LOG_INFO, "MLBS_WEB_SERVER", "BattleDataToJson: Serialization complete.");
    return j.dump(4);
}

// Fungsi untuk serialisasi data Ban/Pick
std::string BanPickToJson() {
    __android_log_print(ANDROID_LOG_INFO, "MLBS_WEB_SERVER", "BanPickToJson: Acquiring lock...");
    std::lock_guard<std::mutex> lock(g_State.stateMutex);
    __android_log_print(ANDROID_LOG_INFO, "MLBS_WEB_SERVER", "BanPickToJson: Lock acquired.");

    nlohmann::json j;
    j["status"] = "success";

    nlohmann::json data;
    data["state"] = g_State.bpState;
    data["timer"] = g_State.bpTimer;

    data["players"] = nlohmann::json::array();
    for (const auto& p : g_State.bpPlayers) {
        data["players"].push_back({
            {"uid", p.uid},
            {"name", p.name},
            {"camp", p.camp},
            {"role", p.role},
            {"spell", p.spell},
            {"ban", p.ban},
            {"pick", p.pick}
        });
    }

    j["data"] = data;

    return j.dump(4);
}


// Fungsi untuk menjalankan server di thread terpisah
void RunServerLoop() {
    // Endpoint untuk mendapatkan state game saat ini (ringkas)
    svr->Get("/state", [](const httplib::Request &, httplib::Response &res) {
        __android_log_print(ANDROID_LOG_INFO, "MLBS_WEB_SERVER", "Received /state request");
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(StateToJson(), "application/json");
        __android_log_print(ANDROID_LOG_INFO, "MLBS_WEB_SERVER", "/state request handled");
    });

    // Endpoint baru untuk mendapatkan data room info lengkap
    svr->Get("/inforoom", [](const httplib::Request &, httplib::Response &res) {
        __android_log_print(ANDROID_LOG_INFO, "MLBS_WEB_SERVER", "Received /inforoom request");
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(RoomDataToJson(), "application/json");
        __android_log_print(ANDROID_LOG_INFO, "MLBS_WEB_SERVER", "/inforoom request handled");
    });

    // Endpoint baru untuk mendapatkan data battle real-time
    svr->Get("/infobattle", [](const httplib::Request &, httplib::Response &res) {
        __android_log_print(ANDROID_LOG_INFO, "MLBS_WEB_SERVER", "Received /infobattle request");
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(BattleDataToJson(), "application/json");
        __android_log_print(ANDROID_LOG_INFO, "MLBS_WEB_SERVER", "/infobattle request handled");
    });

    // Endpoint baru untuk Ban Pick
    svr->Get("/banpick", [](const httplib::Request &, httplib::Response &res) {
        __android_log_print(ANDROID_LOG_INFO, "MLBS_WEB_SERVER", "Received /banpick request");
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(BanPickToJson(), "application/json");
        __android_log_print(ANDROID_LOG_INFO, "MLBS_WEB_SERVER", "/banpick request handled");
    });

    // Endpoint baru untuk mendapatkan waktu pertandingan
    svr->Get("/timebattle", [](const httplib::Request &, httplib::Response &res) {
        __android_log_print(ANDROID_LOG_INFO, "MLBS_WEB_SERVER", "Received /timebattle request");
        res.set_header("Access-Control-Allow-Origin", "*");
        nlohmann::json j;
        j["battle_time"] = GetBattleTime();
        res.set_content(j.dump(), "application/json");
        __android_log_print(ANDROID_LOG_INFO, "MLBS_WEB_SERVER", "/timebattle request handled");
    });

    // Endpoint untuk mengkonfigurasi mod
    svr->Post("/config", [](const httplib::Request &req, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");

        nlohmann::json j = nlohmann::json::parse(req.body, nullptr, false);
        if (j.is_discarded()) {
            res.status = 400;
            res.set_content("{\"status\": \"error\", \"message\": \"Invalid JSON\"}", "application/json");
            return;
        }

        bool stateChanged = false;
        if (j.contains("roomInfoEnabled") && j["roomInfoEnabled"].is_boolean()) {
            __android_log_print(ANDROID_LOG_INFO, "MLBS_WEB_SERVER", "Received /config request for room info");
            std::lock_guard<std::mutex> lock(g_State.stateMutex);
            g_State.roomInfoEnabled = j["roomInfoEnabled"].get<bool>();
            stateChanged = true;
        }

        if (stateChanged) {
            SaveConfig(g_State);
            res.set_content("{\"status\": \"success\"}", "application/json");
        } else {
            res.status = 400;
            res.set_content("{\"status\": \"error\", \"message\": \"Invalid parameter\"}", "application/json");
        }
    });

    // Handle OPTIONS request for CORS preflight on /config
    svr->Options("/config", [](const httplib::Request &, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        res.status = 204;
    });
    
    is_running = true;
    __android_log_print(ANDROID_LOG_INFO, "MLBS_WEB_SERVER", "Starting web server on 0.0.0.0:2626...");
    g_IsWebServerReady = svr->listen("0.0.0.0", 2626);
    is_running = false; // listen has returned, so it's not running anymore

    if (g_IsWebServerReady) {
        __android_log_print(ANDROID_LOG_INFO, "MLBS_WEB_SERVER", "Web server started successfully on port 2626.");
    } else {
        __android_log_print(ANDROID_LOG_ERROR, "MLBS_WEB_SERVER", "Failed to start web server on port 2626.");
    }
}

// Fungsi utama yang dipanggil untuk memulai server
void StartWebServer() {
    if (is_running) {
        __android_log_print(ANDROID_LOG_INFO, "MLBS_WEB_SERVER", "Web server is already running.");
        return;
    }
    
    // Hentikan server lama jika ada
    StopWebServer(); 

    // Buat instance server baru
    svr = std::make_unique<httplib::Server>();

    // Jalankan di thread baru
    server_thread = std::thread(RunServerLoop);
}
