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
#include <fstream>
#include <sys/stat.h>

#include "Include.h"
#include "utils.h"
#include "Il2Cpp/il2cpp_dump.h"
#include "feature/GameClass.h"
#include "feature/ToString.h"
#include "feature/ToString2.h"
#include "feature/BattleData.h" 
#include "struct/LogicPlayer.h" 
#include "IpcServer.h"
#include "obfuscate.h"
#include "dobby.h"
#include "Broadcaster.h"

GlobalState g_State;
std::chrono::steady_clock::time_point g_battleStartTime;
std::chrono::duration<float> g_elapsedBattleTime(0);
std::atomic<bool> g_isBattleTimerRunning(false);

void* g_LogicBattleManager_Instance = nullptr;
void* g_BattleData_Instance = nullptr;

#define LOG_TAG "MLBS_CORE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

std::string SafeReadString(uintptr_t monoStringPtr) {
    if (monoStringPtr == 0) return "";
    int32_t length = 0;
    if (!utils::read_address((void*)(monoStringPtr + 0x10), &length, sizeof(length))) return "";
    if (length <= 0 || length > 1024) return "";
    std::u16string u16;
    u16.resize(length);
    if (!utils::read_address((void*)(monoStringPtr + 0x14), &u16[0], length * sizeof(char16_t))) return "";
    std::string utf8;
    utf8.reserve(length);
    for (char16_t c : u16) {
        if (c < 0x80) utf8.push_back((char)c);
        else utf8.push_back('?'); 
    }
    return utf8;
}

#define READ_FIELD(target, type, offset) \
    if(offset > 0) { \
        utils::read_address((void*)((uintptr_t)pawn + offset), &target, sizeof(type)); \
    }

#define READ_STRING(target, offset) \
    if(offset > 0) { \
        uintptr_t strPtr = 0; \
        if (utils::read_address((void*)((uintptr_t)pawn + offset), &strPtr, sizeof(strPtr)) && strPtr != 0) { \
            target = SafeReadString(strPtr); \
        } \
    }

#define READ_PTR(target, offset) \
    if(offset > 0) { \
        utils::read_address((void*)((uintptr_t)pawn + offset), &target, sizeof(uintptr_t)); \
    }

