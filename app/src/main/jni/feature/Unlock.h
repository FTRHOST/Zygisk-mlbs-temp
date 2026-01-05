#pragma once
#define DefineHook(RET, NAME, ARGS) \
    RET (*o ## NAME) ARGS; \
    RET NAME ARGS


struct CmdHeroSkin : Il2CppObject {
    uint32_t iId;
    uint32_t iLimitTime;
    uint32_t iSource;
};


struct CmdHeroStatue : Il2CppObject {
    uint32_t iId;
    uint32_t iLimitTime;
    uint32_t iSource;
};
bool ndUnlockSkin3 = true;
bool ndUnlockSkin = false;


DefineHook(CmdHeroSkin *, GetHeroSkin, (void * unk, uintptr_t m_heroskins, uint skinid)) {
    auto ret = oGetHeroSkin(unk, m_heroskins, skinid);

    if (ret||!ndUnlockSkin)
        return ret;

    auto MTTDProto_CmdHeroSkin_ctor = (void *(*)(CmdHeroSkin *)) (CmdHeroSkin__ctor);
    auto instance = (CmdHeroSkin *) Il2CppCreateClassInstance("Assembly-CSharp.dll", "MTTDProto", "CmdHeroSkin");
    if (MTTDProto_CmdHeroSkin_ctor(instance)) {
        instance->iId = skinid;
        instance->iLimitTime = 0;
        instance->iSource = 0;
    }
    return instance;
}

DefineHook(CmdHeroSkin *, GetMCLimitSkin, (void * unk, uint skinId)) {
    auto ret = oGetMCLimitSkin(unk, skinId);

    if (ret||!ndUnlockSkin)
        return ret;

    auto  MTTDProto_CmdHeroSkin_ctor = (void *(*)(CmdHeroSkin *)) (CmdHeroSkin__ctor);
    auto instance = (CmdHeroSkin *) Il2CppCreateClassInstance("Assembly-CSharp.dll", "MTTDProto", "CmdHeroSkin");
    if (MTTDProto_CmdHeroSkin_ctor(instance)) {
        instance->iId = skinId;
        instance->iLimitTime = 0;
        instance->iSource = 0;
    }
    return instance;
}

DefineHook(CmdHeroStatue *, GetHeroHolyStatue, (void * unk, uintptr_t m_heroStatues, uint statueid)) {
    auto ret = oGetHeroHolyStatue(unk, m_heroStatues, statueid);

    if (ret||!ndUnlockSkin)
        return ret;

    auto MTTDProto_CmdHeroStatue_ctor = (void *(*)(CmdHeroStatue *)) (CmdHeroStatue__ctor);
    auto instance = (CmdHeroStatue *) Il2CppCreateClassInstance("Assembly-CSharp.dll", "MTTDProto", "CmdHeroStatue");
    if (MTTDProto_CmdHeroStatue_ctor(instance)) {
        instance->iId = statueid;
        instance->iLimitTime = 0;
        instance->iSource = 0;

    }
    return instance;
}

DefineHook(bool, IsCanUseSkin, (void * unk, int heroid)) {
    return true;
}

DefineHook(CmdHeroSkin *, IsHaveSkin, (void * unk, int skinid)) {
    auto ret = oIsHaveSkin(unk, skinid);

    if (ret||!ndUnlockSkin)
        return ret;

    auto MTTDProto_CmdHeroSkin_ctor = (void *(*)(CmdHeroSkin *)) (CmdHeroSkin__ctor);
    auto instance = (CmdHeroSkin *) Il2CppCreateClassInstance("Assembly-CSharp.dll", "MTTDProto", "CmdHeroSkin");
    if (MTTDProto_CmdHeroSkin_ctor(instance)) {
        instance->iId = skinid;
        instance->iLimitTime = 0;
        instance->iSource = 0;
    }
    return instance;
}

DefineHook(CmdHeroSkin *, IsHaveSkinForever, (void * unk, int skinid)) {
    auto ret = oIsHaveSkinForever(unk, skinid);

    if (ret||!ndUnlockSkin)
        return ret;

    auto MTTDProto_CmdHeroSkin_ctor = (void *(*)(CmdHeroSkin *)) (CmdHeroSkin__ctor);
    auto instance = (CmdHeroSkin *) Il2CppCreateClassInstance("Assembly-CSharp.dll", "MTTDProto", "CmdHeroSkin");
    if (MTTDProto_CmdHeroSkin_ctor(instance)) {
        instance->iId = skinid;
        instance->iLimitTime = 0;
        instance->iSource = 0;
    }
    return instance;
}

DefineHook(CmdHeroStatue *, IsHaveStatue, (void * unk, uint32_t statueid)) {
    auto ret = oIsHaveStatue(unk, statueid);

    if (ret||!ndUnlockSkin)
        return ret;

    auto MTTDProto_CmdHeroStatue_ctor = (void *(*)(CmdHeroStatue *)) (CmdHeroStatue__ctor);
    auto instance = (CmdHeroStatue *) Il2CppCreateClassInstance("Assembly-CSharp.dll", "MTTDProto", "CmdHeroStatue");
    if (MTTDProto_CmdHeroStatue_ctor(instance)) {
        instance->iId = statueid;
        instance->iLimitTime = 0;
        instance->iSource = 0;
    }
    return instance;
}

DefineHook(CmdHeroStatue *, IsHaveStatueForever, (void * unk, uint32_t statueid)) {
    auto ret = oIsHaveStatueForever(unk, statueid);

    if (ret||!ndUnlockSkin)
        return ret;

    auto MTTDProto_CmdHeroStatue_ctor = (void *(*)(CmdHeroStatue *)) (CmdHeroStatue__ctor);
    auto instance = (CmdHeroStatue *) Il2CppCreateClassInstance("Assembly-CSharp.dll", "MTTDProto", "CmdHeroStatue");
    if (MTTDProto_CmdHeroStatue_ctor(instance)) {
        instance->iId = statueid;
        instance->iLimitTime = 0;
        instance->iSource = 0;
    }
    return instance;
}
DefineHook(uint, GetExpiredCardSkinLimitTime, (void * unk, uint32_t skinid)) {
    if (ndUnlockSkin) {
        return oGetExpiredCardSkinLimitTime(unk,skinid);
    }
}
DefineHook(bool, GetLeaderSkinBForbid, (void * unk, uint32_t skinid)) {
    if (ndUnlockSkin) {
        return true;
    }
}
DefineHook(bool, BRankHeroCanUse, (void * unk, uint32_t skinid)) {
    if (ndUnlockSkin) {
        return true;
    }
}
DefineHook(bool, ContainSkin, (void * unk, uint32_t skinid)) {
    if (ndUnlockSkin) {
        return false;
    }
}
DefineHook(bool, UIChooseHero_ShowSkinExpiryDate, (void * unk)) {
    return true;
}

DefineHook(bool, IsForbidStatue, (void * unk, uint32_t statueid)) {
    return false;
}

DefineHook(bool, IsForbidSkin, (void * unk, uint32_t skinid)) {
    return false;
}
DefineHook(bool, CheckReputationUnlockSkin, (void * unk, int skinid)) {
    return true;
}
DefineHook(bool, SendData, (void * unk, int skinid)) {
    return true;
}
DefineHook(bool, GetStarSkinRank, (void * unk, uint32_t skinid)) {
    return true;
}
DefineHook(bool, IsForbidARSkin, (void * unk, uint32_t id)) {
    return false;
}

DefineHook(bool, IsForbidHeros, (void * unk, uint32_t heroId)) {
    return false;
}
DefineHook(bool, IsForbidHeroInChooseHero, (void * unk, uint32_t heroId)) {
    return false;
}
DefineHook(bool, IsActivityForbidHeros, (void * unk, uint32_t heroId)) {
    return false;
}
DefineHook(bool, IsLimitActiveHero, (void * unk, uint32_t heroId)) {
    return true;
}
DefineHook(bool, IsHeroInShop, (void * unk, int heroId)) {
    return true;
}
uint32_t m_HeroID = 0, m_SkinID = 0;
DefineHook(void, SaveSelectHeroSkinId, (uintptr_t thiz, uint32_t skinid, uint32_t heroid)) {
    if (oIsHaveSkin(0, skinid) || oIsHaveSkinForever(0, skinid) || ndUnlockSkin) {
        m_SkinID = 0;
        return oSaveSelectHeroSkinId(thiz, skinid, heroid);
    }
    m_HeroID = heroid;
    m_SkinID = skinid;
}
DefineHook(void, SendSelectSkin, (uintptr_t thiz, uint32_t skinid, uint32_t heroid)) {
    if (oIsHaveSkin(0, skinid) || oIsHaveSkinForever(0, skinid) || !ndUnlockSkin) {
        m_SkinID = 0;
        return oSendSelectSkin(thiz, skinid, heroid);
    }

    auto UIChooseHero_BatttleSelectSkin = (void (*)(uintptr_t, uint64_t, uint32_t)) (Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "UIChooseHero", "BatttleSelectSkin", 2));

    uint64_t m_uiID;
    Il2CppGetStaticFieldValue("Assembly-CSharp.dll", "", "SystemData", "m_uiID", &m_uiID);
    UIChooseHero_BatttleSelectSkin(thiz, m_uiID, skinid);

    m_HeroID = heroid;
    m_SkinID = skinid;
}
DefineHook(void, RefreshSkinDic, (uintptr_t thiz, uint32_t heroid, uint skinid, uint64_t uid )) {
    if (ndUnlockSkin) {
        uint64_t m_uiID;
        Il2CppGetStaticFieldValue("Assembly-CSharp.dll", "", "SystemData", "m_uiID", &m_uiID);
        return oRefreshSkinDic(thiz, m_HeroID, m_SkinID, m_uiID);
    }
    m_HeroID = *(uint32_t *) ((uintptr_t) thiz + UIRankHero_ChangeShow_iSelfHero);
    m_SkinID = skinid;
}


