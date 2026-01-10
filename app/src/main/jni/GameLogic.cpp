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
#include "struct/LogicPlayer.h" // Include LogicPlayer offsets
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

void UpdateLogicPlayerStats(void* logicBattleManager) {
    if (!logicBattleManager) return;

    std::vector<LogicPlayerStats> localLogicPlayers;

    // Helper lambda to process a list of LogicFighters (assumed to be LogicPlayers)
    auto processList = [&](uintptr_t listOffset) {
        if (listOffset == 0) return;
        void* listPtr = *(void**)((uintptr_t)logicBattleManager + listOffset);
        if (!listPtr) return;

        auto* list = (monoList<void*>*)listPtr;
        int size = list->getSize();

        for (int i = 0; i < size; i++) {
            void* pawn = list->getItems()[i];
            if (!pawn) continue;

            LogicPlayerStats s = {};

            // Use READ_FIELD macro or similar logic manually since READ_FIELD expects 'pawn' var
            // and we have 'pawn' here.

            // Pointers
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

            // Player Info from m_PlayerData
            if (s.m_PlayerData_Ptr) {
                // Manually read from m_PlayerData_Ptr using offsets
                s.heroId = *(uint32_t*)(s.m_PlayerData_Ptr + SystemData_RoomData_heroid);

                MonoString* nameStr = *(MonoString**)(s.m_PlayerData_Ptr + SystemData_RoomData__sName);
                if (nameStr) {
                    s.playerName = nameStr->CString();
                } else {
                    s.playerName = "";
                }
            }

            // Simple Values
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

    // Process CampA and CampB players
    // m_CampAPlayerList is at 0x100, m_CampBPlayerList is at 0x108
    processList(0x100);
    processList(0x108);

    {
        std::lock_guard<std::mutex> lock(g_State.stateMutex);
        g_State.logicPlayers = localLogicPlayers;
    }
}

void UpdateBattleStats(void* logicBattleManager) {
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

        // Copy all raw fields to Global State
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

        // Also map legacy fields to keep structure valid but use new names under the hood
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

    // Update Logic Players (New Feature)
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
        else if (currentBattleState >= 3) { // In-Game (3, 6, 7 etc)
             UpdateBattleStats(logicBattleManager); // Update Score, Gold, Time, LogicPlayer
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

             // --- LOGIC PLAYER STATS (NEW) ---
             ss << ",\"logic_players\":[";
             for (size_t i = 0; i < g_State.logicPlayers.size(); ++i) {
                 const auto& s = g_State.logicPlayers[i];
                 ss << "{";

                 // Pointers (as hex strings or raw ints)
                 ss << "\"m_LoigcBezierBullet_Ptr\":" << s.m_LoigcBezierBullet_Ptr << ",";
                 ss << "\"moveControllers_Ptr\":" << s.moveControllers_Ptr << ",";
                 ss << "\"m_copyHurtCount_Ptr\":" << s.m_copyHurtCount_Ptr << ",";
                 ss << "\"m_dictFirstHitHeroTime_Ptr\":" << s.m_dictFirstHitHeroTime_Ptr << ",";
                 ss << "\"m_listTimeSpent4Kill_Ptr\":" << s.m_listTimeSpent4Kill_Ptr << ",";
                 ss << "\"m_arrSavedPositions_Ptr\":" << s.m_arrSavedPositions_Ptr << ",";
                 ss << "\"hurtInfos_Ptr\":" << s.hurtInfos_Ptr << ",";
                 ss << "\"enemySightLoss_Ptr\":" << s.enemySightLoss_Ptr << ",";
                 ss << "\"endedSightValue_Ptr\":" << s.endedSightValue_Ptr << ",";
                 ss << "\"ongoingSightValue_Ptr\":" << s.ongoingSightValue_Ptr << ",";
                 ss << "\"multiKillAssistIDs_Ptr\":" << s.multiKillAssistIDs_Ptr << ",";
                 ss << "\"m_LogicGuLianBulletManger_Ptr\":" << s.m_LogicGuLianBulletManger_Ptr << ",";
                 ss << "\"m_magicTranSpellSideEffect_Ptr\":" << s.m_magicTranSpellSideEffect_Ptr << ",";
                 ss << "\"m_magicTranSpellStageEffect_Ptr\":" << s.m_magicTranSpellStageEffect_Ptr << ",";
                 ss << "\"m_TwinPlayer_Ptr\":" << s.m_TwinPlayer_Ptr << ",";
                 ss << "\"m_summonTwinAI_Ptr\":" << s.m_summonTwinAI_Ptr << ",";
                 ss << "\"m_AFKTurnAIComponent_Ptr\":" << s.m_AFKTurnAIComponent_Ptr << ",";
                 ss << "\"m_uiAFKHoldCDRangeTimes_Ptr\":" << s.m_uiAFKHoldCDRangeTimes_Ptr << ",";
                 ss << "\"m_SynFightData_Ptr\":" << s.m_SynFightData_Ptr << ",";
                 ss << "\"dicIgnoreOpered_Ptr\":" << s.dicIgnoreOpered_Ptr << ",";
                 ss << "\"m_RelativeScore_Ptr\":" << s.m_RelativeScore_Ptr << ",";
                 ss << "\"dicTalentSkill_Ptr\":" << s.dicTalentSkill_Ptr << ",";
                 ss << "\"dicRuneSkill2023_Ptr\":" << s.dicRuneSkill2023_Ptr << ",";
                 ss << "\"lsMissions_Ptr\":" << s.lsMissions_Ptr << ",";
                 ss << "\"easterEggMissions_Ptr\":" << s.easterEggMissions_Ptr << ",";
                 ss << "\"m_lsEmoji_Ptr\":" << s.m_lsEmoji_Ptr << ",";
                 ss << "\"m_lsAutoEmoji_Ptr\":" << s.m_lsAutoEmoji_Ptr << ",";
                 ss << "\"m_lsAnima_Ptr\":" << s.m_lsAnima_Ptr << ",";
                 ss << "\"m_lsGraffiti_Ptr\":" << s.m_lsGraffiti_Ptr << ",";
                 ss << "\"m_PlayerData_Ptr\":" << s.m_PlayerData_Ptr << ",";
                 ss << "\"m_ConfigData_Ptr\":" << s.m_ConfigData_Ptr << ",";
                 ss << "\"m_HeroCostType_Ptr\":" << s.m_HeroCostType_Ptr << ",";
                 ss << "\"m_BattleConfig_Ptr\":" << s.m_BattleConfig_Ptr << ",";
                 ss << "\"m_TowerTurnData_Ptr\":" << s.m_TowerTurnData_Ptr << ",";
                 ss << "\"m_OperateTimeMonitor_Ptr\":" << s.m_OperateTimeMonitor_Ptr << ",";
                 ss << "\"m_CheckNearComponent_Ptr\":" << s.m_CheckNearComponent_Ptr << ",";
                 ss << "\"m_EstimateAttrComponent_Ptr\":" << s.m_EstimateAttrComponent_Ptr << ",";
                 ss << "\"m_StoreSkillComp_Ptr\":" << s.m_StoreSkillComp_Ptr << ",";
                 ss << "\"m_operCache_Ptr\":" << s.m_operCache_Ptr << ",";
                 ss << "\"m_HighLightComp_Ptr\":" << s.m_HighLightComp_Ptr << ",";
                 ss << "\"m_GankShoeRewardComp_Ptr\":" << s.m_GankShoeRewardComp_Ptr << ",";
                 ss << "\"m_ReqMoveDir_Ptr\":" << s.m_ReqMoveDir_Ptr << ",";
                 ss << "\"m_ReqMovePos_Ptr\":" << s.m_ReqMovePos_Ptr << ",";
                 ss << "\"listKillTime_Ptr\":" << s.listKillTime_Ptr << ",";
                 ss << "\"m_vDelayRemoveSkillIds_Ptr\":" << s.m_vDelayRemoveSkillIds_Ptr << ",";
                 ss << "\"m_HitHeroTimes_SkillGuid_Ptr\":" << s.m_HitHeroTimes_SkillGuid_Ptr << ",";
                 ss << "\"m_dStealValue_Ptr\":" << s.m_dStealValue_Ptr << ",";
                 ss << "\"m_AutoAttackAI_Ptr\":" << s.m_AutoAttackAI_Ptr << ",";
                 ss << "\"m_LogicPunish_Ptr\":" << s.m_LogicPunish_Ptr << ",";
                 ss << "\"m_Killer_Ptr\":" << s.m_Killer_Ptr << ",";
                 ss << "\"m_DevourData_Ptr\":" << s.m_DevourData_Ptr << ",";
                 ss << "\"m_ControlSummer_Ptr\":" << s.m_ControlSummer_Ptr << ",";
                 ss << "\"m_vSkillLogicFighter_Ptr\":" << s.m_vSkillLogicFighter_Ptr << ",";
                 ss << "\"m_vPlayerDeadInfo_Ptr\":" << s.m_vPlayerDeadInfo_Ptr << ",";
                 ss << "\"m_RecmendEquips_Ptr\":" << s.m_RecmendEquips_Ptr << ",";
                 ss << "\"m_v2StarDir_Ptr\":" << s.m_v2StarDir_Ptr << ",";
                 ss << "\"shopData_Ptr\":" << s.shopData_Ptr << ",";
                 ss << "\"v2LastCheckPos_Ptr\":" << s.v2LastCheckPos_Ptr << ",";
                 ss << "\"lastCheckMoveDir_Ptr\":" << s.lastCheckMoveDir_Ptr << ",";
                 ss << "\"lastFailedAutoAiSpellCast_Ptr\":" << s.lastFailedAutoAiSpellCast_Ptr << ",";
                 ss << "\"ownNormalSkillCache_Ptr\":" << s.ownNormalSkillCache_Ptr << ",";
                 ss << "\"lEatFruits_Ptr\":" << s.lEatFruits_Ptr << ",";

                 // Basic Info
                 ss << "\"playerName\":\"" << s.playerName << "\",";
                 ss << "\"heroId\":" << s.heroId << ",";

                 // Simple Values
                 ss << "\"totalGold\":" << s.totalGold << ",";
                 ss << "\"m_HurtTotalValue\":" << s.m_HurtTotalValue << ",";
                 ss << "\"m_HurtHeroValue\":" << s.m_HurtHeroValue << ",";
                 ss << "\"m_ATKHero\":" << s.m_ATKHero << ",";
                 ss << "\"m_iCommonAttackHeroCount\":" << s.m_iCommonAttackHeroCount << ",";
                 ss << "\"m_iNormalSkillHeroCount\":" << s.m_iNormalSkillHeroCount << ",";
                 ss << "\"m_HurtHeroReel\":" << s.m_HurtHeroReel << ",";
                 ss << "\"m_HurtHeroAD\":" << s.m_HurtHeroAD << ",";
                 ss << "\"m_HurtHeroAP\":" << s.m_HurtHeroAP << ",";
                 ss << "\"m_HurtHeroByEquip\":" << s.m_HurtHeroByEquip << ",";
                 ss << "\"m_HurtHeroByEmblem\":" << s.m_HurtHeroByEmblem << ",";
                 ss << "\"m_HurtTowerValue\":" << s.m_HurtTowerValue << ",";
                 ss << "\"m_HurtSoliderValue\":" << s.m_HurtSoliderValue << ",";
                 ss << "\"m_iInjuredShield\":" << s.m_iInjuredShield << ",";
                 ss << "\"m_InjuredValue\":" << s.m_InjuredValue << ",";
                 ss << "\"m_InjuredTower\":" << s.m_InjuredTower << ",";
                 ss << "\"m_InjuredTotal\":" << s.m_InjuredTotal << ",";
                 ss << "\"m_InjuredSoldier\":" << s.m_InjuredSoldier << ",";
                 ss << "\"m_InjuredAD\":" << s.m_InjuredAD << ",";
                 ss << "\"m_InjuredAP\":" << s.m_InjuredAP << ",";
                 ss << "\"m_InjuredReal\":" << s.m_InjuredReal << ",";
                 ss << "\"m_RealInjuredVal\":" << s.m_RealInjuredVal << ",";
                 ss << "\"m_iBeCuredValue\":" << s.m_iBeCuredValue << ",";
                 ss << "\"m_CureHero\":" << s.m_CureHero << ",";
                 ss << "\"m_CureTeammate\":" << s.m_CureTeammate << ",";
                 ss << "\"m_CureSelf\":" << s.m_CureSelf << ",";
                 ss << "\"m_CureHeroJustSkill\":" << s.m_CureHeroJustSkill << ",";
                 ss << "\"m_iSkillUseCount\":" << s.m_iSkillUseCount << ",";
                 ss << "\"m_iCommonAtkUseCount\":" << s.m_iCommonAtkUseCount << ",";
                 ss << "\"m_iCommonAtkUseCount_AllSkillCD\":" << s.m_iCommonAtkUseCount_AllSkillCD << ",";
                 ss << "\"m_iNormalSkillUseCount\":" << s.m_iNormalSkillUseCount << ",";
                 ss << "\"m_iNormalSkillHasDraggedCount\":" << s.m_iNormalSkillHasDraggedCount << ",";
                 ss << "\"m_iFirstSkillUseCount\":" << s.m_iFirstSkillUseCount << ",";
                 ss << "\"m_iSecondSkillUseCount\":" << s.m_iSecondSkillUseCount << ",";
                 ss << "\"m_iThirdSkillUseCount\":" << s.m_iThirdSkillUseCount << ",";
                 ss << "\"m_iFourthSkillUseCount\":" << s.m_iFourthSkillUseCount << ",";
                 ss << "\"m_iEquipSkillUseCount\":" << s.m_iEquipSkillUseCount << ",";
                 ss << "\"m_iCureSkillUseCount\":" << s.m_iCureSkillUseCount << ",";
                 ss << "\"m_iBackHomeSkillUseCount\":" << s.m_iBackHomeSkillUseCount << ",";
                 ss << "\"m_iSummonSkillUseCount\":" << s.m_iSummonSkillUseCount << ",";
                 ss << "\"m_iHuntSkillUseCount\":" << s.m_iHuntSkillUseCount << ",";
                 ss << "\"m_iGankSkillUseCount\":" << s.m_iGankSkillUseCount << ",";
                 ss << "\"m_iKillMageCount\":" << s.m_iKillMageCount << ",";
                 ss << "\"m_iKillMarksmanCount\":" << s.m_iKillMarksmanCount << ",";
                 ss << "\"m_iEnterHeroBattleFromGrass\":" << s.m_iEnterHeroBattleFromGrass << ",";
                 ss << "\"m_iEnterGrassTimes\":" << s.m_iEnterGrassTimes << ",";
                 ss << "\"KillTowerTimes\":" << s.KillTowerTimes << ",";
                 ss << "\"KillSoldierTimes\":" << s.KillSoldierTimes << ",";
                 ss << "\"m_nSavedPositionsStart\":" << s.m_nSavedPositionsStart << ",";
                 ss << "\"m_nSavedPositionsCount\":" << s.m_nSavedPositionsCount << ",";
                 ss << "\"m_uLossOfSightTime\":" << s.m_uLossOfSightTime << ",";
                 ss << "\"sightIdGenerator\":" << s.sightIdGenerator << ",";
                 ss << "\"continueKill\":" << s.continueKill << ",";
                 ss << "\"multiKill\":" << s.multiKill << ",";
                 ss << "\"DoubleKillTimes\":" << s.DoubleKillTimes << ",";
                 ss << "\"TripleKillTimes\":" << s.TripleKillTimes << ",";
                 ss << "\"QuadraKillTimes\":" << s.QuadraKillTimes << ",";
                 ss << "\"PentaKillTimes\":" << s.PentaKillTimes << ",";
                 ss << "\"greenLightCanUse\":" << s.greenLightCanUse << ",";
                 ss << "\"greenLightStartTime\":" << s.greenLightStartTime << ",";
                 ss << "\"greenLightTimeSpan\":" << s.greenLightTimeSpan << ",";
                 ss << "\"greenLightIgnoreCountDown\":" << s.greenLightIgnoreCountDown << ",";
                 ss << "\"bMonitoringSoloBreakLane\":" << s.bMonitoringSoloBreakLane << ",";
                 ss << "\"uMonitoringTowerGuid\":" << s.uMonitoringTowerGuid << ",";
                 ss << "\"uMonitoringTimeout\":" << s.uMonitoringTimeout << ",";
                 ss << "\"lastReceiveMoveOptTime\":" << s.lastReceiveMoveOptTime << ",";
                 ss << "\"moveProtectTime\":" << s.moveProtectTime << ",";
                 ss << "\"m_bMoveProtectAIState\":" << s.m_bMoveProtectAIState << ",";
                 ss << "\"uCheckStarLightTaskTimer\":" << s.uCheckStarLightTaskTimer << ",";
                 ss << "\"uLastGuideSoldier2Tower\":" << s.uLastGuideSoldier2Tower << ",";
                 ss << "\"m_iGuideSoldier2Tower\":" << s.m_iGuideSoldier2Tower << ",";
                 ss << "\"m_bIsTwinMain\":" << s.m_bIsTwinMain << ",";
                 ss << "\"m_bIsTwinControl\":" << s.m_bIsTwinControl << ",";
                 ss << "\"bMLAIState\":" << s.bMLAIState << ",";
                 ss << "\"bShowConnectMsg\":" << s.bShowConnectMsg << ",";
                 ss << "\"m_IsRobotPlayer\":" << s.m_IsRobotPlayer << ",";
                 ss << "\"m_uiWaitTrunAITime\":" << s.m_uiWaitTrunAITime << ",";
                 ss << "\"uiQuicklyTrunToAITime\":" << s.uiQuicklyTrunToAITime << ",";
                 ss << "\"uiNomalTurnAITime\":" << s.uiNomalTurnAITime << ",";
                 ss << "\"uIgnoreTurnAITime\":" << s.uIgnoreTurnAITime << ",";
                 ss << "\"iIgnoreOpered\":" << s.iIgnoreOpered << ",";
                 ss << "\"m_bForceAi\":" << s.m_bForceAi << ",";
                 ss << "\"m_bWeakNetWork\":" << s.m_bWeakNetWork << ",";
                 ss << "\"m_uLastTimePlayerOpered\":" << s.m_uLastTimePlayerOpered << ",";
                 ss << "\"bWaitTurnAI\":" << s.bWaitTurnAI << ",";
                 ss << "\"uplandRangeDistance\":" << s.uplandRangeDistance << ",";
                 ss << "\"m_bConnected\":" << s.m_bConnected << ",";
                 ss << "\"m_uiVoiceParam\":" << s.m_uiVoiceParam << ",";
                 ss << "\"m_iHolyStatueSkillID\":" << s.m_iHolyStatueSkillID << ",";
                 ss << "\"m_uHolyStatueID\":" << s.m_uHolyStatueID << ",";
                 ss << "\"m_uHolyStatueIDIfUsed\":" << s.m_uHolyStatueIDIfUsed << ",";
                 ss << "\"m_TotalExp\":" << s.m_TotalExp << ",";
                 ss << "\"m_bGankEquip\":" << s.m_bGankEquip << ",";
                 ss << "\"m_bHuntSkill\":" << s.m_bHuntSkill << ",";
                 ss << "\"m_bLowestMoneyOrExp\":" << s.m_bLowestMoneyOrExp << ",";
                 ss << "\"m_ShareMoneyEx\":" << s.m_ShareMoneyEx << ",";
                 ss << "\"m_ShareExpEx\":" << s.m_ShareExpEx << ",";
                 ss << "\"m_RewardMoney\":" << s.m_RewardMoney << ",";
                 ss << "\"m_iBaseMoney\":" << s.m_iBaseMoney << ",";
                 ss << "\"m_KillBounty\":" << s.m_KillBounty << ",";
                 ss << "\"m_bBountyOverThreshold\":" << s.m_bBountyOverThreshold << ",";
                 ss << "\"m_uLastBountyOverThreshold\":" << s.m_uLastBountyOverThreshold << ",";
                 ss << "\"m_iContinueDeadSub\":" << s.m_iContinueDeadSub << ",";
                 ss << "\"m_iContinueKillNum\":" << s.m_iContinueKillNum << ",";
                 ss << "\"m_iContinueKillAdd\":" << s.m_iContinueKillAdd << ",";
                 ss << "\"m_RewardExp\":" << s.m_RewardExp << ",";
                 ss << "\"m_iBaseExp\":" << s.m_iBaseExp << ",";
                 ss << "\"m_iLevelExp\":" << s.m_iLevelExp << ",";
                 ss << "\"m_iLvExpRate\":" << s.m_iLvExpRate << ",";
                 ss << "\"m_fContinueDeadPara\":" << s.m_fContinueDeadPara << ",";
                 ss << "\"DeadAndKillTimes\":" << s.DeadAndKillTimes << ",";
                 ss << "\"m_AssistTimesReward\":" << s.m_AssistTimesReward << ",";
                 ss << "\"m_bReqMoveUpdate\":" << s.m_bReqMoveUpdate << ",";
                 ss << "\"bDeathHoldKillCount\":" << s.bDeathHoldKillCount << ",";
                 ss << "\"mShutDown\":" << s.mShutDown << ",";
                 ss << "\"lastKillTime\":" << s.lastKillTime << ",";
                 ss << "\"mutiKillUsefulTime\":" << s.mutiKillUsefulTime << ",";
                 ss << "\"mutiKillUsefulTimeOn5kill\":" << s.mutiKillUsefulTimeOn5kill << ",";
                 ss << "\"m_uiLastMoveTime\":" << s.m_uiLastMoveTime << ",";
                 ss << "\"m_GetGoldTimesBySoldier\":" << s.m_GetGoldTimesBySoldier << ",";
                 ss << "\"m_BeyondGodlike\":" << s.m_BeyondGodlike << ",";
                 ss << "\"m_MaxMutiKill\":" << s.m_MaxMutiKill << ",";
                 ss << "\"m_MaxContinueKill\":" << s.m_MaxContinueKill << ",";
                 ss << "\"m_singleKill\":" << s.m_singleKill << ",";
                 ss << "\"m_KillLingZhu\":" << s.m_KillLingZhu << ",";
                 ss << "\"m_AssistLingZhu\":" << s.m_AssistLingZhu << ",";
                 ss << "\"KillWildTimes\":" << s.KillWildTimes << ",";
                 ss << "\"m_WeekKill\":" << s.m_WeekKill << ",";
                 ss << "\"m_KillShenGui\":" << s.m_KillShenGui << ",";
                 ss << "\"m_AssistShenGui\":" << s.m_AssistShenGui << ",";
                 ss << "\"m_KillCdMonster\":" << s.m_KillCdMonster << ",";
                 ss << "\"m_KillAtkMonster\":" << s.m_KillAtkMonster << ",";
                 ss << "\"m_KillMePlayerCount\":" << s.m_KillMePlayerCount << ",";
                 ss << "\"m_CurZoneId\":" << s.m_CurZoneId << ",";
                 ss << "\"m_HurtTurtle\":" << s.m_HurtTurtle << ",";
                 ss << "\"m_HurtLord\":" << s.m_HurtLord << ",";
                 ss << "\"m_ShieldCureHero\":" << s.m_ShieldCureHero << ",";
                 ss << "\"m_ShieldCureSelf\":" << s.m_ShieldCureSelf << ",";
                 ss << "\"m_ShieldTeammate\":" << s.m_ShieldTeammate << ",";
                 ss << "\"m_SufferControlTime\":" << s.m_SufferControlTime << ",";
                 ss << "\"m_SufferSlowTime\":" << s.m_SufferSlowTime << ",";
                 ss << "\"m_ControlTime\":" << s.m_ControlTime << ",";
                 ss << "\"m_KillsWithRedAndBlueBuff\":" << s.m_KillsWithRedAndBlueBuff << ",";
                 ss << "\"m_MoveDis\":" << s.m_MoveDis << ",";
                 ss << "\"m_MoveDisTickCount\":" << s.m_MoveDisTickCount << ",";
                 ss << "\"m_MoveCountPrePosX\":" << s.m_MoveCountPrePosX << ",";
                 ss << "\"m_MoveCountPrePosY\":" << s.m_MoveCountPrePosY << ",";
                 ss << "\"m_GoldByWild\":" << s.m_GoldByWild << ",";
                 ss << "\"m_GoldBySoldier\":" << s.m_GoldBySoldier << ",";
                 ss << "\"m_GoldByHero\":" << s.m_GoldByHero << ",";
                 ss << "\"iAllHurtVal\":" << s.iAllHurtVal << ",";
                 ss << "\"m_CrlTimes\":" << s.m_CrlTimes << ",";
                 ss << "\"m_iPoisonValue\":" << s.m_iPoisonValue << ",";
                 ss << "\"m_hurtEnemyWild\":" << s.m_hurtEnemyWild << ",";
                 ss << "\"m_hurtWildValue\":" << s.m_hurtWildValue << ",";
                 ss << "\"m_TrunSpeed\":" << s.m_TrunSpeed << ",";
                 ss << "\"m_GreatGuid\":" << s.m_GreatGuid << ",";
                 ss << "\"m_bRefuseSelectAIType\":" << s.m_bRefuseSelectAIType << ",";
                 ss << "\"m_uiLastOperFrameTime\":" << s.m_uiLastOperFrameTime << ",";
                 ss << "\"SummonSkillId\":" << s.SummonSkillId << ",";
                 ss << "\"m_SummonStartSkillId\":" << s.m_SummonStartSkillId << ",";
                 ss << "\"m_RankLv\":" << s.m_RankLv << ",";
                 ss << "\"m_bigRankLv\":" << s.m_bigRankLv << ",";
                 ss << "\"m_rankStar\":" << s.m_rankStar << ",";
                 ss << "\"m_rankNum\":" << s.m_rankNum << ",";
                 ss << "\"m_lastReliveTime\":" << s.m_lastReliveTime << ",";
                 ss << "\"m_ReviveTimeMs\":" << s.m_ReviveTimeMs << ",";
                 ss << "\"m_bFastDie\":" << s.m_bFastDie << ",";
                 ss << "\"m_EatFruit\":" << s.m_EatFruit << ",";
                 ss << "\"m_KillByFruit\":" << s.m_KillByFruit << ",";
                 ss << "\"m_GetFruitOnMin\":" << s.m_GetFruitOnMin << ",";
                 ss << "\"bAllowRelive\":" << s.bAllowRelive << ",";
                 ss << "\"m_uiRoleLevel\":" << s.m_uiRoleLevel << ",";
                 ss << "\"m_iAddGoldValue\":" << s.m_iAddGoldValue << ",";
                 ss << "\"iMaxHurtValue\":" << s.iMaxHurtValue << ",";
                 ss << "\"m_iSkinId\":" << s.m_iSkinId << ",";
                 ss << "\"m_iDragonCrystalId\":" << s.m_iDragonCrystalId << ",";
                 ss << "\"m_uUserMapID\":" << s.m_uUserMapID << ",";
                 ss << "\"iLastGiveupEquip\":" << s.iLastGiveupEquip << ",";
                 ss << "\"m_iSurvivalTime\":" << s.m_iSurvivalTime << ",";
                 ss << "\"m_iChickenRanking\":" << s.m_iChickenRanking << ",";
                 ss << "\"m_bEmojiBirthday\":" << s.m_bEmojiBirthday << ",";
                 ss << "\"logAttackSpeed\":" << s.logAttackSpeed << ",";
                 ss << "\"doAttackSpeed\":" << s.doAttackSpeed << ",";
                 ss << "\"m_CommATK_RunTimer\":" << s.m_CommATK_RunTimer << ",";
                 ss << "\"m_dCommATKSingTime_Mod\":" << s.m_dCommATKSingTime_Mod << ",";
                 ss << "\"m_CommATKSingTime_LastTimer\":" << s.m_CommATKSingTime_LastTimer << ",";
                 ss << "\"m_dCommATKCD_Mod\":" << s.m_dCommATKCD_Mod << ",";
                 ss << "\"m_CommATKCD_LastTimer\":" << s.m_CommATKCD_LastTimer << ",";
                 ss << "\"m_PriorEquip\":" << s.m_PriorEquip << ",";
                 ss << "\"m_uHeroEnhanceLevel\":" << s.m_uHeroEnhanceLevel << ",";
                 ss << "\"m_bGhostHasDied\":" << s.m_bGhostHasDied << ",";
                 ss << "\"lastCheckDirSymbol\":" << s.lastCheckDirSymbol << ",";
                 ss << "\"right\":" << s.right << ",";
                 ss << "\"lastFailedAutoAiSpellCastTime\":" << s.lastFailedAutoAiSpellCastTime << ",";
                 ss << "\"autoTime\":" << s.autoTime << ",";
                 ss << "\"m_dXpGrowthDecimal\":" << s.m_dXpGrowthDecimal << ",";
                 ss << "\"bBornedBoss\":" << s.bBornedBoss << ",";
                 ss << "\"iPreMutiKillValue\":" << s.iPreMutiKillValue << ",";
                 ss << "\"iPreContinueKillValue\":" << s.iPreContinueKillValue << ",";
                 ss << "\"iPreKillLingZhu\":" << s.iPreKillLingZhu << ",";
                 ss << "\"iPreKillShenGui\":" << s.iPreKillShenGui << ",";
                 ss << "\"iPreShutDown\":" << s.iPreShutDown << ",";
                 ss << "\"bCheckFirstBlood\":" << s.bCheckFirstBlood << ",";
                 ss << "\"iCurrentResult\":" << s.iCurrentResult << ",";
                 ss << "\"iPreGetResultTime\":" << s.iPreGetResultTime << ",";
                 ss << "\"iCurKilledResult\":" << s.iCurKilledResult << ",";
                 ss << "\"iPreKilledResultTime\":" << s.iPreKilledResultTime;
                 ss << "}";
                 if (i < g_State.logicPlayers.size() - 1) ss << ",";
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

// Forward declaration
void UpdateBattleStats(void* logicBattleManager);

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
