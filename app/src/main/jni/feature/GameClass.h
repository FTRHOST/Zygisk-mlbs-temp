#pragma once

#include "../Include.h"

//Class Camera
#define Camera_get_main (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Camera", "get_main")
#define Camera_WorldToScreenPoint (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Camera", "WorldToScreenPoint", 1)
#define Camera_get_fieldOfView (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Camera", "get_fieldOfView")
#define Camera_set_fieldOfView (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Camera", "set_fieldOfView", 1)

void *get_main() {
	return reinterpret_cast<void *(__fastcall *)()>(Camera_get_main)();
}

Vector3 WorldToScreen(Vector3 position) {
	return reinterpret_cast<Vector3(__fastcall *)(void *, Vector3)>(Camera_WorldToScreenPoint)(get_main(), position);
}

float get_fieldOfView() {
	return reinterpret_cast<float(__fastcall *)(void *)>(Camera_get_fieldOfView)(get_main());
}

void *set_fieldOfView(float value) {
	return reinterpret_cast<void *(__fastcall *)(void *, float)>(Camera_set_fieldOfView)(get_main(), value);
}

//Class Transform
#define Transform_get_position (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Transform", "get_position")

class Transform {
public:
    Vector3 get_position() {
        auto get_position = (Vector3 (*)(Transform *)) (Transform_get_position);
        return get_position(this);
    }
};

//Class BattleManager
#define BattleManager_m_LocalPlayerShow (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "BattleManager", "m_LocalPlayerShow")
#define Camera_get_Positions (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "BattleManager", "m_LocalPlayerShow")
#define BattleManager_m_ShowPlayers (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "BattleManager", "m_ShowPlayers")
#define BattleManager_m_ShowMonsters (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "BattleManager", "m_ShowMonsters")
#define BattleManager_m_LocalPlayerShow (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "BattleManager", OBFUSCATE("m_LocalPlayerShow"))

#define BattleManager_m_dicPlayerShow (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "BattleManager", OBFUSCATE("m_dicPlayerShow"))

#define BattleManager_m_dicMonsterShow (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "BattleManager", OBFUSCATE("m_dicMonsterShow"))

#define BattleManager_m_ShowPlayers (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "BattleManager", OBFUSCATE("m_ShowPlayers"))

//Class BattleBridge
#define BattleBridge_bStartBattle (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "BattleBridge", "bStartBattle")


#define BattleManager_m_LocalShowPlayers (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("BattleManager"), OBFUSCATE("m_LocalShowPlayers"))
#define ShowPlayer_m_bInBattle (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("ShowPlayer"), OBFUSCATE("m_bInBattle"))

//Class ShowEntity
#define ShowEntity_get_InTransformation (uintptr_t) Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "ShowEntity", OBFUSCATE("get_InTransformation"))

#define ShowEntity__Position (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", OBFUSCATE("m_vCachePosition"))

#define ShowEntity_bShowEntityLayer (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", OBFUSCATE("bShowEntityLayer"))

#define ShowEntity_m_OwnSkillComp (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", OBFUSCATE("m_OwnSkillComp"))

#define ShowEntity_m_RoleName (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", OBFUSCATE("m_RoleName"))

#define ShowEntity__MoveDir (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", OBFUSCATE("_MoveDir"))
//Class EntityBase
#define EntityBase_bMove (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "Battle", "EntityBase", OBFUSCATE("bMove"))

#define EntityBase_m_ID (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", OBFUSCATE("m_ID"))

#define EntityBase_m_Level (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", OBFUSCATE("m_Level"))

#define EntityBase_m_bDeath (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", OBFUSCATE("m_bDeath"))

#define EntityBase_m_IsDeathProgress (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", OBFUSCATE("m_IsDeathProgress"))

#define EntityBase_m_bSameCampType (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", OBFUSCATE("m_bSameCampType"))

#define EntityBase_m_Hp (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", OBFUSCATE("m_Hp"))