DefineHook(void, RefreshHeroSkinShow, (uintptr_t thiz, uint64_t uid, uint32_t heroid, uint skinid)) {
    if (ndUnlockSkin) {
        uint64_t m_uiID;
        Il2CppGetStaticFieldValue("Assembly-CSharp.dll", "", "SystemData", "m_uiID", &m_uiID);
        return oRefreshHeroSkinShow(thiz, m_uiID, m_HeroID, m_SkinID);
    }
    m_HeroID = *(uint32_t *) ((uintptr_t) thiz + UIRankHero_ChangeShow_iSelfHero);
    m_SkinID = skinid;
}
void (*oBatttleSelectSkin2)(void* thiz_, uint64_t uid, uint skinid);
void BatttleSelectSkin2(void* thiz_, uint64_t uid, uint skinid) {
    if (ndUnlockSkin3) {
        uint64_t m_uiID;
        Il2CppGetStaticFieldValue("Assembly-CSharp.dll", "", "SystemData", "m_uiID", &m_uiID);
        return oBatttleSelectSkin2(thiz_, m_uiID, m_SkinID);
    }
}
DefineHook(void, BatttleSelectSkin, (uintptr_t thiz, uint64_t uid, uint skinid)) {
    if (ndUnlockSkin) {
        uint64_t m_uiID;
        Il2CppGetStaticFieldValue("Assembly-CSharp.dll", "", "SystemData", "m_uiID", &m_uiID);
        return oBatttleSelectSkin(thiz, m_uiID, m_SkinID);
    }
}
DefineHook(void, TrySendUseSkin, (void * thiz, uint32_t skinid, bool)) {
    if (oIsHaveSkin(0, skinid) || oIsHaveSkinForever(0, skinid) || !ndUnlockSkin) {
        m_SkinID = 0;
        return oTrySendUseSkin(thiz, skinid, true);
    }

    m_HeroID = *(uint32_t *) ((uintptr_t) thiz + UIRankHero_ChangeShow_iSelfHero);
    m_SkinID = skinid;
}

