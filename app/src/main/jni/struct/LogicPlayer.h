#pragma once
#include <cstdint>

// Offsets for Battle.LogicPlayer based on user provided dump
// Class: Battle.LogicPlayer
// Base: Battle.LogicFighter

#define LogicPlayer_m_ID 0xac // From EntityBase (Inherited)

#define LogicPlayer_m_LoigcBezierBullet 0x7e8 // Dictionary<int, LogicBulletBase>
#define LogicPlayer_moveControllers 0x7f0 // HashSet<uint>
#define LogicPlayer_totalGold 0x7f8 // int32
#define LogicPlayer_m_copyHurtCount 0x800 // Dictionary<uint, CopyHurtInfo>
#define LogicPlayer_m_HurtTotalValue 0x808 // double
#define LogicPlayer_m_HurtHeroValue 0x810 // double
#define LogicPlayer_m_ATKHero 0x818 // double
#define LogicPlayer_m_iCommonAttackHeroCount 0x820 // int32
#define LogicPlayer_m_iNormalSkillHeroCount 0x824 // int32
#define LogicPlayer_m_HurtHeroReel 0x828 // double
#define LogicPlayer_m_HurtHeroAD 0x830 // double
#define LogicPlayer_m_HurtHeroAP 0x838 // double
#define LogicPlayer_m_HurtHeroByEquip 0x840 // double
#define LogicPlayer_m_HurtHeroByEmblem 0x848 // double
#define LogicPlayer_m_HurtTowerValue 0x850 // double
#define LogicPlayer_m_HurtSoliderValue 0x858 // double
#define LogicPlayer_m_iInjuredShield 0x860 // int32
#define LogicPlayer_m_InjuredValue 0x868 // double
#define LogicPlayer_m_InjuredTower 0x870 // double
#define LogicPlayer_m_InjuredTotal 0x878 // int32
#define LogicPlayer_m_InjuredSoldier 0x880 // double
#define LogicPlayer_m_InjuredAD 0x888 // double
#define LogicPlayer_m_InjuredAP 0x890 // double
#define LogicPlayer_m_InjuredReal 0x898 // double
#define LogicPlayer_m_RealInjuredVal 0x8a0 // double
#define LogicPlayer_m_iBeCuredValue 0x8a8 // int32
#define LogicPlayer_m_CureHero 0x8b0 // double
#define LogicPlayer_m_CureTeammate 0x8b8 // double
#define LogicPlayer_m_CureSelf 0x8c0 // double
#define LogicPlayer_m_CureHeroJustSkill 0x8c8 // double
#define LogicPlayer_m_iSkillUseCount 0x8d0 // int32
#define LogicPlayer_m_iCommonAtkUseCount 0x8d4 // int32
#define LogicPlayer_m_iCommonAtkUseCount_AllSkillCD 0x8d8 // int32
#define LogicPlayer_m_iNormalSkillUseCount 0x8dc // int32
#define LogicPlayer_m_iNormalSkillHasDraggedCount 0x8e0 // int32
#define LogicPlayer_m_iFirstSkillUseCount 0x8e4 // int32
#define LogicPlayer_m_iSecondSkillUseCount 0x8e8 // int32
#define LogicPlayer_m_iThirdSkillUseCount 0x8ec // int32
#define LogicPlayer_m_iFourthSkillUseCount 0x8f0 // int32
#define LogicPlayer_m_iEquipSkillUseCount 0x8f4 // int32
#define LogicPlayer_m_iCureSkillUseCount 0x8f8 // int32
#define LogicPlayer_m_iBackHomeSkillUseCount 0x8fc // int32
#define LogicPlayer_m_iSummonSkillUseCount 0x900 // int32
#define LogicPlayer_m_iHuntSkillUseCount 0x904 // int32
#define LogicPlayer_m_iGankSkillUseCount 0x908 // int32
#define LogicPlayer_m_iKillMageCount 0x90c // int32
#define LogicPlayer_m_iKillMarksmanCount 0x910 // int32
#define LogicPlayer_m_iEnterHeroBattleFromGrass 0x914 // int32
#define LogicPlayer_m_iEnterGrassTimes 0x918 // int32
#define LogicPlayer_m_dictFirstHitHeroTime 0x920 // Dictionary<uint, uint>
#define LogicPlayer_m_listTimeSpent4Kill 0x928 // List<int>
#define LogicPlayer_KillTowerTimes 0x930 // int32
#define LogicPlayer_KillSoldierTimes 0x934 // int32
#define LogicPlayer_m_arrSavedPositions 0x938 // DVector2[]
#define LogicPlayer_m_nSavedPositionsStart 0x940 // int32
#define LogicPlayer_m_nSavedPositionsCount 0x944 // int32
#define LogicPlayer_hurtInfos 0x948 // List<BeAtkData>
#define LogicPlayer_m_uLossOfSightTime 0x950 // uint32
#define LogicPlayer_enemySightLoss 0x958 // Dictionary<uint, uint>
#define LogicPlayer_endedSightValue 0x960 // List<SightValueDetail>
#define LogicPlayer_ongoingSightValue 0x968 // List<SightValueDetail>
#define LogicPlayer_sightIdGenerator 0x970 // int32
// #define LogicPlayer_lossTimeDuration 0x974 // Nullable<float> (Size might be > 4 bytes)
#define LogicPlayer_continueKill 0x97c // int32
#define LogicPlayer_multiKill 0x980 // int32
#define LogicPlayer_DoubleKillTimes 0x984 // int32
#define LogicPlayer_TripleKillTimes 0x988 // int32
#define LogicPlayer_QuadraKillTimes 0x98c // int32
#define LogicPlayer_PentaKillTimes 0x990 // int32
#define LogicPlayer_multiKillAssistIDs 0x998 // List<uint>
// #define LogicPlayer_greenLightSkillID 0x9a0 // Nullable<int>
#define LogicPlayer_greenLightCanUse 0x9a8 // bool
#define LogicPlayer_greenLightStartTime 0x9ac // uint32
#define LogicPlayer_greenLightTimeSpan 0x9b0 // uint32
#define LogicPlayer_greenLightIgnoreCountDown 0x9b4 // bool
#define LogicPlayer_m_LogicGuLianBulletManger 0x9b8 // LogicGuLianBulletManger
#define LogicPlayer_bMonitoringSoloBreakLane 0x9c0 // bool
#define LogicPlayer_uMonitoringTowerGuid 0x9c4 // uint32
#define LogicPlayer_uMonitoringTimeout 0x9c8 // uint32
#define LogicPlayer_m_magicTranSpellSideEffect 0x9d0 // Dictionary<int, LogicEffect>
#define LogicPlayer_m_magicTranSpellStageEffect 0x9d8 // Dictionary<int, LogicEffect>
#define LogicPlayer_lastReceiveMoveOptTime 0x9e0 // uint32
#define LogicPlayer_moveProtectTime 0x9e4 // int32
#define LogicPlayer_m_bMoveProtectAIState 0x9e8 // bool
#define LogicPlayer_uCheckStarLightTaskTimer 0x9ec // uint32
#define LogicPlayer_uLastGuideSoldier2Tower 0x9f0 // uint32
#define LogicPlayer_m_iGuideSoldier2Tower 0x9f4 // int32
#define LogicPlayer_m_TwinPlayer 0x9f8 // LogicPlayer
#define LogicPlayer_m_bIsTwinMain 0xa00 // bool
#define LogicPlayer_m_bIsTwinControl 0xa01 // bool
#define LogicPlayer_m_summonTwinAI 0xa08 // AIBase
#define LogicPlayer_m_AFKTurnAIComponent 0xa10 // AFKTurnAIComponent
#define LogicPlayer_bMLAIState 0xa18 // bool
#define LogicPlayer_bShowConnectMsg 0xa19 // bool
#define LogicPlayer_m_IsRobotPlayer 0xa1a // bool
#define LogicPlayer_m_uiWaitTrunAITime 0xa1c // uint32
#define LogicPlayer_uiQuicklyTrunToAITime 0xa20 // uint32 (readonly)
#define LogicPlayer_uiNomalTurnAITime 0xa24 // uint32 (internal readonly)
#define LogicPlayer_uIgnoreTurnAITime 0xa28 // uint32
#define LogicPlayer_iIgnoreOpered 0xa2c // int32
#define LogicPlayer_m_bForceAi 0xa30 // bool
#define LogicPlayer_m_bWeakNetWork 0xa31 // bool
#define LogicPlayer_m_uiAFKHoldCDRangeTimes 0xa38 // List<ValueTuple<uint, uint, uint>>
#define LogicPlayer_m_uLastTimePlayerOpered 0xa40 // uint32
#define LogicPlayer_bWaitTurnAI 0xa44 // bool
// #define LogicPlayer_g_aoiCastData 0x0 // static
#define LogicPlayer_m_SynFightData 0xa48 // FightPlayerData
#define LogicPlayer_uplandRangeDistance 0xa50 // int32 (readonly)
#define LogicPlayer_dicIgnoreOpered 0xa58 // Dictionary
#define LogicPlayer_m_bConnected 0xa60 // bool
#define LogicPlayer_m_uiVoiceParam 0xa64 // uint32
#define LogicPlayer_m_RelativeScore 0xa68 // Dictionary<uint, int>
#define LogicPlayer_dicTalentSkill 0xa70 // Dictionary<uint, uint>
#define LogicPlayer_dicRuneSkill2023 0xa78 // Dictionary<uint, uint>
#define LogicPlayer_lsMissions 0xa80 // List<uint>
#define LogicPlayer_easterEggMissions 0xa88 // List<uint>
#define LogicPlayer_m_lsEmoji 0xa90 // List<uint>
#define LogicPlayer_m_lsAutoEmoji 0xa98 // BattleAutoEmojiInfos
#define LogicPlayer_m_lsAnima 0xaa0 // List<uint>
#define LogicPlayer_m_lsGraffiti 0xaa8 // List<uint>
#define LogicPlayer_m_iHolyStatueSkillID 0xab0 // int32
#define LogicPlayer_m_uHolyStatueID 0xab4 // uint32
#define LogicPlayer_m_uHolyStatueIDIfUsed 0xab8 // uint32
#define LogicPlayer_m_TotalExp 0xabc // int32
#define LogicPlayer_m_PlayerData 0xac0 // PlayerData
#define LogicPlayer_m_ConfigData 0xac8 // CData_Hero_Element
#define LogicPlayer_m_HeroCostType 0xad0 // HeroCostType
#define LogicPlayer_m_BattleConfig 0xad8 // BattleConfig
#define LogicPlayer_m_TowerTurnData 0xae0 // LogicTurnFighterData
#define LogicPlayer_m_OperateTimeMonitor 0xae8 // OperateTimeMonitor
#define LogicPlayer_m_CheckNearComponent 0xaf0 // CheckNearDirOptimizeComponent
#define LogicPlayer_m_EstimateAttrComponent 0xaf8 // LogicEstimateAttrComponent
#define LogicPlayer_m_StoreSkillComp 0xb00 // LogicStoreSkillComp
#define LogicPlayer_m_operCache 0xb08 // LogicOperCacheComp
#define LogicPlayer_m_HighLightComp 0xb10 // HighLight_Comp
#define LogicPlayer_m_GankShoeRewardComp 0xb18 // LogicGankShoeRewardComp
#define LogicPlayer_m_bGankEquip 0xb20 // bool
#define LogicPlayer_m_bHuntSkill 0xb21 // bool
#define LogicPlayer_m_bLowestMoneyOrExp 0xb24 // int32
#define LogicPlayer_m_ShareMoneyEx 0xb28 // double
#define LogicPlayer_m_ShareExpEx 0xb30 // double
#define LogicPlayer_m_RewardMoney 0xb38 // int32
#define LogicPlayer_m_iBaseMoney 0xb3c // int32
#define LogicPlayer_m_KillBounty 0xb40 // int32
#define LogicPlayer_m_bBountyOverThreshold 0xb44 // bool
#define LogicPlayer_m_uLastBountyOverThreshold 0xb48 // uint32
#define LogicPlayer_m_iContinueDeadSub 0xb4c // int32
#define LogicPlayer_m_iContinueKillNum 0xb50 // int32
#define LogicPlayer_m_iContinueKillAdd 0xb54 // int32
#define LogicPlayer_m_RewardExp 0xb58 // int32
#define LogicPlayer_m_iBaseExp 0xb5c // int32
#define LogicPlayer_m_iLevelExp 0xb60 // int32
#define LogicPlayer_m_iLvExpRate 0xb68 // double
#define LogicPlayer_m_fContinueDeadPara 0xb70 // double
#define LogicPlayer_DeadAndKillTimes 0xb78 // int32
#define LogicPlayer_m_AssistTimesReward 0xb7c // int32
#define LogicPlayer_m_bReqMoveUpdate 0xb80 // bool
#define LogicPlayer_m_ReqMoveDir 0xb88 // DVector2 (struct, check size, usually 16 bytes for 2 doubles)
#define LogicPlayer_m_ReqMovePos 0xb98 // DVector2
#define LogicPlayer_bDeathHoldKillCount 0xba8 // bool
#define LogicPlayer_mShutDown 0xbac // int32
#define LogicPlayer_lastKillTime 0xbb0 // uint32
#define LogicPlayer_mutiKillUsefulTime 0xbb4 // uint32
#define LogicPlayer_mutiKillUsefulTimeOn5kill 0xbb8 // uint32
#define LogicPlayer_listKillTime 0xbc0 // List<string>
#define LogicPlayer_m_vDelayRemoveSkillIds 0xbc8 // Dictionary<int, uint>
#define LogicPlayer_m_uiLastMoveTime 0xbd0 // uint32
#define LogicPlayer_m_GetGoldTimesBySoldier 0xbd4 // int32
#define LogicPlayer_m_BeyondGodlike 0xbd8 // int32
#define LogicPlayer_m_MaxMutiKill 0xbdc // int32
#define LogicPlayer_m_MaxContinueKill 0xbe0 // int32
#define LogicPlayer_m_singleKill 0xbe4 // int32
#define LogicPlayer_m_KillLingZhu 0xbe8 // int32
#define LogicPlayer_m_AssistLingZhu 0xbec // int32
#define LogicPlayer_KillWildTimes 0xbf0 // int32
#define LogicPlayer_m_HitHeroTimes_SkillGuid 0xbf8 // HashSet<uint>
#define LogicPlayer_m_WeekKill 0xc00 // int32
#define LogicPlayer_m_KillShenGui 0xc04 // int32
#define LogicPlayer_m_AssistShenGui 0xc08 // int32
#define LogicPlayer_m_KillCdMonster 0xc0c // int32
#define LogicPlayer_m_KillAtkMonster 0xc10 // int32
#define LogicPlayer_m_KillMePlayerCount 0xc14 // int32
#define LogicPlayer_m_CurZoneId 0xc18 // int32
#define LogicPlayer_m_HurtTurtle 0xc20 // double
#define LogicPlayer_m_HurtLord 0xc28 // double
#define LogicPlayer_m_ShieldCureHero 0xc30 // double
#define LogicPlayer_m_ShieldCureSelf 0xc38 // double
#define LogicPlayer_m_ShieldTeammate 0xc40 // double
#define LogicPlayer_m_SufferControlTime 0xc48 // int32
#define LogicPlayer_m_SufferSlowTime 0xc4c // int32
#define LogicPlayer_m_ControlTime 0xc50 // int32
#define LogicPlayer_m_KillsWithRedAndBlueBuff 0xc54 // int32
#define LogicPlayer_m_MoveDis 0xc58 // double
#define LogicPlayer_m_MoveDisTickCount 0xc60 // double
#define LogicPlayer_m_MoveCountPrePosX 0xc68 // double
#define LogicPlayer_m_MoveCountPrePosY 0xc70 // double
#define LogicPlayer_m_GoldByWild 0xc78 // int32
#define LogicPlayer_m_GoldBySoldier 0xc7c // int32
#define LogicPlayer_m_GoldByHero 0xc80 // int32
#define LogicPlayer_iAllHurtVal 0xc84 // int32
#define LogicPlayer_m_CrlTimes 0xc88 // int32
#define LogicPlayer_m_iPoisonValue 0xc8c // int32
#define LogicPlayer_m_hurtEnemyWild 0xc90 // double
#define LogicPlayer_m_hurtWildValue 0xc98 // double
#define LogicPlayer_m_dStealValue 0xca0 // Dictionary<int, Dictionary<int, int>>
#define LogicPlayer_m_TrunSpeed 0xca8 // double
#define LogicPlayer_m_GreatGuid 0xcb0 // uint32
#define LogicPlayer_m_bRefuseSelectAIType 0xcb4 // bool
#define LogicPlayer_m_uiLastOperFrameTime 0xcb8 // uint32
#define LogicPlayer_SummonSkillId 0xcbc // int32
#define LogicPlayer_m_SummonStartSkillId 0xcc0 // int32
#define LogicPlayer_m_RankLv 0xcc4 // uint32
#define LogicPlayer_m_bigRankLv 0xcc8 // uint32
#define LogicPlayer_m_rankStar 0xccc // uint32
#define LogicPlayer_m_rankNum 0xcd0 // uint32
#define LogicPlayer_m_AutoAttackAI 0xcd8 // AutoAttackAI
#define LogicPlayer_m_LogicPunish 0xce0 // LogicPunish
#define LogicPlayer_m_lastReliveTime 0xce8 // uint32
#define LogicPlayer_m_ReviveTimeMs 0xcec // uint32
#define LogicPlayer_m_bFastDie 0xcf0 // bool
#define LogicPlayer_m_EatFruit 0xcf4 // uint32
#define LogicPlayer_m_KillByFruit 0xcf8 // uint32
#define LogicPlayer_m_GetFruitOnMin 0xcfc // uint32
#define LogicPlayer_bAllowRelive 0xd00 // bool
#define LogicPlayer_m_uiRoleLevel 0xd04 // uint32
#define LogicPlayer_m_Killer 0xd08 // LogicFighter
#define LogicPlayer_m_iAddGoldValue 0xd10 // int32
#define LogicPlayer_iMaxHurtValue 0xd14 // int32
#define LogicPlayer_m_iSkinId 0xd18 // int32
#define LogicPlayer_m_iDragonCrystalId 0xd1c // int32
#define LogicPlayer_m_uUserMapID 0xd20 // uint32
#define LogicPlayer_m_DevourData 0xd28 // DevourData
#define LogicPlayer_iLastGiveupEquip 0xd30 // int32
#define LogicPlayer_m_ControlSummer 0xd38 // LogicControlSummons
#define LogicPlayer_m_vSkillLogicFighter 0xd40 // Dictionary<int, LogicFighter>
#define LogicPlayer_m_iSurvivalTime 0xd48 // uint32
#define LogicPlayer_m_iChickenRanking 0xd4c // uint32
#define LogicPlayer_m_bEmojiBirthday 0xd50 // bool
#define LogicPlayer_logAttackSpeed 0xd51 // bool
#define LogicPlayer_doAttackSpeed 0xd52 // bool
#define LogicPlayer_m_CommATK_RunTimer 0xd54 // uint32
#define LogicPlayer_m_dCommATKSingTime_Mod 0xd58 // int32
#define LogicPlayer_m_CommATKSingTime_LastTimer 0xd5c // uint32
#define LogicPlayer_m_dCommATKCD_Mod 0xd60 // int32
#define LogicPlayer_m_CommATKCD_LastTimer 0xd64 // uint32
#define LogicPlayer_m_vPlayerDeadInfo 0xd68 // List<PlayerDeadInfo>
#define LogicPlayer_m_PriorEquip 0xd70 // uint32
#define LogicPlayer_m_RecmendEquips 0xd78 // List<uint>
#define LogicPlayer_m_uHeroEnhanceLevel 0xd80 // uint32
#define LogicPlayer_m_bGhostHasDied 0xd84 // bool
#define LogicPlayer_m_v2StarDir 0xd88 // DVector2
#define LogicPlayer_shopData 0xd98 // CantShopComp
#define LogicPlayer_v2LastCheckPos 0xda0 // DVector2
#define LogicPlayer_lastCheckDirSymbol 0xdb0 // int32
#define LogicPlayer_lastCheckMoveDir 0xdb8 // DVector2
#define LogicPlayer_right 0xdc8 // int32
#define LogicPlayer_lastFailedAutoAiSpellCast 0xdd0 // SpellCastData
#define LogicPlayer_lastFailedAutoAiSpellCastTime 0xdd8 // uint32
#define LogicPlayer_ownNormalSkillCache 0xde0 // IList<int>
#define LogicPlayer_autoTime 0xde8 // int32
#define LogicPlayer_m_dXpGrowthDecimal 0xdf0 // double
#define LogicPlayer_lEatFruits 0xdf8 // List<EatFruitData>
#define LogicPlayer_bBornedBoss 0xe00 // bool
#define LogicPlayer_iPreMutiKillValue 0xe04 // int32
#define LogicPlayer_iPreContinueKillValue 0xe08 // int32
#define LogicPlayer_iPreKillLingZhu 0xe0c // int32
#define LogicPlayer_iPreKillShenGui 0xe10 // int32
#define LogicPlayer_iPreShutDown 0xe14 // int32
#define LogicPlayer_bCheckFirstBlood 0xe18 // bool
#define LogicPlayer_iCurrentResult 0xe1c // int32
#define LogicPlayer_iPreGetResultTime 0xe20 // uint32
#define LogicPlayer_iCurKilledResult 0xe24 // int32
#define LogicPlayer_iPreKilledResultTime 0xe28 // uint32