#define EntityBase_m_HpMax (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", OBFUSCATE("m_HpMax"))
#define EntityBase_m_EntityCampType (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", OBFUSCATE("m_EntityCampType"))
#define EntityBase_m_iType (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", OBFUSCATE("m_iType"))
#define EntityBase_canSight (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", OBFUSCATE("canSight"))
#define EntityBase_m_uGuid (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", OBFUSCATE("m_uGuid"))
#define EntityBase_m_bSameCampType (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", "m_bSameCampType")
#define EntityBase_m_EntityCampType (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", OBFUSCATE("m_EntityCampType"))
#define EntityBase_m_IsDeathProgress (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", "m_IsDeathProgress")
#define EntityBase_m_bDeath (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", "m_bDeath")
#define EntityBase_m_Hp (uintptr_t)  Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", "m_Hp")
#define EntityBase_m_HpMax (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", "m_HpMax")
#define EntityBase_m_ID (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", "m_ID")
#define EntityBase_m_uGuid (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", "m_uGuid")
#define EntityBase_m_Level (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", "m_Level")
#define EntityBase_m_iType (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", "m_iType")
#define EntityBase_canSight (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", "canSight")

//Class ShowPlayer
#define ShowPlayer_m_HeroName (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowPlayer", OBFUSCATE("m_HeroName"))
#define ShowPlayer_m_HeroName (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "ShowPlayer", "m_HeroName")

//Class LogicFighter
#define LogicFighter_m_SkillComp (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "Battle", OBFUSCATE("LogicFighter"), OBFUSCATE("m_SkillComp"))

#define LogicBattleManager_GetBattleState (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("LogicBattleManager"), OBFUSCATE("GetBattleState"))

int GetBattleState(void* instance) {
    return reinterpret_cast<int(__fastcall *)(void*)>(LogicBattleManager_GetBattleState)(instance);
}

//Class SystemData
#define SystemData_GetBattlePlayerInfo (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("SystemData"), OBFUSCATE("GetBattlePlayerInfo"))