DefineHook(void, SendUseSkin, (void * thiz, uint32_t skinid, bool)) {
    if (oIsHaveSkin(0, skinid) || oIsHaveSkinForever(0, skinid) || !ndUnlockSkin) {
        m_SkinID = 0;
        return oSendUseSkin(thiz, skinid, true);
    }
    m_HeroID = *(uint32_t *) ((uintptr_t) thiz + UIRankHero_ChangeShow_iSelfHero);
    m_SkinID = skinid;
}
DefineHook(void, SetPlayerData, (uintptr_t thiz, uintptr_t playerinfo, uint32_t uiSelfCamp)) {
    if (ndUnlockSkin) {
        if (m_SkinID) {
            if (!oIsHaveSkin(0, m_SkinID) || !oIsHaveSkinForever(0, m_SkinID)) {
                uint64_t m_uiID;
                Il2CppGetStaticFieldValue("Assembly-CSharp.dll", "", "SystemData", "m_uiID", &m_uiID);
                if (*(uint64_t *) (playerinfo + BattlePlayerInfo_lUid) == m_uiID) {
                    uint32_t heroid = *(uint32_t *) (playerinfo + BattlePlayerInfo_uiSelHero);
                    if (heroid == m_HeroID) {
                        *(uint32_t *) ((uintptr_t) playerinfo + BattlePlayerInfo_uiSkinId) = m_SkinID;
                    }
                }
            }
        }
    }
    return oSetPlayerData(thiz, playerinfo, uiSelfCamp);
}
DefineHook(void, SetPlayerData_, (uintptr_t thiz, uintptr_t playerinfo)) {
    if (ndUnlockSkin) {
        if (m_SkinID) {
            if (!oIsHaveSkin(0, m_SkinID) || !oIsHaveSkinForever(0, m_SkinID)) {
                uint64_t m_uiID;
                Il2CppGetStaticFieldValue("Assembly-CSharp.dll", "", "SystemData", "m_uiID", &m_uiID);
                if (*(uint64_t *) (playerinfo + BattlePlayerInfo_lUid) == m_uiID) {
                    uint32_t heroid = *(uint32_t *) (playerinfo + BattlePlayerInfo_uiSelHero);
                    if (heroid == m_HeroID) {
                        *(uint32_t *) ((uintptr_t) playerinfo + BattlePlayerInfo_uiSkinId) = m_SkinID;
                    }
                }
            }
        }
    }
    return oSetPlayerData_(thiz, playerinfo);
}

