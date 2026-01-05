#pragma once

struct RInfoBlueTeam {
    std::string Name;
    std::string UserID;
    std::string Squad;
    std::string Rank;
    std::string Hero;
    std::string Spell;
};

struct RInfoRedTeam {
    std::string Name;
    std::string UserID;
    std::string Squad;
    std::string Rank;
    std::string Hero;
    std::string Spell;
};

struct {
    RInfoBlueTeam PlayerB[10];
    RInfoRedTeam PlayerR[10];
} RoomInfo;
struct {
    struct sRoomInfo {
        bool Name;
        bool Rank;
        bool Spell;
        bool Hero;
        bool Team;
        bool Screen;
    };
    sRoomInfo RoomInfo{0};

} Esp;
struct sResponse {
    int count;
    int state;
    int GetBattleState;
    int Herolevel;
    int BattleType;
};
sResponse response{0};


int Cimod_Screen(ImDrawList *draw,ImVec2 screen){
    if(!Esp.RoomInfo.Screen) return 0;
    void *LogicBattleManager = 0;
    Il2CppGetStaticFieldValue(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("LogicBattleManager"), OBFUSCATE("Instance"), &LogicBattleManager);
    if(!LogicBattleManager) return 0;
    int State = GetBattleState(LogicBattleManager);
    if (State == 2 || State == 3) {
        auto roomLists = ((monoList<void **> *(*)(uintptr_t))SystemData_GetBattlePlayerInfo)(0);
        if(!roomLists) return 0;
        int iSelfCamp = -1;
        uint64_t m_uiID;
        Il2CppGetStaticFieldValue(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("SystemData"), OBFUSCATE("m_uiID"), &m_uiID);
        for(int i = 0;i < roomLists->getSize();i ++){
            void *roomData = roomLists->getItems()[i];
            if(!roomData) continue;
            auto lUid = *(int *) ((uintptr_t)roomData + SystemData_RoomData_lUid);
            if (lUid != m_uiID) continue;
            iSelfCamp = *(int *) ((uintptr_t)roomData + SystemData_RoomData_iCamp);

            break;
        }

        auto marginX = screen.x / 4;
        ImVec2 teamPos[] {
                {marginX, screen.y / 6.00f},
                {marginX, screen.y / 3.00f},
                {marginX, screen.y / 2.00f},
                {marginX, screen.y / 1.50f},
                {marginX, screen.y / 1.20f},
                {marginX, screen.y / 1.00f},
        };
        ImVec2 enemyPos[] {
                {screen.x - marginX, screen.y / 6.00f},
                {screen.x - marginX, screen.y / 3.00f},
                {screen.x - marginX, screen.y / 2.00f},
                {screen.x - marginX, screen.y / 1.50f},
                {screen.x - marginX, screen.y / 1.20f},
                {screen.x - marginX, screen.y / 1.00f},
        };
        // MY TEAM NAME //
        ImVec2 titlePos(teamPos[0].x, teamPos[0].y - (20 * 4));
        std::string strTeamName = " TEAM ";
        draw->AddText(NULL, 20, titlePos, IM_COL32(0, 255, 0, 255), strTeamName.c_str());
        draw->AddText(NULL, 20, titlePos, IM_COL32(0, 255, 0, 255), strTeamName.c_str());
        draw->AddText(NULL, 20, titlePos, IM_COL32(0, 255, 0, 255), strTeamName.c_str());

        // ENEMY TEAM NAME //
        titlePos = ImVec2(enemyPos[0].x - 70, enemyPos[0].y - (20 * 4) );
        std::string strEnemyTeamName = " ENEMY ";
        draw->AddText(NULL, 20, titlePos, IM_COL32(255, 0, 0, 255), strEnemyTeamName.c_str());
        draw->AddText(NULL, 20, titlePos, IM_COL32(255, 0, 0, 255), strEnemyTeamName.c_str());
        draw->AddText(NULL, 20, titlePos, IM_COL32(255, 0, 0, 255), strEnemyTeamName.c_str());
        for(int i = 0;i < roomLists->getSize();i ++){
            void *roomData = roomLists->getItems()[i];
            if(!roomData) continue;
            int summonSkillId = *(int *) ((uintptr_t) roomData + SystemData_RoomData_summonSkillId);
            int rankLevel = *(int *)((uint64_t) roomData + SystemData_RoomData_uiRankLevel);
            int iMythPoint = *(int *) ((uintptr_t) roomData + SystemData_RoomData_iMythPoint);

            int iPos = *(int *)((uint64_t) roomData +  SystemData_RoomData_iPos);
            int campType = *(int *)((uint64_t) roomData + SystemData_RoomData_iCamp);
            uint32_t heroid = *(uint32_t *)((uint64_t) roomData + SystemData_RoomData_heroid);
            MonoString *playerName = *(MonoString **)((long) roomData + SystemData_RoomData__sName);
            const char* rankName = getRankName(rankLevel);

            bool sameTeam = campType == iSelfCamp;

            ImVec2 lockPos(0, 0);
            float x = sameTeam ? teamPos[iPos].x : enemyPos[iPos].x;
            float y = sameTeam ? teamPos[iPos].y : enemyPos[iPos].y;

            // Kotak latar belakang abu-abu transparan
            ImVec2 boxPos(x - 10, y - 20);
            ImVec2 boxSize(175, 100); // Sesuaikan ukuran kotak sesuai kebutuhan
            ImVec4 bgColor(0.0f, 0.0f, 0.0f, 0.5f); // Warna hitam transparan

            //   draw->AddRectFilled(boxPos, ImVec2(boxPos.x + boxSize.x, boxPos.y + boxSize.y), IM_COL32(255, 255, 255, 255)); // Kotak putih sebagai border
            draw->AddRectFilled(boxPos, ImVec2(boxPos.x + boxSize.x, boxPos.y + boxSize.y), IM_COL32(bgColor.x * 255, bgColor.y * 255, bgColor.z * 255, bgColor.w * 255)); // Kotak abu-abu transparan

            if (Esp.RoomInfo.Name) {
                lockPos = ImVec2(boxPos.x + 10, boxPos.y + 10);
                std::string _sName("Nick : ");
                if (playerName)
                    _sName.append(playerName->toChars());
                else
                    _sName.append(" - ");
                draw->AddText(NULL, 14, lockPos, IM_COL32(255, 255, 255, 255), _sName.c_str());
            }
            if (Esp.RoomInfo.Rank) {
                lockPos = ImVec2(boxPos.x + 10, boxPos.y + 30);
                std::string strRank("Rank : ");
                strRank.append(rankName);
                draw->AddText(NULL, 14, lockPos, IM_COL32(255, 230, 0, 255), strRank.c_str());
            }
            if (State == 3) {
                if (Esp.RoomInfo.Spell) {
                    std::string strsummonSkillId("Spell : ");
                    strsummonSkillId.append(getSummonid(summonSkillId));
                    lockPos = ImVec2(boxPos.x + 10, boxPos.y + 70);
                    draw->AddText(NULL, 14, lockPos, IM_COL32(255, 230, 0, 255), strsummonSkillId.c_str());
			        int spellid = SpellPic(summonSkillId);
                    DrawSpellHeadField(lockPos, spellid, 1, 1, 45);
                }
                if (Esp.RoomInfo.Hero) {
                    ImVec2 iconPos(boxPos.x - 30, boxPos.y + 30); // Sesuaikan posisi ikon sesuai kebutuhan
                    //DrawHeroIcon2(heroid, iconPos);
                    //DrawIconLogo(3, iconPos, heroid, 100, 100, 50, 0, false);
                    DrawIconLogo(3, iconPos, heroid, 100, 100, 100, 0, false);
                }			
            }
        }
    }
    return 0;
}
void RoomInfoList() {
    void *LogicBattleManager_Instance;
    Il2CppGetStaticFieldValue(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("LogicBattleManager"), OBFUSCATE("Instance"), &LogicBattleManager_Instance);
    if (LogicBattleManager_Instance) {
        int iGetBattleState = GetBattleState(LogicBattleManager_Instance);
        if (iGetBattleState == 2 || iGetBattleState == 3) {
            auto GetBattlePlayerInfo = ((monoList<void **> *(*)(uintptr_t))SystemData_GetBattlePlayerInfo)(0);
            if (GetBattlePlayerInfo) {
                int PlayerB = 0, PlayerR = 0;
                int iSelfCamp = -1;
                int m_uiID;
                Il2CppGetStaticFieldValue(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("SystemData"), OBFUSCATE("m_uiID"), &m_uiID);
                if (!m_uiID) return;
                for (int i = 0; i < GetBattlePlayerInfo->getSize(); i++) {
                    auto Pawn = GetBattlePlayerInfo->getItems()[i];
                    if (!Pawn) continue;
                    auto iCamp = *(int *) ((uintptr_t)Pawn + SystemData_RoomData_iCamp);
                    auto lUid = *(int *) ((uintptr_t)Pawn + SystemData_RoomData_lUid);
                    if (lUid != m_uiID) continue;
                    iSelfCamp = iCamp;
                    break;
                }
                for (int i = 0; i < GetBattlePlayerInfo->getSize(); i++) {
                    auto Pawn = GetBattlePlayerInfo->getItems()[i];
                    if (!Pawn) continue;
                    auto iCamp = *(int *) ((uintptr_t)Pawn + SystemData_RoomData_iCamp);
                    auto _sName = *(MonoString **)((uintptr_t)Pawn + SystemData_RoomData__sName);
                    auto _steamSimpleName = *(MonoString **)((uintptr_t)Pawn + SystemData_RoomData__steamSimpleName);
                    auto _steamName = *(MonoString **)((uintptr_t)Pawn + SystemData_RoomData__steamName);
                    auto uiRankLevel = *(int *) ((uintptr_t)Pawn + SystemData_RoomData_uiRankLevel);
                    auto iMythPoint = *(int *) ((uintptr_t)Pawn + SystemData_RoomData_iMythPoint);
                    auto heroid = *(int *) ((uintptr_t)Pawn + SystemData_RoomData_heroid);
                    auto summonSkillId = *(int *) ((uintptr_t)Pawn + SystemData_RoomData_summonSkillId);
                    auto lUid = *(int *) ((uintptr_t)Pawn + SystemData_RoomData_lUid);
                    auto uiZoneId = *(int *) ((uintptr_t)Pawn + SystemData_RoomData_uiZoneId);
                    if (iCamp == iSelfCamp) {
                        if (_sName) {
                            RoomInfo.PlayerB[PlayerB].Name = _sName->toString();
                        }
                        RoomInfo.PlayerB[PlayerB].UserID = to_string(lUid) + " (" + to_string(uiZoneId) + ")";
                        if (_steamSimpleName && _steamName) {
                            if (_steamSimpleName->toString() != "" && _steamName->toString() != "") {
                                RoomInfo.PlayerB[PlayerB].Squad = "[" + _steamSimpleName->toString() + "]" + _steamName->toString();
                            }
                        }
                        RoomInfo.PlayerB[PlayerB].Rank = RankToString(uiRankLevel, iMythPoint);
                        RoomInfo.PlayerB[PlayerB].Hero = HeroToString(heroid);
                        RoomInfo.PlayerB[PlayerB].Spell = SpellToString(summonSkillId);
                        PlayerB++;
                    } else {
                        if (_sName) {
                            RoomInfo.PlayerR[PlayerR].Name = _sName->toString();
                        }
                        RoomInfo.PlayerR[PlayerR].UserID = to_string(lUid) + " (" + to_string(uiZoneId) + ")";
                        if (_steamSimpleName && _steamName) {
                            if (_steamSimpleName->toString() != "" && _steamName->toString() != "") {
                                RoomInfo.PlayerR[PlayerR].Squad = "[" + _steamSimpleName->toString() + "]" + _steamName->toString();
                            }
                        }
                        RoomInfo.PlayerR[PlayerR].Rank = RankToString(uiRankLevel, iMythPoint);
                        RoomInfo.PlayerR[PlayerR].Hero = HeroToString(heroid);
                        RoomInfo.PlayerR[PlayerR].Spell = SpellToString(summonSkillId);
                        PlayerR++;
                    }
                }
            }
        }
    }
}
#define DefineHook2(RET, NAME, ARGS) \
    RET (*o ## NAME) ARGS; \
    RET NAME ARGS

uintptr_t Fields_ObjectOption_pos_Foot,Fields_ObjectOption_pos_Head,Fields_EntityBase_m_ID,Fields_EntityBase_m_HpMax,Fields_ShowEntity_m_RoleName,Fields_ShowPlayer_m_HeroName,Fields_ShowPlayer_m_Config,Fields_ShowEntity_bShowEntityLayer;
uintptr_t OffsetFunc_ShowSelfPlayer_TryUseSkill, Offsets_LogicSkillComp_InSkillCdForShow,Offsets_ShowEntity_get_Position,Fields_ConcurrentPlayerData_pOwner,Fields_EntityBase_m_Level,Fields_EntityBase_m_Xp,Fields_LogicPlayer_m_KillWildTimes,Fields_ShowEntity_MoveDir,Fields_LogicPlayer_dicTalentSkill,Fields_EntityBase_m_Hp;
uintptr_t Fields_ShowEntity_m_HeadIcon, OffsetFunc_ShowEntity_UpdateEyeLayer,Fields_EntityBase_m_bDeath,Fields_EntityBase_m_IsDeathProgress,Fields_EntityBase_m_uGuid,Fields_EntityBase_m_bSameCampType;
uintptr_t Fields_ShowEntity__Position,OffsetFunc_BattleBridge_SynBloodAttr,OffsetFunc_BattleBridge_UpdateBloodPos, OffsetFunc_BattleBridge_SetMapEntityIconPos, OffsetFunc_BattleBridge_SetBloodInvisibility, OffsetFunc_BattleBridge_ShowHeadEquip, OffsetFunc_BattleBridge_SetMapInvisibility;
uintptr_t Offsets_CoolDownData_GetCoolTime,Fields_CoolDownComp_m_DicCoolInfo,Offsets_ShowSelfPlayer_TryUseSkillByLockUnit,Fields_EntityBase_m_iType,Fields_BattleManager__m_ShowMonsters,Fields_ShowMonster_m_Config,Fields_CData_Monster_Element_m_ID;
uintptr_t Offsets_Screen__SetResolution,Fields_BattleBridge_bStartBattle,Fields_BattleManager__m_LocalPlayerShow,Fields_BattleManager_m_dicPlayerShow,Fields_ShowEntity_m_EntityTsf; 
uintptr_t Fields_LogicBattleManager__m_eBattleState,Fields_LogicSkillComp_m_CoolDownComp,Offsets_Transform_get_position,Offsets_ShowEntity_get_m_obPo,Offsets_Camera__WorldToScreenPoint,Offsets_Transform_get_localPosition;
 