//Class SystemData.RoomData (Expanded with ALL User Requested Fields)
#define SystemData_RoomData_bAutoConditionNew (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "bAutoConditionNew")
#define SystemData_RoomData_bShowSeasonAchieve (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "bShowSeasonAchieve")
#define SystemData_RoomData_iStyleBoardId (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iStyleBoardId")
#define SystemData_RoomData_iMatchEffectId (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iMatchEffectId")
#define SystemData_RoomData_iDayBreakNo1Count (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iDayBreakNo1Count")
#define SystemData_RoomData_lUid (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "lUid")
#define SystemData_RoomData_bUid (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "bUid")
#define SystemData_RoomData_iCamp (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iCamp")
#define SystemData_RoomData_iPos (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iPos")
#define SystemData_RoomData_bAutoReadySelect (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "bAutoReadySelect")
#define SystemData_RoomData__sName (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "_sName")
#define SystemData_RoomData_bRobot (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "bRobot")
#define SystemData_RoomData_heroid (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "heroid")
#define SystemData_RoomData_heroskin (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "heroskin")
#define SystemData_RoomData_headID (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "headID")
#define SystemData_RoomData_uiSex (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "uiSex")
#define SystemData_RoomData_country (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "country")
#define SystemData_RoomData_uiZoneId (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "uiZoneId")
#define SystemData_RoomData_summonSkillId (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "summonSkillId")
#define SystemData_RoomData_runeId (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "runeId")
#define SystemData_RoomData_mapTalentTree (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "mapTalentTree")
#define SystemData_RoomData_mRuneSkill2023 (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "mRuneSkill2023")
#define SystemData_RoomData_runeLv (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "runeLv")
#define SystemData_RoomData_skinlist (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "skinlist")
#define SystemData_RoomData_facePath (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "facePath")
#define SystemData_RoomData_faceBorder (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "faceBorder")
#define SystemData_RoomData_bStarVip (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "bStarVip")
#define SystemData_RoomData_bMCStarVip (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "bMCStarVip")
#define SystemData_RoomData_bMCStarVipPlus (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "bMCStarVipPlus")
#define SystemData_RoomData_ulRoomID (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "ulRoomID")
#define SystemData_RoomData_iConBlackRoomId (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iConBlackRoomId")
#define SystemData_RoomData_banHero (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "banHero")
#define SystemData_RoomData_vCanSelectHero (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "vCanSelectHero")
#define SystemData_RoomData_vCanPickHero (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "vCanPickHero")
#define SystemData_RoomData_uiBattlePlayerType (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "uiBattlePlayerType")
#define SystemData_RoomData_sThisLoginCountry (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "sThisLoginCountry")
#define SystemData_RoomData_sCreateRoleCountry (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "sCreateRoleCountry")
#define SystemData_RoomData_uiLanguage (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "uiLanguage")
#define SystemData_RoomData_bIsOpenLive (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "bIsOpenLive")
#define SystemData_RoomData_iTeamId (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iTeamId")
#define SystemData_RoomData_iTeamNationId (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iTeamNationId")
#define SystemData_RoomData__steamName (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "_steamName")
#define SystemData_RoomData__steamSimpleName (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "_steamSimpleName")
#define SystemData_RoomData_iCertify (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iCertify")
#define SystemData_RoomData_lsEffectSkins (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "lsEffectSkins")
#define SystemData_RoomData_lsComEffSkins (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "lsComEffSkins")
#define SystemData_RoomData_vMissions (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "vMissions")
#define SystemData_RoomData_uiRankLevel (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "uiRankLevel")
#define SystemData_RoomData_uiPVPRank (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "uiPVPRank")
#define SystemData_RoomData_bRankReview (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "bRankReview")
#define SystemData_RoomData_iElo (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iElo")
#define SystemData_RoomData_uiRoleLevel (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "uiRoleLevel")
#define SystemData_RoomData_bNewPlayer (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "bNewPlayer")
#define SystemData_RoomData_iRoad (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iRoad")
#define SystemData_RoomData_uiSkinSource (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "uiSkinSource")
#define SystemData_RoomData_iFighterType (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iFighterType")
#define SystemData_RoomData_iWorldCupSupportCountry (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iWorldCupSupportCountry")
#define SystemData_RoomData_iHeroLevel (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iHeroLevel")
#define SystemData_RoomData_iHeroSubLevel (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iHeroSubLevel")
#define SystemData_RoomData_iHeroPowerLevel (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iHeroPowerLevel")
#define SystemData_RoomData_iActCamp (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iActCamp")
#define SystemData_RoomData_vTitle (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "vTitle")
#define SystemData_RoomData_mHeroMission (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "mHeroMission")
#define SystemData_RoomData_vEmoji (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "vEmoji")
#define SystemData_RoomData_vItemBuff (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "vItemBuff")
#define SystemData_RoomData_vMapPaint (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "vMapPaint")
#define SystemData_RoomData_mSkinPaint (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "mSkinPaint")
#define SystemData_RoomData_sClientVersion (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "sClientVersion")
#define SystemData_RoomData_uiHolyStatue (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "uiHolyStatue")
#define SystemData_RoomData_uiKamon (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "uiKamon")
#define SystemData_RoomData_uiUserMapID (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "uiUserMapID")
#define SystemData_RoomData_iSurviveRank (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iSurviveRank")
#define SystemData_RoomData_iDefenceRankID (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iDefenceRankID")
#define SystemData_RoomData_iLeagueWCNum (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iLeagueWCNum")
#define SystemData_RoomData_iLeagueFCNum (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iLeagueFCNum")
#define SystemData_RoomData_iMPLCertifyTime (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iMPLCertifyTime")
#define SystemData_RoomData_iMPLCertifyID (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iMPLCertifyID")
#define SystemData_RoomData_mapBattleAttr (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "mapBattleAttr")
#define SystemData_RoomData_iHeroUseCount (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iHeroUseCount")
#define SystemData_RoomData_iMythPoint (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iMythPoint")
#define SystemData_RoomData_bMythEvaled (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "bMythEvaled")
#define SystemData_RoomData_iDefenceFlag (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iDefenceFlag")
#define SystemData_RoomData_iDefenPoint (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iDefenPoint")
#define SystemData_RoomData_iDefenceMap (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iDefenceMap")
#define SystemData_RoomData_iAIType (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iAIType")
#define SystemData_RoomData_iAISeed (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iAISeed")
#define SystemData_RoomData_sAiName (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "sAiName")
#define SystemData_RoomData_iWarmValue (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iWarmValue")
#define SystemData_RoomData_uiAircraftIDChooose (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "uiAircraftIDChooose")
#define SystemData_RoomData_uiHeroIDChoose (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "uiHeroIDChoose")
#define SystemData_RoomData_uiHeroSkinIDChoose (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "uiHeroSkinIDChoose")
#define SystemData_RoomData_uiMapIDChoose (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "uiMapIDChoose")
#define SystemData_RoomData_uiMapSkinIDChoose (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "uiMapSkinIDChoose")
#define SystemData_RoomData_uiDefenceRankScore (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "uiDefenceRankScore")
#define SystemData_RoomData_bBanChat (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "bBanChat")
#define SystemData_RoomData_iChatBanFinishTime (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iChatBanFinishTime")
#define SystemData_RoomData_iChatBanBattleNum (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "iChatBanBattleNum")
#define SystemData_RoomData_vFastChat (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "vFastChat")
#define SystemData_RoomData_vWantS (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "vWantS")