DefineHook(void, AddPlayerInfo, (uintptr_t playerinfo, uint selfCamp, uintptr_t roomdata, bool ignoreEmblem)) {
    if (ndUnlockSkin) {
        if (m_SkinID) {
            if (!oIsHaveSkin(0, m_SkinID) || !oIsHaveSkinForever(0, m_SkinID)) {
                uint64_t m_uiID;
                Il2CppGetStaticFieldValue("Assembly-CSharp.dll", "", "SystemData", "m_uiID", &m_uiID);
                if (*(uint64_t *) ((uintptr_t) playerinfo + BattlePlayerInfo_lUid) == m_uiID) {
                    uint32_t heroid = *(uint32_t *) ((uintptr_t) playerinfo + BattlePlayerInfo_uiSelHero);
                    if (heroid == m_HeroID) {
                        *(uint32_t *) ((uintptr_t) playerinfo + BattlePlayerInfo_uiSkinId) = m_SkinID;
                    }
                }
            }
        }
    }
    return oAddPlayerInfo(playerinfo, selfCamp, roomdata, ignoreEmblem);
}

DefineHook(void, AddPlayerInfo_, (uintptr_t playerinfo, uintptr_t roomdata)) {
    if (ndUnlockSkin) {
        if (m_SkinID) {
            if (!oIsHaveSkin(0, m_SkinID) || !oIsHaveSkinForever(0, m_SkinID)) {
                uint64_t m_uiID;
                Il2CppGetStaticFieldValue("Assembly-CSharp.dll", "", "SystemData", "m_uiID", &m_uiID);
                if (*(uint64_t *) ((uintptr_t) playerinfo + BattlePlayerInfo_lUid) == m_uiID) {
                    uint32_t heroid = *(uint32_t *) ((uintptr_t) playerinfo + BattlePlayerInfo_uiSelHero);
                    if (heroid == m_HeroID) {
                        *(uint32_t *) ((uintptr_t) playerinfo + BattlePlayerInfo_uiSkinId) = m_SkinID;
                    }
                }
            }
        }
    }
    return oAddPlayerInfo_(playerinfo, roomdata);
}
DefineHook(void, OnSetSelfBattlePlayerInfo, (uintptr_t thiz,  uintptr_t playerinfo)) {
    if (ndUnlockSkin) {
        if (m_SkinID) {
            if (!oIsHaveSkin(0, m_SkinID) || !oIsHaveSkinForever(0, m_SkinID)) {
                uint64_t m_uiID;
                Il2CppGetStaticFieldValue("Assembly-CSharp.dll", "", "SystemData", "m_uiID", &m_uiID);
                if (*(uint64_t *) ((uintptr_t) playerinfo + BattlePlayerInfo_lUid) == m_uiID) {
                    uint32_t heroid = *(uint32_t *) ((uintptr_t) playerinfo + BattlePlayerInfo_uiSelHero);
                    if (heroid == m_HeroID) {
                        *(uint32_t *) ((uintptr_t) playerinfo + BattlePlayerInfo_uiSkinId) = m_SkinID;
                    }
                }
            }
        }
    }
    return oOnSetSelfBattlePlayerInfo(thiz, playerinfo);
}
// ====================================================================================================================================
DefineHook(void, CheckHeroDefaultSkin, (uintptr_t thiz, uint32_t heroid ,uint skinid )) {
    if (ndUnlockSkin) {
        return oCheckHeroDefaultSkin(thiz, m_HeroID, m_SkinID);
    }
    m_HeroID = *(uint32_t *) ((uintptr_t) thiz + UIRankHero_ChangeShow_iSelfHero);
    m_SkinID = skinid;
}
// ====================================================================================================================================
DefineHook(void, SendRawData, (void * thiz, uint uiMsgID, void * rawData, int rawDataSize, int eSocketType, int packReliType, bool bNeedLockRequestIndex, int expectedSize)) {
    if (ndUnlockSkin) {
        if (uiMsgID == 1015  || uiMsgID == 1016 || uiMsgID == 1019 || uiMsgID == 1020
            || uiMsgID == 1031  || uiMsgID == 1032  || uiMsgID == 1035 || uiMsgID == 1036
            || uiMsgID == 1160  || uiMsgID == 1161  || uiMsgID == 1162  || uiMsgID == 1163
            || uiMsgID == 1208  || uiMsgID == 10017  || uiMsgID == 10018 || uiMsgID == 10192
            || uiMsgID == 10193 || uiMsgID == 10603  || uiMsgID == 10604 || uiMsgID == 19457
            || uiMsgID == 19458  || uiMsgID == 19459 || uiMsgID == 19460 || uiMsgID == 19156
            || uiMsgID == 19157  || uiMsgID == 19150  || uiMsgID == 19151 || uiMsgID == 19131
            || uiMsgID == 19132 || uiMsgID == 1150  || uiMsgID == 1151  || uiMsgID == 1152
            || uiMsgID == 1153  || uiMsgID == 1154 || uiMsgID == 1155 || uiMsgID == 10103
            || uiMsgID == 10104  || uiMsgID == 19461  || uiMsgID == 19462 || uiMsgID == 11502
            || uiMsgID == 11503  || uiMsgID == 11149 || uiMsgID == 11150 || uiMsgID == 40047
            || uiMsgID == 40048  || uiMsgID == 31859  || uiMsgID == 31860  || uiMsgID == 50087
            || uiMsgID == 50088) {
            return;
        }
        if (uiMsgID == 6) {
        }
    }
    return oSendRawData(thiz, uiMsgID, rawData, rawDataSize, eSocketType, packReliType, bNeedLockRequestIndex, expectedSize);
}