BattleStats GetBattleStats() {
    BattleStats stats = {};
    void* showFightDataInstance = nullptr;
    Il2CppGetStaticFieldValue(OBFUSCATE("Assembly-CSharp.dll"), "", OBFUSCATE("ShowFightData"), OBFUSCATE("Instance"), &showFightDataInstance);
    if (showFightDataInstance) {
        auto* pData = static_cast<ShowFightDataTiny_Layout*>(showFightDataInstance);
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

void LoadConfig() {
    std::string path = "/storage/emulated/0/Android/data/com.mobile.legends/files/config.json";
    std::ifstream file(path);
    if (!file.is_open()) {
        std::ofstream outfile(path);
        if (outfile.is_open()) {
            outfile << "{\n  \"mod_enabled\": true\n}";
            outfile.close();
        }
        g_State.isModEnabled = true;
        return;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    bool oldEnabled = g_State.isModEnabled;
    g_State.isModEnabled = (content.find("\"mod_enabled\": false") == std::string::npos);
    if (oldEnabled != g_State.isModEnabled) {
        std::lock_guard<std::mutex> lock(g_State.stateMutex);
        g_State.players.clear();
        g_State.logicPlayers.clear();
        g_State.battlePlayers.clear();
    }
}

void UpdateLogicPlayerStats(void* logicBattleManager) {
    if (!logicBattleManager) return;
    {
        std::lock_guard<std::mutex> lock(g_State.stateMutex);
        if (g_State.battleState < 3) return;
    }
    std::vector<LogicPlayerStats> localLogicPlayers;
    auto processList = [&](uintptr_t listOffset) {
        if (listOffset == 0) return;
        void* listPtr = nullptr;
        if (!utils::read_address((void*)((uintptr_t)logicBattleManager + listOffset), &listPtr, sizeof(listPtr)) || !listPtr) return;
        auto* list = (monoList<void*>*)listPtr;
        int size = list->getSize();
        if (size < 0 || size > 20) return; 
        for (int i = 0; i < size; i++) {
            void* pawn = list->getItems()[i];
            if (!pawn) continue;
            LogicPlayerStats s = {};
            READ_PTR(s.m_LoigcBezierBullet_Ptr, LogicPlayer_m_LoigcBezierBullet);
            READ_PTR(s.moveControllers_Ptr, LogicPlayer_moveControllers);
            READ_PTR(s.m_copyHurtCount_Ptr, LogicPlayer_m_copyHurtCount);
            READ_PTR(s.m_dictFirstHitHeroTime_Ptr, LogicPlayer_m_dictFirstHitHeroTime);
            READ_PTR(s.m_listTimeSpent4Kill_Ptr, LogicPlayer_m_listTimeSpent4Kill);
            READ_PTR(s.m_arrSavedPositions_Ptr, LogicPlayer_m_arrSavedPositions);
            READ_PTR(s.hurtInfos_Ptr, LogicPlayer_hurtInfos);
            READ_PTR(s.enemySightLoss_Ptr, LogicPlayer_enemySightLoss);
            READ_PTR(s.endedSightValue_Ptr, LogicPlayer_endedSightValue);
            READ_PTR(s.ongoingSightValue_Ptr, LogicPlayer_ongoingSightValue);
            READ_PTR(s.multiKillAssistIDs_Ptr, LogicPlayer_multiKillAssistIDs);
            READ_PTR(s.m_LogicGuLianBulletManger_Ptr, LogicPlayer_m_LogicGuLianBulletManger);
            READ_PTR(s.m_magicTranSpellSideEffect_Ptr, LogicPlayer_m_magicTranSpellSideEffect);
            READ_PTR(s.m_magicTranSpellStageEffect_Ptr, LogicPlayer_m_magicTranSpellStageEffect);
            READ_PTR(s.m_TwinPlayer_Ptr, LogicPlayer_m_TwinPlayer);
            READ_PTR(s.m_summonTwinAI_Ptr, LogicPlayer_m_summonTwinAI);
            READ_PTR(s.m_AFKTurnAIComponent_Ptr, LogicPlayer_m_AFKTurnAIComponent);
            READ_PTR(s.m_uiAFKHoldCDRangeTimes_Ptr, LogicPlayer_m_uiAFKHoldCDRangeTimes);
            READ_PTR(s.m_SynFightData_Ptr, LogicPlayer_m_SynFightData);
            READ_PTR(s.dicIgnoreOpered_Ptr, LogicPlayer_dicIgnoreOpered);
            READ_PTR(s.m_RelativeScore_Ptr, LogicPlayer_m_RelativeScore);
            READ_PTR(s.dicTalentSkill_Ptr, LogicPlayer_dicTalentSkill);
            READ_PTR(s.dicRuneSkill2023_Ptr, LogicPlayer_dicRuneSkill2023);
            READ_PTR(s.lsMissions_Ptr, LogicPlayer_lsMissions);
            READ_PTR(s.easterEggMissions_Ptr, LogicPlayer_easterEggMissions);
            READ_PTR(s.m_lsEmoji_Ptr, LogicPlayer_m_lsEmoji);
            READ_PTR(s.m_lsAutoEmoji_Ptr, LogicPlayer_m_lsAutoEmoji);
            READ_PTR(s.m_lsAnima_Ptr, LogicPlayer_m_lsAnima);
            READ_PTR(s.m_lsGraffiti_Ptr, LogicPlayer_m_lsGraffiti);
            READ_PTR(s.m_PlayerData_Ptr, LogicPlayer_m_PlayerData);
            READ_PTR(s.m_ConfigData_Ptr, LogicPlayer_m_ConfigData);
            READ_PTR(s.m_HeroCostType_Ptr, LogicPlayer_m_HeroCostType);
            READ_PTR(s.m_BattleConfig_Ptr, LogicPlayer_m_BattleConfig);
            READ_PTR(s.m_TowerTurnData_Ptr, LogicPlayer_m_TowerTurnData);
            READ_PTR(s.m_OperateTimeMonitor_Ptr, LogicPlayer_m_OperateTimeMonitor);
            READ_PTR(s.m_CheckNearComponent_Ptr, LogicPlayer_m_CheckNearComponent);
            READ_PTR(s.m_EstimateAttrComponent_Ptr, LogicPlayer_m_EstimateAttrComponent);
            READ_PTR(s.m_StoreSkillComp_Ptr, LogicPlayer_m_StoreSkillComp);
            READ_PTR(s.m_operCache_Ptr, LogicPlayer_m_operCache);
            READ_PTR(s.m_HighLightComp_Ptr, LogicPlayer_m_HighLightComp);
            READ_PTR(s.m_GankShoeRewardComp_Ptr, LogicPlayer_m_GankShoeRewardComp);
            READ_PTR(s.m_ReqMoveDir_Ptr, LogicPlayer_m_ReqMoveDir);
            READ_PTR(s.m_ReqMovePos_Ptr, LogicPlayer_m_ReqMovePos);
            READ_PTR(s.listKillTime_Ptr, LogicPlayer_listKillTime);
            READ_PTR(s.m_vDelayRemoveSkillIds_Ptr, LogicPlayer_m_vDelayRemoveSkillIds);
            READ_PTR(s.m_HitHeroTimes_SkillGuid_Ptr, LogicPlayer_m_HitHeroTimes_SkillGuid);
            READ_PTR(s.m_dStealValue_Ptr, LogicPlayer_m_dStealValue);
            READ_PTR(s.m_AutoAttackAI_Ptr, LogicPlayer_m_AutoAttackAI);
            READ_PTR(s.m_LogicPunish_Ptr, LogicPlayer_m_LogicPunish);
            READ_PTR(s.m_Killer_Ptr, LogicPlayer_m_Killer);
            READ_PTR(s.m_DevourData_Ptr, LogicPlayer_m_DevourData);
            READ_PTR(s.m_ControlSummer_Ptr, LogicPlayer_m_ControlSummer);
            READ_PTR(s.m_vSkillLogicFighter_Ptr, LogicPlayer_m_vSkillLogicFighter);
            READ_PTR(s.m_vPlayerDeadInfo_Ptr, LogicPlayer_m_vPlayerDeadInfo);
            READ_PTR(s.m_RecmendEquips_Ptr, LogicPlayer_m_RecmendEquips);
            READ_PTR(s.m_v2StarDir_Ptr, LogicPlayer_m_v2StarDir);
            READ_PTR(s.shopData_Ptr, LogicPlayer_shopData);
            READ_PTR(s.v2LastCheckPos_Ptr, LogicPlayer_v2LastCheckPos);
            READ_PTR(s.lastCheckMoveDir_Ptr, LogicPlayer_lastCheckMoveDir);
            READ_PTR(s.lastFailedAutoAiSpellCast_Ptr, LogicPlayer_lastFailedAutoAiSpellCast);
            READ_PTR(s.ownNormalSkillCache_Ptr, LogicPlayer_ownNormalSkillCache);
            READ_PTR(s.lEatFruits_Ptr, LogicPlayer_lEatFruits);
            READ_FIELD(s.m_ID, int32_t, LogicPlayer_m_ID);
            READ_FIELD(s.totalGold, int32_t, LogicPlayer_totalGold);
            READ_FIELD(s.m_HurtTotalValue, double, LogicPlayer_m_HurtTotalValue);
            READ_FIELD(s.m_HurtHeroValue, double, LogicPlayer_m_HurtHeroValue);
            READ_FIELD(s.m_ATKHero, double, LogicPlayer_m_ATKHero);
            READ_FIELD(s.m_iCommonAttackHeroCount, int32_t, LogicPlayer_m_iCommonAttackHeroCount);
            READ_FIELD(s.m_iNormalSkillHeroCount, int32_t, LogicPlayer_m_iNormalSkillHeroCount);
            READ_FIELD(s.m_HurtHeroReel, double, LogicPlayer_m_HurtHeroReel);
            READ_FIELD(s.m_HurtHeroAD, double, LogicPlayer_m_HurtHeroAD);
            READ_FIELD(s.m_HurtHeroAP, double, LogicPlayer_m_HurtHeroAP);
            READ_FIELD(s.m_HurtHeroByEquip, double, LogicPlayer_m_HurtHeroByEquip);
            READ_FIELD(s.m_HurtHeroByEmblem, double, LogicPlayer_m_HurtHeroByEmblem);
            READ_FIELD(s.m_HurtTowerValue, double, LogicPlayer_m_HurtTowerValue);
            READ_FIELD(s.m_HurtSoliderValue, double, LogicPlayer_m_HurtSoliderValue);
            READ_FIELD(s.m_iInjuredShield, int32_t, LogicPlayer_m_iInjuredShield);
            READ_FIELD(s.m_InjuredValue, double, LogicPlayer_m_InjuredValue);
            READ_FIELD(s.m_InjuredTower, double, LogicPlayer_m_InjuredTower);
            READ_FIELD(s.m_InjuredTotal, int32_t, LogicPlayer_m_InjuredTotal);
            READ_FIELD(s.m_InjuredSoldier, double, LogicPlayer_m_InjuredSoldier);
            READ_FIELD(s.m_InjuredAD, double, LogicPlayer_m_InjuredAD);
            READ_FIELD(s.m_InjuredAP, double, LogicPlayer_m_InjuredAP);
            READ_FIELD(s.m_InjuredReal, double, LogicPlayer_m_InjuredReal);
            READ_FIELD(s.m_RealInjuredVal, double, LogicPlayer_m_RealInjuredVal);
            READ_FIELD(s.m_iBeCuredValue, int32_t, LogicPlayer_m_iBeCuredValue);
            READ_FIELD(s.m_CureHero, double, LogicPlayer_m_CureHero);
            READ_FIELD(s.m_CureTeammate, double, LogicPlayer_m_CureTeammate);
            READ_FIELD(s.m_CureSelf, double, LogicPlayer_m_CureSelf);
            READ_FIELD(s.m_CureHeroJustSkill, double, LogicPlayer_m_CureHeroJustSkill);
            READ_FIELD(s.m_iSkillUseCount, int32_t, LogicPlayer_m_iSkillUseCount);
            READ_FIELD(s.m_iCommonAtkUseCount, int32_t, LogicPlayer_m_iCommonAtkUseCount);
            READ_FIELD(s.m_iCommonAtkUseCount_AllSkillCD, int32_t, LogicPlayer_m_iCommonAtkUseCount_AllSkillCD);
            READ_FIELD(s.m_iNormalSkillUseCount, int32_t, LogicPlayer_m_iNormalSkillUseCount);
            READ_FIELD(s.m_iNormalSkillHasDraggedCount, int32_t, LogicPlayer_m_iNormalSkillHasDraggedCount);
            READ_FIELD(s.m_iFirstSkillUseCount, int32_t, LogicPlayer_m_iFirstSkillUseCount);
            READ_FIELD(s.m_iSecondSkillUseCount, int32_t, LogicPlayer_m_iSecondSkillUseCount);
            READ_FIELD(s.m_iThirdSkillUseCount, int32_t, LogicPlayer_m_iThirdSkillUseCount);
            READ_FIELD(s.m_iFourthSkillUseCount, int32_t, LogicPlayer_m_iFourthSkillUseCount);
            READ_FIELD(s.m_iEquipSkillUseCount, int32_t, LogicPlayer_m_iEquipSkillUseCount);
            READ_FIELD(s.m_iCureSkillUseCount, int32_t, LogicPlayer_m_iCureSkillUseCount);
            READ_FIELD(s.m_iBackHomeSkillUseCount, int32_t, LogicPlayer_m_iBackHomeSkillUseCount);
            READ_FIELD(s.m_iSummonSkillUseCount, int32_t, LogicPlayer_m_iSummonSkillUseCount);
            READ_FIELD(s.m_iHuntSkillUseCount, int32_t, LogicPlayer_m_iHuntSkillUseCount);
            READ_FIELD(s.m_iGankSkillUseCount, int32_t, LogicPlayer_m_iGankSkillUseCount);
            READ_FIELD(s.m_iKillMageCount, int32_t, LogicPlayer_m_iKillMageCount);
            READ_FIELD(s.m_iKillMarksmanCount, int32_t, LogicPlayer_m_iKillMarksmanCount);
            READ_FIELD(s.m_iEnterHeroBattleFromGrass, int32_t, LogicPlayer_m_iEnterHeroBattleFromGrass);
            READ_FIELD(s.m_iEnterGrassTimes, int32_t, LogicPlayer_m_iEnterGrassTimes);
            READ_FIELD(s.KillTowerTimes, int32_t, LogicPlayer_KillTowerTimes);
            READ_FIELD(s.KillSoldierTimes, int32_t, LogicPlayer_KillSoldierTimes);
            READ_FIELD(s.m_nSavedPositionsStart, int32_t, LogicPlayer_m_nSavedPositionsStart);
            READ_FIELD(s.m_nSavedPositionsCount, int32_t, LogicPlayer_m_nSavedPositionsCount);
            READ_FIELD(s.m_uLossOfSightTime, uint32_t, LogicPlayer_m_uLossOfSightTime);
            READ_FIELD(s.sightIdGenerator, int32_t, LogicPlayer_sightIdGenerator);
            READ_FIELD(s.continueKill, int32_t, LogicPlayer_continueKill);
            READ_FIELD(s.multiKill, int32_t, LogicPlayer_multiKill);
            READ_FIELD(s.DoubleKillTimes, int32_t, LogicPlayer_DoubleKillTimes);
            READ_FIELD(s.TripleKillTimes, int32_t, LogicPlayer_TripleKillTimes);
            READ_FIELD(s.QuadraKillTimes, int32_t, LogicPlayer_QuadraKillTimes);
            READ_FIELD(s.PentaKillTimes, int32_t, LogicPlayer_PentaKillTimes);
            READ_FIELD(s.greenLightCanUse, bool, LogicPlayer_greenLightCanUse);
            READ_FIELD(s.greenLightStartTime, uint32_t, LogicPlayer_greenLightStartTime);
            READ_FIELD(s.greenLightTimeSpan, uint32_t, LogicPlayer_greenLightTimeSpan);
            READ_FIELD(s.greenLightIgnoreCountDown, bool, LogicPlayer_greenLightIgnoreCountDown);
            READ_FIELD(s.bMonitoringSoloBreakLane, bool, LogicPlayer_bMonitoringSoloBreakLane);
            READ_FIELD(s.uMonitoringTowerGuid, uint32_t, LogicPlayer_uMonitoringTowerGuid);
            READ_FIELD(s.uMonitoringTimeout, uint32_t, LogicPlayer_uMonitoringTimeout);
            READ_FIELD(s.lastReceiveMoveOptTime, uint32_t, LogicPlayer_lastReceiveMoveOptTime);
            READ_FIELD(s.moveProtectTime, int32_t, LogicPlayer_moveProtectTime);
            READ_FIELD(s.m_bMoveProtectAIState, bool, LogicPlayer_m_bMoveProtectAIState);
            READ_FIELD(s.uCheckStarLightTaskTimer, uint32_t, LogicPlayer_uCheckStarLightTaskTimer);
            READ_FIELD(s.uLastGuideSoldier2Tower, uint32_t, LogicPlayer_uLastGuideSoldier2Tower);
            READ_FIELD(s.m_iGuideSoldier2Tower, int32_t, LogicPlayer_m_iGuideSoldier2Tower);
            READ_FIELD(s.m_bIsTwinMain, bool, LogicPlayer_m_bIsTwinMain);
            READ_FIELD(s.m_bIsTwinControl, bool, LogicPlayer_m_bIsTwinControl);
            READ_FIELD(s.bMLAIState, bool, LogicPlayer_bMLAIState);
            READ_FIELD(s.bShowConnectMsg, bool, LogicPlayer_bShowConnectMsg);
            READ_FIELD(s.m_IsRobotPlayer, bool, LogicPlayer_m_IsRobotPlayer);
            READ_FIELD(s.m_uiWaitTrunAITime, uint32_t, LogicPlayer_m_uiWaitTrunAITime);
            READ_FIELD(s.uiQuicklyTrunToAITime, uint32_t, LogicPlayer_uiQuicklyTrunToAITime);
            READ_FIELD(s.uiNomalTurnAITime, uint32_t, LogicPlayer_uiNomalTurnAITime);
            READ_FIELD(s.uIgnoreTurnAITime, uint32_t, LogicPlayer_uIgnoreTurnAITime);
            READ_FIELD(s.iIgnoreOpered, int32_t, LogicPlayer_iIgnoreOpered);
            READ_FIELD(s.m_bForceAi, bool, LogicPlayer_m_bForceAi);
            READ_FIELD(s.m_bWeakNetWork, bool, LogicPlayer_m_bWeakNetWork);
            READ_FIELD(s.m_uLastTimePlayerOpered, uint32_t, LogicPlayer_m_uLastTimePlayerOpered);
            READ_FIELD(s.bWaitTurnAI, bool, LogicPlayer_bWaitTurnAI);
            READ_FIELD(s.uplandRangeDistance, int32_t, LogicPlayer_uplandRangeDistance);
            READ_FIELD(s.m_bConnected, bool, LogicPlayer_m_bConnected);
            READ_FIELD(s.m_uiVoiceParam, uint32_t, LogicPlayer_m_uiVoiceParam);
            READ_FIELD(s.m_iHolyStatueSkillID, int32_t, LogicPlayer_m_iHolyStatueSkillID);
            READ_FIELD(s.m_uHolyStatueID, uint32_t, LogicPlayer_m_uHolyStatueID);
            READ_FIELD(s.m_uHolyStatueIDIfUsed, uint32_t, LogicPlayer_m_uHolyStatueIDIfUsed);
            READ_FIELD(s.m_TotalExp, int32_t, LogicPlayer_m_TotalExp);
            READ_FIELD(s.m_bGankEquip, bool, LogicPlayer_m_bGankEquip);
            READ_FIELD(s.m_bHuntSkill, bool, LogicPlayer_m_bHuntSkill);
            READ_FIELD(s.m_bLowestMoneyOrExp, int32_t, LogicPlayer_m_bLowestMoneyOrExp);
            READ_FIELD(s.m_ShareMoneyEx, double, LogicPlayer_m_ShareMoneyEx);
            READ_FIELD(s.m_ShareExpEx, double, LogicPlayer_m_ShareExpEx);
            READ_FIELD(s.m_RewardMoney, int32_t, LogicPlayer_m_RewardMoney);
            READ_FIELD(s.m_iBaseMoney, int32_t, LogicPlayer_m_iBaseMoney);
            READ_FIELD(s.m_KillBounty, int32_t, LogicPlayer_m_KillBounty);
            READ_FIELD(s.m_bBountyOverThreshold, bool, LogicPlayer_m_bBountyOverThreshold);
            READ_FIELD(s.m_uLastBountyOverThreshold, uint32_t, LogicPlayer_m_uLastBountyOverThreshold);
            READ_FIELD(s.m_iContinueDeadSub, int32_t, LogicPlayer_m_iContinueDeadSub);
            READ_FIELD(s.m_iContinueKillNum, int32_t, LogicPlayer_m_iContinueKillNum);
            READ_FIELD(s.m_iContinueKillAdd, int32_t, LogicPlayer_m_iContinueKillAdd);
            READ_FIELD(s.m_RewardExp, int32_t, LogicPlayer_m_RewardExp);
            READ_FIELD(s.m_iBaseExp, int32_t, LogicPlayer_m_iBaseExp);
            READ_FIELD(s.m_iLevelExp, int32_t, LogicPlayer_m_iLevelExp);
            READ_FIELD(s.m_iLvExpRate, double, LogicPlayer_m_iLvExpRate);
            READ_FIELD(s.m_fContinueDeadPara, double, LogicPlayer_m_fContinueDeadPara);
            READ_FIELD(s.DeadAndKillTimes, int32_t, LogicPlayer_DeadAndKillTimes);
            READ_FIELD(s.m_AssistTimesReward, int32_t, LogicPlayer_m_AssistTimesReward);
            READ_FIELD(s.m_bReqMoveUpdate, bool, LogicPlayer_m_bReqMoveUpdate);
            READ_FIELD(s.bDeathHoldKillCount, bool, LogicPlayer_bDeathHoldKillCount);
            READ_FIELD(s.mShutDown, int32_t, LogicPlayer_mShutDown);
            READ_FIELD(s.lastKillTime, uint32_t, LogicPlayer_lastKillTime);
            READ_FIELD(s.mutiKillUsefulTime, uint32_t, LogicPlayer_mutiKillUsefulTime);
            READ_FIELD(s.mutiKillUsefulTimeOn5kill, uint32_t, LogicPlayer_mutiKillUsefulTimeOn5kill);
            READ_FIELD(s.m_uiLastMoveTime, uint32_t, LogicPlayer_m_uiLastMoveTime);
            READ_FIELD(s.m_GetGoldTimesBySoldier, int32_t, LogicPlayer_m_GetGoldTimesBySoldier);
            READ_FIELD(s.m_BeyondGodlike, int32_t, LogicPlayer_m_BeyondGodlike);
            READ_FIELD(s.m_MaxMutiKill, int32_t, LogicPlayer_m_MaxMutiKill);
            READ_FIELD(s.m_MaxContinueKill, int32_t, LogicPlayer_m_MaxContinueKill);
            READ_FIELD(s.m_singleKill, int32_t, LogicPlayer_m_singleKill);
            READ_FIELD(s.m_KillLingZhu, int32_t, LogicPlayer_m_KillLingZhu);
            READ_FIELD(s.m_AssistLingZhu, int32_t, LogicPlayer_m_AssistLingZhu);
            READ_FIELD(s.KillWildTimes, int32_t, LogicPlayer_KillWildTimes);
            READ_FIELD(s.m_WeekKill, int32_t, LogicPlayer_m_WeekKill);
            READ_FIELD(s.m_KillShenGui, int32_t, LogicPlayer_m_KillShenGui);
            READ_FIELD(s.m_AssistShenGui, int32_t, LogicPlayer_m_AssistShenGui);
            READ_FIELD(s.m_KillCdMonster, int32_t, LogicPlayer_m_KillCdMonster);
            READ_FIELD(s.m_KillAtkMonster, int32_t, LogicPlayer_m_KillAtkMonster);
            READ_FIELD(s.m_KillMePlayerCount, int32_t, LogicPlayer_m_KillMePlayerCount);
            READ_FIELD(s.m_CurZoneId, int32_t, LogicPlayer_m_CurZoneId);
            READ_FIELD(s.m_HurtTurtle, double, LogicPlayer_m_HurtTurtle);
            READ_FIELD(s.m_HurtLord, double, LogicPlayer_m_HurtLord);
            READ_FIELD(s.m_ShieldCureHero, double, LogicPlayer_m_ShieldCureHero);
            READ_FIELD(s.m_ShieldCureSelf, double, LogicPlayer_m_ShieldCureSelf);
            READ_FIELD(s.m_ShieldTeammate, double, LogicPlayer_m_ShieldTeammate);
            READ_FIELD(s.m_SufferControlTime, int32_t, LogicPlayer_m_SufferControlTime);
            READ_FIELD(s.m_SufferSlowTime, int32_t, LogicPlayer_m_SufferSlowTime);
            READ_FIELD(s.m_ControlTime, int32_t, LogicPlayer_m_ControlTime);
            READ_FIELD(s.m_KillsWithRedAndBlueBuff, int32_t, LogicPlayer_m_KillsWithRedAndBlueBuff);
            READ_FIELD(s.m_MoveDis, double, LogicPlayer_m_MoveDis);
            READ_FIELD(s.m_MoveDisTickCount, double, LogicPlayer_m_MoveDisTickCount);
            READ_FIELD(s.m_MoveCountPrePosX, double, LogicPlayer_m_MoveCountPrePosX);
            READ_FIELD(s.m_MoveCountPrePosY, double, LogicPlayer_m_MoveCountPrePosY);
            READ_FIELD(s.m_GoldByWild, int32_t, LogicPlayer_m_GoldByWild);
            READ_FIELD(s.m_GoldBySoldier, int32_t, LogicPlayer_m_GoldBySoldier);
            READ_FIELD(s.m_GoldByHero, int32_t, LogicPlayer_m_GoldByHero);
            READ_FIELD(s.iAllHurtVal, int32_t, LogicPlayer_iAllHurtVal);
            READ_FIELD(s.m_CrlTimes, int32_t, LogicPlayer_m_CrlTimes);
            READ_FIELD(s.m_iPoisonValue, int32_t, LogicPlayer_m_iPoisonValue);
            READ_FIELD(s.m_hurtEnemyWild, double, LogicPlayer_m_hurtEnemyWild);
            READ_FIELD(s.m_hurtWildValue, double, LogicPlayer_m_hurtWildValue);
            READ_FIELD(s.m_TrunSpeed, double, LogicPlayer_m_TrunSpeed);
            READ_FIELD(s.m_GreatGuid, uint32_t, LogicPlayer_m_GreatGuid);
            READ_FIELD(s.m_bRefuseSelectAIType, bool, LogicPlayer_m_bRefuseSelectAIType);
            READ_FIELD(s.m_uiLastOperFrameTime, uint32_t, LogicPlayer_m_uiLastOperFrameTime);
            READ_FIELD(s.SummonSkillId, int32_t, LogicPlayer_SummonSkillId);
            READ_FIELD(s.m_SummonStartSkillId, int32_t, LogicPlayer_m_SummonStartSkillId);
            READ_FIELD(s.m_RankLv, uint32_t, LogicPlayer_m_RankLv);
            READ_FIELD(s.m_bigRankLv, uint32_t, LogicPlayer_m_bigRankLv);
            READ_FIELD(s.m_rankStar, uint32_t, LogicPlayer_m_rankStar);
            READ_FIELD(s.m_rankNum, uint32_t, LogicPlayer_m_rankNum);
            READ_FIELD(s.m_lastReliveTime, uint32_t, LogicPlayer_m_lastReliveTime);
            READ_FIELD(s.m_ReviveTimeMs, uint32_t, LogicPlayer_m_ReviveTimeMs);
            READ_FIELD(s.m_bFastDie, bool, LogicPlayer_m_bFastDie);
            READ_FIELD(s.m_EatFruit, uint32_t, LogicPlayer_m_EatFruit);
            READ_FIELD(s.m_KillByFruit, uint32_t, LogicPlayer_m_KillByFruit);
            READ_FIELD(s.m_GetFruitOnMin, uint32_t, LogicPlayer_m_GetFruitOnMin);
            READ_FIELD(s.bAllowRelive, bool, LogicPlayer_bAllowRelive);
            READ_FIELD(s.m_uiRoleLevel, uint32_t, LogicPlayer_m_uiRoleLevel);
            READ_FIELD(s.m_iAddGoldValue, int32_t, LogicPlayer_m_iAddGoldValue);
            READ_FIELD(s.iMaxHurtValue, int32_t, LogicPlayer_iMaxHurtValue);
            READ_FIELD(s.m_iSkinId, int32_t, LogicPlayer_m_iSkinId);
            READ_FIELD(s.m_iDragonCrystalId, int32_t, LogicPlayer_m_iDragonCrystalId);
            READ_FIELD(s.m_uUserMapID, uint32_t, LogicPlayer_m_uUserMapID);
            READ_FIELD(s.iLastGiveupEquip, int32_t, LogicPlayer_iLastGiveupEquip);
            READ_FIELD(s.m_iSurvivalTime, uint32_t, LogicPlayer_m_iSurvivalTime);
            READ_FIELD(s.m_iChickenRanking, uint32_t, LogicPlayer_m_iChickenRanking);
            READ_FIELD(s.m_bEmojiBirthday, bool, LogicPlayer_m_bEmojiBirthday);
            READ_FIELD(s.logAttackSpeed, bool, LogicPlayer_logAttackSpeed);
            READ_FIELD(s.doAttackSpeed, bool, LogicPlayer_doAttackSpeed);
            READ_FIELD(s.m_CommATK_RunTimer, uint32_t, LogicPlayer_m_CommATK_RunTimer);
            READ_FIELD(s.m_dCommATKSingTime_Mod, int32_t, LogicPlayer_m_dCommATKSingTime_Mod);
            READ_FIELD(s.m_CommATKSingTime_LastTimer, uint32_t, LogicPlayer_m_CommATKSingTime_LastTimer);
            READ_FIELD(s.m_dCommATKCD_Mod, int32_t, LogicPlayer_m_dCommATKCD_Mod);
            READ_FIELD(s.m_CommATKCD_LastTimer, uint32_t, LogicPlayer_m_CommATKCD_LastTimer);
            READ_FIELD(s.m_PriorEquip, uint32_t, LogicPlayer_m_PriorEquip);
            READ_FIELD(s.m_uHeroEnhanceLevel, uint32_t, LogicPlayer_m_uHeroEnhanceLevel);
            READ_FIELD(s.m_bGhostHasDied, bool, LogicPlayer_m_bGhostHasDied);
            READ_FIELD(s.lastCheckDirSymbol, int32_t, LogicPlayer_lastCheckDirSymbol);
            READ_FIELD(s.right, int32_t, LogicPlayer_right);
            READ_FIELD(s.lastFailedAutoAiSpellCastTime, uint32_t, LogicPlayer_lastFailedAutoAiSpellCastTime);
            READ_FIELD(s.autoTime, int32_t, LogicPlayer_autoTime);
            READ_FIELD(s.m_dXpGrowthDecimal, double, LogicPlayer_m_dXpGrowthDecimal);
            READ_FIELD(s.bBornedBoss, bool, LogicPlayer_bBornedBoss);
            READ_FIELD(s.iPreMutiKillValue, int32_t, LogicPlayer_iPreMutiKillValue);
            READ_FIELD(s.iPreContinueKillValue, int32_t, LogicPlayer_iPreContinueKillValue);
            READ_FIELD(s.iPreKillLingZhu, int32_t, LogicPlayer_iPreKillLingZhu);
            READ_FIELD(s.iPreKillShenGui, int32_t, LogicPlayer_iPreKillShenGui);
            READ_FIELD(s.iPreShutDown, int32_t, LogicPlayer_iPreShutDown);
            READ_FIELD(s.bCheckFirstBlood, bool, LogicPlayer_bCheckFirstBlood);
            READ_FIELD(s.iCurrentResult, int32_t, LogicPlayer_iCurrentResult);
            READ_FIELD(s.iPreGetResultTime, uint32_t, LogicPlayer_iPreGetResultTime);
            READ_FIELD(s.iCurKilledResult, int32_t, LogicPlayer_iCurKilledResult);
            READ_FIELD(s.iPreKilledResultTime, uint32_t, LogicPlayer_iPreKilledResultTime);
            localLogicPlayers.push_back(s);
        }
    };
    processList(0x100);
    processList(0x108);
    { std::lock_guard<std::mutex> lock(g_State.stateMutex); g_State.logicPlayers = localLogicPlayers; }
}

void UpdateBattleStats(void* logicBattleManager) {
    float time = 0.0f;
    if (BattleStaticInit_GetTime) {
         float (*GetTimeFunc)() = (float (*)())BattleStaticInit_GetTime;
         time = GetTimeFunc();
    }
    BattleStats stats = GetBattleStats();
    std::vector<PlayerBattleData> localBattlePlayers;
    void* battleDataInstance = nullptr;
    Il2CppGetStaticFieldValue(OBFUSCATE("Assembly-CSharp.dll"), "", OBFUSCATE("BattleData"), OBFUSCATE("Instance"), &battleDataInstance);
    if (battleDataInstance && BattleData_heroInfoList > 0) {
        void* dictPtr = nullptr;
        if (utils::read_address((void*)((uintptr_t)battleDataInstance + BattleData_heroInfoList), &dictPtr, sizeof(dictPtr)) && dictPtr) {
            auto* dictionary = (Dictionary<uint32_t, void*>*)dictPtr;
            if (dictionary->entries && dictionary->count > 0 && dictionary->count < 50) {
                auto entries = dictionary->entries->toCPPlist();
                for (auto& entry : entries) {
                    void* fightHeroInfo = entry.value;
                    if (!fightHeroInfo) continue;
                    PlayerBattleData pb = {};
                    void* pawn = fightHeroInfo; 
                    READ_FIELD(pb.uGuid, uint32_t, FightHeroInfo_m_uGuid);
                    READ_STRING(pb.playerName, FightHeroInfo_m_PlayerName);
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
        g_State.battlePlayers = localBattlePlayers;
    }
    UpdateLogicPlayerStats(logicBattleManager);
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
        READ_FIELD(p.uiHeroIDChoose, uint32_t, SystemData_RoomData_uiHeroIDChoose);
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
        READ_FIELD(p.bNewPlayer, bool, SystemData_RoomData_bNewPlayer);
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
        READ_FIELD(p.bForbidUseFaceName, bool, SystemData_RoomData_bForbidUseFaceName);
        READ_STRING(p.sClientIp, SystemData_RoomData_sClientIp);
        READ_FIELD(p.iRoomOrder, uint32_t, SystemData_RoomData_iRoomOrder);
        READ_PTR(p.vRougeTotalSkill_Ptr, SystemData_RoomData_vRougeTotalSkill);
        READ_PTR(p.vRougeOMGSkill_Ptr, SystemData_RoomData_vRougeOMGSkill);
        READ_PTR(p.vRecommendEquipList_Ptr, SystemData_RoomData_vRecommendEquipList);
        READ_STRING(p.sRecommendEquipVersion, SystemData_RoomData_sRecommendEquipVersion);
        READ_PTR(p.vPingParamDetail_Ptr, SystemData_RoomData_vPingParamDetail);
        READ_FIELD(p.uiPlayerPing, uint32_t, SystemData_RoomData_uiPlayerPing);
        READ_FIELD(p.mSkinRankSeasonTag_Ptr, uint32_t, SystemData_RoomData_mSkinRankSeasonTag);
        READ_FIELD(p.mSkinNumTag_Ptr, uint32_t, SystemData_RoomData_mSkinNumTag);
        READ_FIELD(p.bFullSkillaber, bool, SystemData_RoomData_bFullSkillaber);
        READ_FIELD(p.uiCommanderSkinAttackEffect, uint32_t, SystemData_RoomData_uiCommanderSkinAttackEffect);
        READ_FIELD(p.uiDailyFreeRandomNum, uint32_t, SystemData_RoomData_uiDailyFreeRandomNum);
        READ_FIELD(p.bIllustrateCornerEffectClose, bool, SystemData_RoomData_bIllustrateCornerEffectClose);
        READ_FIELD(p.bTagedBackOf2022, bool, SystemData_RoomData_bTagedBackOf2022);
        READ_FIELD(p.iTapConflictTipNum, uint32_t, SystemData_RoomData_iTapConflictTipNum);
        READ_FIELD(p.iNameShowType, uint32_t, SystemData_RoomData_iNameShowType);
        READ_FIELD(p.bOpenHighLight, bool, SystemData_RoomData_bOpenHighLight);
        READ_FIELD(p.mMCBanPickCommander_Ptr, uint32_t, SystemData_RoomData_mMCBanPickCommander);
        READ_PTR(p.vForbidBanCommander_Ptr, SystemData_RoomData_vForbidBanCommander);
        READ_FIELD(p.iTeamLevel, uint32_t, SystemData_RoomData_iTeamLevel);
        READ_PTR(p.vAdditionalHero_Ptr, SystemData_RoomData_vAdditionalHero);
        READ_FIELD(p.uiDisorderPublicHeroScore, uint32_t, SystemData_RoomData_uiDisorderPublicHeroScore);
        READ_FIELD(p.bPlayerBirthdayToday, bool, SystemData_RoomData_bPlayerBirthdayToday);
        READ_FIELD(p.iTeamHeadId, uint32_t, SystemData_RoomData_iTeamHeadId);
        READ_PTR(p.mapHeroBattleNum_Ptr, SystemData_RoomData_mapHeroBattleNum);
        READ_PTR(p.vCurSeasonRealRoadInfo_Ptr, SystemData_RoomData_vCurSeasonRealRoadInfo);
        READ_PTR(p.vCultivateRoadShow_Ptr, SystemData_RoomData_vCultivateRoadShow);
        READ_FIELD(p.uiCommanderLevel, uint32_t, SystemData_RoomData_uiCommanderLevel);
        READ_FIELD(p.bOpenSubRankID, bool, SystemData_RoomData_bOpenSubRankID);
        READ_FIELD(p.iSubRankID, uint32_t, SystemData_RoomData_iSubRankID);
        READ_FIELD(p.iSingleLv, uint32_t, SystemData_RoomData_iSingleLv);
        READ_FIELD(p.stArenaMatchBattleInfo_Ptr, uint32_t, SystemData_RoomData_stArenaMatchBattleInfo);
        READ_FIELD(p.stArenaMatchShowInfo_Ptr, uint32_t, SystemData_RoomData_stArenaMatchShowInfo);
        READ_FIELD(p.stSkinAttach_Ptr, uint32_t, SystemData_RoomData_stSkinAttach);
        READ_FIELD(p.iMatchTeamId, uint64_t, SystemData_RoomData_iMatchTeamId);
        READ_FIELD(p.iFlowBackTYpe, uint32_t, SystemData_RoomData_iFlowBackTYpe);
        READ_FIELD(p.bRoadAdditionCover, bool, SystemData_RoomData_bRoadAdditionCover);
        READ_FIELD(p.iRoadAdditionCoverTimes, uint32_t, SystemData_RoomData_iRoadAdditionCoverTimes);
        READ_FIELD(p.iRoomPos, uint32_t, SystemData_RoomData_iRoomPos);
        READ_FIELD(p.stEasterEggInfo_Ptr, uint32_t, SystemData_RoomData_stEasterEggInfo);
        READ_STRING(p.sMatchTeamName, SystemData_RoomData_sMatchTeamName);
        READ_FIELD(p.iMatchTeamFaceId, uint32_t, SystemData_RoomData_iMatchTeamFaceId);
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
    static int configTick = 0;
    if (++configTick >= 180) { LoadConfig(); configTick = 0; }
    if (!g_State.isModEnabled) {
        std::lock_guard<std::mutex> lock(g_State.stateMutex);
        g_State.players.clear(); g_State.logicPlayers.clear(); return;
    }
    static int logicTick = 0, infoTick = 0;
    logicTick++; infoTick++;
    void *logicBattleManager = nullptr;
    Il2CppGetStaticFieldValue(OBFUSCATE("Assembly-CSharp.dll"), "", OBFUSCATE("LogicBattleManager"), OBFUSCATE("Instance"), &logicBattleManager);
    bool isManagerValid = (logicBattleManager != nullptr);
    int currentBattleState = -1;
    if (isManagerValid) {
        currentBattleState = GetBattleState(logicBattleManager);
        { std::lock_guard<std::mutex> lock(g_State.stateMutex); g_State.battleState = currentBattleState; }
        
        if (currentBattleState == 6 && !g_isBattleTimerRunning) {
             g_battleStartTime = std::chrono::steady_clock::now();
             g_isBattleTimerRunning = true;
        } else if (currentBattleState == 7 && g_isBattleTimerRunning) {
             g_isBattleTimerRunning = false;
        }
        if (g_isBattleTimerRunning) {
             g_elapsedBattleTime = std::chrono::steady_clock::now() - g_battleStartTime;
        }

        if (currentBattleState == 2 && infoTick >= 60) { UpdatePlayerInfo(); infoTick = 0; }
        else if (currentBattleState >= 3) {
             if (logicTick >= 15) { UpdateBattleStats(logicBattleManager); logicTick = 0; }
             if (infoTick >= 60) { UpdatePlayerInfo(); infoTick = 0; }
        }
    }
    static int frameTick = 0;
    if (++frameTick % 60 == 0) {
        std::stringstream ss;
        ss << "{\"type\":\"heartbeat\",\"debug\":{\"manager_found\":" << (isManagerValid?"true":"false") << ",\"game_state\":" << currentBattleState << ",\"feature_enabled\":true},\"data\":{";
        {
             std::lock_guard<std::mutex> lock(g_State.stateMutex);
             ss << "\"room_info\":{\"player_count\":" << g_State.players.size() << ",\"players\":[";
             for (size_t i = 0; i < g_State.players.size(); ++i) {
                 const auto& p = g_State.players[i];
                 ss << "{\"lUid\":" << p.lUid << ",\"_sName\":\"" << p._sName << "\",\"iCamp\":" << p.iCamp << ",\"heroid\":" << p.heroid << ",\"uiRankLevel\":" << p.uiRankLevel << ",\"summonSkillId\":" << p.summonSkillId 
                    << ",\"banHero\":" << p.banHero << ",\"iRoad\":" << p.iRoad << ",\"uiZoneId\":" << p.uiZoneId << ",\"heroskin\":" << p.heroskin << "}";
                 if (i < g_State.players.size() - 1) ss << ",";
             }
             ss << "],\"logic_players\":[";
             for (size_t i = 0; i < g_State.logicPlayers.size(); ++i) {
                 const auto& s = g_State.logicPlayers[i];
                 ss << "{\"m_ID\":" << s.m_ID << ",\"totalGold\":" << s.totalGold 
                    << ",\"_TripleKillTimes\":" << s.TripleKillTimes 
                    << ",\"_QuadraKillTimes\":" << s.QuadraKillTimes 
                    << ",\"_PentaKillTimes\":" << s.PentaKillTimes 
                    << ",\"m_TotalExp\":" << s.m_TotalExp << "}";
                 if (i < g_State.logicPlayers.size() - 1) ss << ",";
             }
             ss << "]},\"battle_stats\":{\"time\":" << g_elapsedBattleTime.count() 
                << ",\"m_iCampAKill\":" << g_State.battleStats.m_iCampAKill 
                << ",\"m_iCampBKill\":" << g_State.battleStats.m_iCampBKill 
                << ",\"m_CampAGold\":" << g_State.battleStats.m_CampAGold 
                << ",\"m_CampBGold\":" << g_State.battleStats.m_CampBGold
                << ",\"m_CampAExp\":" << g_State.battleStats.m_CampAExp
                << ",\"m_CampBExp\":" << g_State.battleStats.m_CampBExp
                << ",\"m_CampAKillTower\":" << g_State.battleStats.m_CampAKillTower
                << ",\"m_CampBKillTower\":" << g_State.battleStats.m_CampBKillTower
                << ",\"m_CampAKillLingZhu\":" << g_State.battleStats.m_CampAKillLingZhu
                << ",\"m_CampBKillLingZhu\":" << g_State.battleStats.m_CampBKillLingZhu
                << ",\"m_CampAKillShenGui\":" << g_State.battleStats.m_CampAKillShenGui
                << ",\"m_CampBKillShenGui\":" << g_State.battleStats.m_CampBKillShenGui
                << "}}}";
        }
        BroadcastData(ss.str());
    }
}

// Hook definitions
void (*old_ISHOW_Change_CampGold)(void* instance, uint32_t campID, uint32_t goldAmount);
void hook_ISHOW_Change_CampGold(void* instance, uint32_t campID, uint32_t goldAmount) {
    if (old_ISHOW_Change_CampGold) {
        old_ISHOW_Change_CampGold(instance, campID, goldAmount);
    }
    std::stringstream ss;
    ss << "{\"camp\": " << campID << ", \"gold\": " << goldAmount << "}";
    Broadcaster::SendEvent(EVENT_GOLD_UPDATE, ss.str());
}

void (*old_ISHOW_Change_KillInfo)(void* instance, int32_t campAKill, int32_t campBKill, uint32_t killerId, uint32_t victimId);
void hook_ISHOW_Change_KillInfo(void* instance, int32_t campAKill, int32_t campBKill, uint32_t killerId, uint32_t victimId) {
    if (old_ISHOW_Change_KillInfo) {
        old_ISHOW_Change_KillInfo(instance, campAKill, campBKill, killerId, victimId);
    }
    std::stringstream ss;
    ss << "{\"campA_kill\": " << campAKill << ", \"campB_kill\": " << campBKill << ", \"killer\": " << killerId << ", \"victim\": " << victimId << "}";
    Broadcaster::SendEvent(EVENT_KILL_UPDATE, ss.str());
}

void (*old_ReceBanHero)(void* instance, uint32_t heroId, uint64_t lUid, uint32_t iTime, void* data);
void hook_ReceBanHero(void* instance, uint32_t heroId, uint64_t lUid, uint32_t iTime, void* data) {
    if (old_ReceBanHero) {
        old_ReceBanHero(instance, heroId, lUid, iTime, data);
    }
    std::stringstream ss;
    ss << "{\"heroId\": " << heroId << ", \"uid\": " << lUid << ", \"time\": " << iTime << ", \"type\": \"ban\"}";
    Broadcaster::SendEvent(EVENT_BAN_PICK, ss.str());
}

void InitGameLogic() {
    LoadConfig();

    // Install hooks
    void* addrChangeCampGold = Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "BattleData", "ISHOW_Change_CampGold", 2);
    if (addrChangeCampGold) {
        DobbyHook(addrChangeCampGold, (void*)hook_ISHOW_Change_CampGold, (void**)&old_ISHOW_Change_CampGold);
        LOGI("Hooked ISHOW_Change_CampGold at %p", addrChangeCampGold);
    } else {
        LOGE("Failed to find ISHOW_Change_CampGold");
    }

    void* addrChangeKillInfo = Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "BattleData", "ISHOW_Change_KillInfo", 4);
    if (addrChangeKillInfo) {
        DobbyHook(addrChangeKillInfo, (void*)hook_ISHOW_Change_KillInfo, (void**)&old_ISHOW_Change_KillInfo);
        LOGI("Hooked ISHOW_Change_KillInfo at %p", addrChangeKillInfo);
    } else {
        LOGE("Failed to find ISHOW_Change_KillInfo");
    }

    void* addrReceBanHero = Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "UIRankHero", "ReceBanHero", 4);
    if (addrReceBanHero) {
        DobbyHook(addrReceBanHero, (void*)hook_ReceBanHero, (void**)&old_ReceBanHero);
        LOGI("Hooked ReceBanHero at %p", addrReceBanHero);
    } else {
        LOGE("Failed to find ReceBanHero");
    }

    LOGI("GameLogic Initialized. Mod Enabled: %s", g_State.isModEnabled ? "true" : "false");
}