// Added SystemData_StopCheckMD5 Definition to resolve undefined identifier error
#define SystemData_StopCheckMD5 (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("SystemData"), OBFUSCATE("StopCheckMD5"), 0)

//Duplikasi Room InfoTable (Consider removing duplicates if above covers them, but keeping for safety of existing code)
#define RoomData_steamSimpleName (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "_steamSimpleName")
#define RoomData_steamName (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", "_steamName")
#define RoomData_lUid (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", OBFUSCATE("lUid"))
#define RoomData_uiZoneId (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", OBFUSCATE("uiZoneId"))
#define RoomData_iCamp (uintptr_t) Il2CppGetStaticFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", OBFUSCATE("iCamp"))
#define RoomData_heroid (uintptr_t) Il2CppGetStaticFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", OBFUSCATE("heroid"))
#define RoomData_uiHeroIDChoose (uintptr_t) Il2CppGetStaticFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", OBFUSCATE("uiHeroIDChoose"))
#define RoomData_iCertify (uintptr_t) Il2CppGetStaticFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", OBFUSCATE("iCertify"))
#define RoomData_bRoomLeader (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", OBFUSCATE("bRoomLeader"))
#define RoomData_summonSkillId (uintptr_t) Il2CppGetStaticFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", OBFUSCATE("summonSkillId"))
#define RoomData_sName (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", OBFUSCATE("_sName"))
#define RoomData_uiRankLevel (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", OBFUSCATE("uiRankLevel"))
#define RoomData_iMythPoint (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", OBFUSCATE("iMythPoint"))
#define RoomData_uiSex (uintptr_t) Il2CppGetStaticFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", OBFUSCATE("uiSex"))
#define RoomData_sCreateRoleCountry (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", OBFUSCATE("sCreateRoleCountry"))
#define RoomData_Flag (uintptr_t) Il2CppGetStaticFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", OBFUSCATE("country"))
#define RoomData_iActCamp (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "SystemData/RoomData", OBFUSCATE("iActCamp"))

//class SystemData
#define SystemData_GetBattlePlayerInfo (uintptr_t) Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "SystemData", OBFUSCATE("GetBattlePlayerInfo"), 0)
#define SystemData_m_uiID (uintptr_t) Il2CppGetStaticFieldOffset("Assembly-CSharp.dll","","SystemData", OBFUSCATE("m_uiID"))


monoList<uintptr_t> *GetBattlePlayerInfo() {
    return reinterpret_cast<monoList<uintptr_t> *(*)(void *)>(SystemData_GetBattlePlayerInfo)(nullptr);
}

void StopMD5Check(){
    return reinterpret_cast<void (__fastcall *)(void *)>(SystemData_StopCheckMD5)((void *)0);
}


//Unlock Skin -> Mencoba
//Class UIRankHero.ChangeShow
#define UIRankHero_ChangeShow_iSelfHero (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("UIRankHero/ChangeShow"), OBFUSCATE("iSelfHero"))

//Class BattlePlayerInfo
#define BattlePlayerInfo_lUid (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("MTTDProto"), OBFUSCATE("BattlePlayerInfo"), OBFUSCATE("lUid"))
#define BattlePlayerInfo_uiSelHero (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("MTTDProto"), OBFUSCATE("BattlePlayerInfo"), OBFUSCATE("uiSelHero"))
#define BattlePlayerInfo_uiSkinId (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("MTTDProto"), OBFUSCATE("BattlePlayerInfo"), OBFUSCATE("uiSkinId"))

//Class CmdHeroSkin
#define CmdHeroSkin__ctor (uintptr_t) (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("MTTDProto"), OBFUSCATE("CmdHeroSkin"), OBFUSCATE(".ctor"))