void UnlockSkin() {
    if (g_State.unlockSkinEnabled) {
        ndUnlockSkin = true;
        ndUnlockSkin = true;
        DobbyHook((void *) SystemData_GetHeroSkin, (void *) GetHeroSkin, (void **) &oGetHeroSkin);
        DobbyHook((void *) SystemData_GetMCLimitSkin, (void *) GetMCLimitSkin, (void **) &oGetMCLimitSkin);
        DobbyHook((void *) SystemData_GetHeroHolyStatue, (void *) GetHeroHolyStatue, (void **) &oGetHeroHolyStatue);
        DobbyHook((void *) SystemData_IsCanUseSkin, (void *) IsCanUseSkin, (void **) &oIsCanUseSkin);
        DobbyHook((void *) SystemData_IsHaveSkin, (void *) IsHaveSkin, (void **) &oIsHaveSkin);
        DobbyHook((void *) SystemData_IsHaveSkinForever, (void *) IsHaveSkinForever, (void **) &oIsHaveSkinForever);
        DobbyHook((void *) SystemData_IsHaveStatue, (void *) IsHaveStatue, (void **) &oIsHaveStatue);
        DobbyHook((void *) SystemData_IsHaveStatueForever, (void *) IsHaveStatueForever, (void **) &oIsHaveStatueForever);
        DobbyHook((void *) SystemData_IsForbidStatue, (void *) IsForbidStatue, (void **) &oIsForbidStatue);
        DobbyHook((void *) SystemData_IsForbidSkin, (void *) IsForbidSkin, (void **) &oIsForbidSkin);
        DobbyHook((void *) SystemData_IsForbidARSkin, (void *) IsForbidARSkin, (void **) &oIsForbidARSkin);

        DobbyHook((void *) UIChooseHero_SendSelectSkin, (void *) SendSelectSkin, (void **) &oSendSelectSkin);
        DobbyHook((void *) UIRankHero_BatttleSelectSkin, (void *) BatttleSelectSkin, (void **) &oBatttleSelectSkin);
        DobbyHook((void *) UIRankHero_BatttleSelectSkin2, (void *) BatttleSelectSkin2, (void **) &oBatttleSelectSkin2);

        DobbyHook((void *) SystemData_GetExpiredCardSkinLimitTime, (void *) GetExpiredCardSkinLimitTime, (void **) &oGetExpiredCardSkinLimitTime);
        DobbyHook((void *) SystemData_GetLeaderSkinBForbid, (void *) GetLeaderSkinBForbid, (void **) &oGetLeaderSkinBForbid);
        DobbyHook((void *) UIRankHero_BRankHeroCanUse, (void *) BRankHeroCanUse, (void **) &oBRankHeroCanUse);
        DobbyHook((void *) StarMemberData_ContainSkin, (void *) ContainSkin, (void **) &oContainSkin);
        DobbyHook((void *) UIChooseHero_CheckHeroDefaultSkin, (void *) CheckHeroDefaultSkin, (void **) &oCheckHeroDefaultSkin);


        DobbyHook((void *) UIRankHero_ChangeShow_SendUseSkin, (void *) SendUseSkin, (void **) &oSendUseSkin);
        DobbyHook((void *) BattleReceiveMessage_SetPlayerData, (void *) SetPlayerData, (void **) &oSetPlayerData);
        DobbyHook((void *) BattleReceiveMessage_SetPlayerData_, (void *) SetPlayerData_, (void **) &oSetPlayerData_);
        DobbyHook((void *) GameServerConfig_SendRawData, (void *) SendRawData, (void **) &oSendRawData);
        DobbyHook((void *) ChooseHeroMgr_OnSetSelfBattlePlayerInfo, (void *) OnSetSelfBattlePlayerInfo, (void **) &oOnSetSelfBattlePlayerInfo);

    } else {
        ndUnlockSkin = false;
        ndUnlockSkin = false;
    }
}