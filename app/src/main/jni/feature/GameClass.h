#pragma once
#include <cstdint>

// SystemData.RoomData (Player Entity) Offsets
constexpr uintptr_t SystemData_RoomData_lUid = 0x20;
constexpr uintptr_t SystemData_RoomData_bUid = 0x28;
constexpr uintptr_t SystemData_RoomData_iCamp = 0x30;
constexpr uintptr_t SystemData_RoomData_iPos = 0x34;
constexpr uintptr_t SystemData_RoomData_bAutoReadySelect = 0x38;
constexpr uintptr_t SystemData_RoomData_sName = 0x40; // _sName
constexpr uintptr_t SystemData_RoomData_bRobot = 0x48;
constexpr uintptr_t SystemData_RoomData_heroid = 0x4c;
constexpr uintptr_t SystemData_RoomData_heroskin = 0x50;
constexpr uintptr_t SystemData_RoomData_headID = 0x54;
constexpr uintptr_t SystemData_RoomData_uiSex = 0x58;
constexpr uintptr_t SystemData_RoomData_country = 0x5c;
constexpr uintptr_t SystemData_RoomData_uiZoneId = 0x60;
constexpr uintptr_t SystemData_RoomData_summonSkillId = 0x64;
constexpr uintptr_t SystemData_RoomData_ulRoomID = 0xa0;
constexpr uintptr_t SystemData_RoomData_uiRankLevel = 0x128;

// BattleData Offsets
constexpr uintptr_t BattleData_heroInfoList = 0x18; // Dictionary<uint, FightHeroInfo>
constexpr uintptr_t BattleData_fGameTime = 0x1f0;   // private uint fGameTime

// ShowFightDataTiny Offsets (Team Stats)
// Verified from dump.cs
constexpr uintptr_t ShowFightDataTiny_m_iCampAKill = 0xe8;
constexpr uintptr_t ShowFightDataTiny_m_iCampBKill = 0xec;
constexpr uintptr_t ShowFightDataTiny_m_CampAGold = 0xf0;
constexpr uintptr_t ShowFightDataTiny_m_CampBGold = 0xf4;
constexpr uintptr_t ShowFightDataTiny_m_CampAExp = 0xf8;
constexpr uintptr_t ShowFightDataTiny_m_CampBExp = 0xfc;
constexpr uintptr_t ShowFightDataTiny_m_CampAKillTower = 0x100;
constexpr uintptr_t ShowFightDataTiny_m_CampBKillTower = 0x104;
constexpr uintptr_t ShowFightDataTiny_m_CampAKillLingZhu = 0x108; // Lord
constexpr uintptr_t ShowFightDataTiny_m_CampBKillLingZhu = 0x10c;
constexpr uintptr_t ShowFightDataTiny_m_CampAKillShenGui = 0x110; // Turtle
constexpr uintptr_t ShowFightDataTiny_m_CampBKillShenGui = 0x114;

// MTTDProto.FightHeroInfo Offsets (Individual Battle Stats)
constexpr uintptr_t FightHeroInfo_m_uGuid = 0x18;
constexpr uintptr_t FightHeroInfo_m_KillNum = 0x20;
constexpr uintptr_t FightHeroInfo_m_DeadNum = 0x24;
constexpr uintptr_t FightHeroInfo_m_AssistNum = 0x28;
constexpr uintptr_t FightHeroInfo_m_Level = 0x2c;
constexpr uintptr_t FightHeroInfo_m_Gold = 0x30;
constexpr uintptr_t FightHeroInfo_m_TotalGold = 0x34;
constexpr uintptr_t FightHeroInfo_m_CampType = 0x38;
constexpr uintptr_t FightHeroInfo_m_HurtHeroValue = 0x68;
constexpr uintptr_t FightHeroInfo_m_HurtTowerValue = 0x6c;
constexpr uintptr_t FightHeroInfo_m_InjuredValue = 0x70;
constexpr uintptr_t FightHeroInfo_iKill3 = 0x190;
constexpr uintptr_t FightHeroInfo_iKill4 = 0x194;
constexpr uintptr_t FightHeroInfo_iKill5 = 0x198;
constexpr uintptr_t FightHeroInfo_iDestroyTowerNum = 0x19c;
constexpr uintptr_t FightHeroInfo_iMonsterCoin = 0x1a0;

// UIRankHero Offsets (Ban/Pick)
constexpr uintptr_t UIRankHero_banList = 0x1c0;
constexpr uintptr_t UIRankHero_pickList = 0x1c8;
constexpr uintptr_t UIRankHero_banOrder = 0x1d8;
constexpr uintptr_t UIRankHero_pickOrder = 0x1e0;