//Class CmdHeroStatue
#define CmdHeroStatue__ctor (uintptr_t) (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("MTTDProto"), OBFUSCATE("CmdHeroStatue"), OBFUSCATE(".ctor"))

//Class SystemData,
#define SystemData_GetHeroSkin (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("SystemData"), OBFUSCATE("GetHeroSkin"), 2)
#define SystemData_GetMCLimitSkin (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("SystemData"), OBFUSCATE("GetMCLimitSkin"), 1)
#define SystemData_GetHeroHolyStatue (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("SystemData"), OBFUSCATE("GetHeroHolyStatue"), 2)
#define SystemData_IsCanUseSkin (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("SystemData"), OBFUSCATE("IsCanUseSkin"), 1)
#define SystemData_IsHaveSkin (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("SystemData"), OBFUSCATE("IsHaveSkin"), 1)
#define SystemData_IsHaveSkinForever (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("SystemData"), OBFUSCATE("IsHaveSkinForever"), 1)
#define SystemData_IsHaveStatue (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("SystemData"), OBFUSCATE("IsHaveStatue"), 1)
#define SystemData_IsHaveStatueForever (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("SystemData"), OBFUSCATE("IsHaveStatueForever"), 1)
#define SystemData_IsForbidStatue (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("SystemData"), OBFUSCATE("IsForbidStatue"), 1)
#define SystemData_IsForbidSkin (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("SystemData"), OBFUSCATE("IsForbidSkin"), 1)
#define SystemData_IsForbidARSkin (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("SystemData"), OBFUSCATE("IsForbidARSkin"), 1)
#define SystemData_CheckReputationUnlockSkin (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("SystemData"), OBFUSCATE("CheckReputationUnlockSkin"), 1)
#define SystemData_GetStarSkinRank (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("SystemData"), OBFUSCATE("GetStarSkinRank"),1)

#define SystemData_IsLimitActiveHero (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("SystemData"), OBFUSCATE("IsLimitActiveHero"), 1)
#define SystemData_IsActivityForbidHeros (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("SystemData"), OBFUSCATE("IsActivityForbidHeros"), 1)
#define SystemData_IsForbidHeroInChooseHero (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("SystemData"), OBFUSCATE("IsForbidHeroInChooseHero"), 1)
#define SystemData_IsForbidHeros (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("SystemData"), OBFUSCATE("IsForbidHeros"),1)

//Class UIChooseHero
#define UIChooseHero_SendSelectSkin (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("ChooseHeroMgr"), OBFUSCATE("SendSelectSkin"), 2)
#define UIChooseHero_SaveSelectHeroSkinId (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("ChooseHeroMgr"), OBFUSCATE("SaveSelectHeroSkinId"), 2)
#define ChooseHeroMgr_BCustomRoomFreeSkill (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("ChooseHeroMgr"), OBFUSCATE("BCustomRoomFreeSkill"), 0)
#define ChooseHeroMgr_OnSetSelfBattlePlayerInfo (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("ChooseHeroMgr"), OBFUSCATE("OnSetSelfBattlePlayerInfo"), 1)
#define UIDataUtil_IsHeroInShop (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("UIDataUtil"), OBFUSCATE("IsHeroInShop"), 1)

//Class UIRankHero
#define UIRankHero_BatttleSelectSkin (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("UIRankHero"), OBFUSCATE("BatttleSelectSkin"), 2)
#define UIRankHero_BatttleSelectSkin2 (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("UIChooseHero"), OBFUSCATE("BatttleSelectSkin"), 2)

#define UIRankHero_RefreshSkinDic (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("UIRankHero"), OBFUSCATE("RefreshSkinDic"), 3)
#define UIRankHero_RefreshHeroSkinShow (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("UIRankHero/RankingHeroShow"), OBFUSCATE("RefreshHeroSkinShow"), 3)

// UIRankHero BanPick Offsets
#define UIRankHero_banList (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("UIRankHero"), OBFUSCATE("banList"))
#define UIRankHero_pickList (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("UIRankHero"), OBFUSCATE("pickList"))
#define UIRankHero_banOrder (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("UIRankHero"), OBFUSCATE("banOrder"))
#define UIRankHero_pickOrder (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("UIRankHero"), OBFUSCATE("pickOrder"))
#define UIRankHero__startBanTime (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("UIRankHero"), OBFUSCATE("_startBanTime"))
#define UIRankHero__startSelectTime (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("UIRankHero"), OBFUSCATE("_startSelectTime"))
#define UIRankHero_iBanTimeSpan (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("UIRankHero"), OBFUSCATE("iBanTimeSpan"))
#define UIRankHero_iPickTimeSpan (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("UIRankHero"), OBFUSCATE("iPickTimeSpan"))

// Battle Stats
#define BattleStaticInit_GetTime (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("BattleStaticInit"), OBFUSCATE("GetTime"))

// BattleData & FightHeroInfo (For infobattle)
#define BattleData_heroInfoList (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "", "BattleData", "heroInfoList")
#define FightHeroInfo_m_KillNum (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "MTTDProto", "FightHeroInfo", "m_KillNum")
#define FightHeroInfo_m_DeadNum (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "MTTDProto", "FightHeroInfo", "m_DeadNum")
#define FightHeroInfo_m_AssistNum (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "MTTDProto", "FightHeroInfo", "m_AssistNum")
#define FightHeroInfo_m_Gold (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "MTTDProto", "FightHeroInfo", "m_Gold")
#define FightHeroInfo_m_TotalGold (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "MTTDProto", "FightHeroInfo", "m_TotalGold")
#define FightHeroInfo_m_PlayerName (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "MTTDProto", "FightHeroInfo", "m_PlayerName")
#define FightHeroInfo_m_uGuid (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "MTTDProto", "FightHeroInfo", "m_uGuid")
#define FightHeroInfo_m_CampType (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "MTTDProto", "FightHeroInfo", "m_CampType")

//Class UIRankHero.ChangeShow
#define UIRankHero_ChangeShow_TrySendUseSkin (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("UIRankHero/ChangeShow"), OBFUSCATE("TrySendUseSkin"), 2)
#define UIRankHero_ChangeShow_SendUseSkin (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("UIRankHero/ChangeShow"), OBFUSCATE("SendUseSkin"), 2)


//Class BattleReceiveMessage
#define BattleReceiveMessage_SetPlayerData (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("BattleReceiveMessage"), OBFUSCATE("SetPlayerData"), 2)
#define BattleReceiveMessage_SetPlayerData_ (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("BattleReceiveMessage"), OBFUSCATE("SetPlayerData"), 1)
#define BattleReceiveMessage_AddPlayerInfo (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("BattleReceiveMessage"), OBFUSCATE("AddPlayerInfo"), 4)
#define BattleReceiveMessage_AddPlayerInfo_ (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("BattleReceiveMessage"), OBFUSCATE("AddPlayerInfo"), 2)
#define BattleReceiveMessage_SendBattleData (uintptr_t) Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "BattleReceiveMessage", "SendBattleData", 4)
#define GameReceiveMessage_SendGameData (uintptr_t) Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "GameReceiveMessage", "SendGameData", 3)
#define GameReceiveMessage_SendGameRawData (uintptr_t) Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "GameReceiveMessage", "_SendGameRawData", 6)

//Class GameServerConfig
#define GameServerConfig_SendRawData (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("GameServerConfig"), OBFUSCATE("SendRawData"), 8)
#define GameServerConfig_SendData2 (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("GameServerConfig"), OBFUSCATE("SendData"), 6)
#define GameServerConfig_IsChannel_dfjz (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("GameServerConfig"), OBFUSCATE("IsChannel_dfjz"), 0)
#define SystemData_GetExpiredCardSkinLimitTime (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("SystemData"), OBFUSCATE("GetExpiredCardSkinLimitTime"), 1)
#define SystemData_GetLeaderSkinBForbid (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("SystemData"), OBFUSCATE("GetLeaderSkinBForbid"), 1)
#define UIRankHero_BRankHeroCanUse (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("UIRankHero"), OBFUSCATE("BRankHeroCanUse"), 1)
#define StarMemberData_ContainSkin (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("StarMemberData"), OBFUSCATE("ContainSkin"), 1)
#define UIChooseHero_CheckHeroDefaultSkin (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("UIChooseHero"), OBFUSCATE("CheckHeroDefaultSkin"),1)