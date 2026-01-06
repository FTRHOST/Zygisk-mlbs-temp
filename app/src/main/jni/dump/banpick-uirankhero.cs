// Assembly-CSharp.dll
public enum ChooseState : 
{
	// Fields
	public System.Int32 value__; // 0x10
	public const ChooseState Unknow = 0; // 0x0
	public const ChooseState Road = 1; // 0x0
	public const ChooseState Ban = 2; // 0x0
	public const ChooseState Pick = 3; // 0x0
	public const ChooseState ChangeHero = 4; // 0x0

	// Methods
}

// Assembly-CSharp.dll
public class UIRankHero : UIBasePanel, IUIMgrParams
{
	// Fields
	private UI_ChooseHeroBPUI m_FrameUI; // 0x180
	private const System.UInt32 BadHeroLineups_Time; // 0x0
	private readonly System.String fpsRecordKey; // 0x188
	private System.UInt32 iBanTimeSpan; // 0x190
	private System.UInt32 iSecondBanTimeSpan; // 0x194
	private System.UInt32 iPickTimeSpan; // 0x198
	private System.UInt32 iBanTwoHeroSpan; // 0x19c
	private System.UInt32 iPreSelectRoadTimeSpan; // 0x1a0
	private System.UInt32 iFirstSelectTimeSpan; // 0x1a4
	private static System.Boolean _bRobotBanPick; // 0x0
	private System.UInt32 iChangeHeroTimeSpan; // 0x1a8
	public static readonly System.UInt32 iShowHeroTime; // 0x4
	public static System.UInt64 iMyRoomId; // 0x8
	private System.UInt32 iRestartBattleTimeSpan; // 0x1ac
	private System.UInt32 m_bpspendtime; // 0x1b0
	private System.Boolean m_bOpenBanPickCtrl; // 0x1b4
	private readonly System.Collections.Generic.Dictionary<System.UInt64,System.Collections.Generic.Dictionary<System.UInt32,System.UInt32>> m_HeroSkinDict; // 0x1b8
	private System.Collections.Generic.Dictionary<System.UInt32,UIRankHero.BanInfo> banList; // 0x1c0
	private System.Collections.Generic.Dictionary<System.UInt32,UIRankHero.SelectInfo> pickList; // 0x1c8
	private System.Collections.Generic.List<System.UInt32> totalBanOrder; // 0x1d0
	private System.Collections.Generic.List<System.UInt32> banOrder; // 0x1d8
	private System.Collections.Generic.List<System.UInt32> pickOrder; // 0x1e0
	private System.Collections.Generic.List<System.UInt32> secondBanOrder; // 0x1e8
	private System.Collections.Generic.List<System.UInt32> secondPickOrder; // 0x1f0
	private static System.Collections.Generic.List<System.UInt32> LeaderQueue; // 0x10
	private System.Boolean _bIsShowBpEffect; // 0x1f8
	private System.Boolean bInit; // 0x1f9
	private System.Boolean _bHideHeroList; // 0x1fa
	public HeroRecordChatComp heroRecordChatComp; // 0x200
	private UIRankHero.RankingHeroShow rankingShow; // 0x208
	private UIRankHero.HeroListShow heroListShow; // 0x210
	private UIRankHero.ChooserTalk chooserTalk; // 0x218
	private UIRankHero.ChooserUIData chooserUIData; // 0x220
	private UIRankHero.BanShow banShow; // 0x228
	private UIRankHero.TimeShow timeShow; // 0x230
	private UIRankHero.ChangeShow changeShow; // 0x238
	private UIBattleInfo.TimeDelegate timerEvent; // 0x240
	private UIRankHero.ProtoSaveMgr protoSaveMgr; // 0x248
	private ShowUIEffect m_M3inspireEffect; // 0x250
	private UnityEngine.GameObject appearEff01; // 0x258
	private UnityEngine.GameObject appearEff02; // 0x260
	private UIRankHero.SelShowTab _lastShow; // 0x268
	private UnityEngine.Coroutine m_TeamStateCoroutine; // 0x270
	private UnityEngine.Coroutine m_CloseLaneCoroutine; // 0x278
	private UnityEngine.Coroutine m_BanTogetherToPickCoroutine; // 0x280
	private UnityEngine.Coroutine m_BanTipsShowCoroutine; // 0x288
	private UnityEngine.Coroutine m_SkillTipsCoroutine; // 0x290
	private UnityEngine.Coroutine _BpEffcorount; // 0x298
	private UnityEngine.Coroutine m_DelayCloseLaneCoroutine; // 0x2a0
	private UnityEngine.Coroutine m_DelayShowHeroListShow; // 0x2a8
	private SkinDyeingGroup dyeingGroup; // 0x2b0
	public VoiceObjectData m_VoiceData; // 0x2b8
	private System.Collections.Generic.List<UnityEngine.GameObject> m_skillIcon; // 0x2c0
	private System.Collections.Generic.List<UnityEngine.GameObject> m_skillISelectcon; // 0x2c8
	private System.Collections.Generic.List<UISprite> m_skillSpriteList; // 0x2d0
	public System.Collections.Generic.Dictionary<System.Int32,UnityEngine.GameObject[]> m_skillInfoDic; // 0x2d8
	private System.Collections.Generic.List<UnityEngine.GameObject> m_skillFlagList; // 0x2e0
	private System.Collections.Generic.Dictionary<UnityEngine.GameObject,UnityEngine.Coroutine> m_aniFrameUpdates; // 0x2e8
	private System.Int32 m_chatShortcutsBgInitHeight; // 0x2f0
	private System.Int32 m_chatShortcutsBgPadding; // 0x2f4
	public System.Int32 m_selectSkillIndex; // 0x2f8
	public System.Int32 m_iSkillTransStep; // 0x2fc
	private UnityEngine.Vector3 m_chatShortcutsBgInitPosition; // 0x300
	private UnityEngine.Vector3 m_chatShortcutsBgJustHeroRecordPosition; // 0x30c
	public System.Int32 favoriteHeroWaitPlayEffectHeroID; // 0x318
	public ShowUIEffect m_FavHeroEffect; // 0x320
	public UIVideo m_videoPlayer; // 0x328
	private System.Boolean m_chatShortcutsBgRepositioned; // 0x330
	private System.UInt32 m_DeltaServerTime; // 0x334
	private System.Int32 m_TotalSpanTime; // 0x338
	private System.UInt32 _startPreSelectRoadTime; // 0x33c
	private System.UInt32 _startBanTime; // 0x340
	private System.UInt32 _startSelectTime; // 0x344
	private System.UInt32 _startExChangeTime; // 0x348
	private System.Boolean _bUseTrail; // 0x34c
	private System.UInt32 iMyCamp; // 0x350
	private static System.Single _lastShowWinningTime; // 0x18
	private static System.Single _lastChangeShowWinningTime; // 0x1c
	private static System.Single _lastShortCutTime; // 0x20
	public System.Boolean bInspireLimit; // 0x354
	public System.Boolean bHasChooseLaneAdd; // 0x355
	public System.UInt32 selectHeroID; // 0x358
	public System.UInt32 selectSkinID; // 0x35c
	public System.UInt32 _selectSkinIDForBegShare; // 0x360
	public System.UInt32 prevSelectHeroID; // 0x364
	private System.Boolean _bPreLoadTopSkinUI; // 0x368
	private System.Int32 _chooseEmblemStep; // 0x36c
	private System.Int32 _chooseSkillStep; // 0x370
	private System.Collections.Generic.List<System.Int32> _clickHeros; // 0x378
	private System.Collections.Generic.List<System.Int32> _clickSkins; // 0x380
	public System.Collections.Generic.List<MTTDProto.Battle_TeammateChuochuoSkin> m_TeammateChuoChuoSkinList; // 0x388
	private System.Boolean <m_bTurboLoadLuaMagic>k__BackingField; // 0x390
	public System.Single m_MessagePanel_initY; // 0x394
	public System.Single m_MessagePanel_flipY; // 0x398
	private SkinDyeingGroup m_DyeingGroup; // 0x3a0
	private UnityEngine.Coroutine selectTipsCo; // 0x3a8
	private ShowUIEffect bpEffect; // 0x3b0
	private System.Single m_eventDuration; // 0x3b8
	private System.Boolean disappear; // 0x3bc
	private UnityEngine.GameObject banEffect; // 0x3c0
	private UnityEngine.GameObject miniMapEffect; // 0x3c8
	public CData_Hero_Element m_heroCfg; // 0x3d0
	private System.Int32 m_skinId; // 0x3d8
	private System.Collections.Generic.List<UnityEngine.GameObject> m_PlayerLaneItems; // 0x3e0
	private System.Collections.Generic.Dictionary<System.Int32,UnityEngine.GameObject> ChatShortItems; // 0x3e8
	private System.Collections.Generic.Dictionary<System.Int32,Activity.ChatShortData> ChatShortcuts; // 0x3f0
	private System.Collections.Generic.Dictionary<System.UInt64,System.Collections.Generic.Dictionary<System.Int32,Activity.ChatShortData>> AIChatShortcuts; // 0x3f8
	private System.String Sure_Check_Road_Invaild_Key; // 0x400
	private UnityEngine.GameObject reamStateBanEff; // 0x408
	private System.String mRoadConflict; // 0x410
	private UnityEngine.Color redColor; // 0x418
	private UnityEngine.GameObject miniMapRepeatEff; // 0x428
	public System.UInt32 m_StopTotalTime; // 0x430
	public System.UInt32 m_TotalTime; // 0x434
	public System.UInt32 m_LastEBanPickTime; // 0x438
	private System.Collections.Generic.Dictionary<System.String,System.String> _tanslateCache; // 0x440
	private UIToggle _autoTranslateToggle; // 0x448
	private UnityEngine.GameObject banLightBGEffect; // 0x450
	private UIGrid m_RecommendGrid; // 0x458
	private System.Collections.Generic.List<UnityEngine.GameObject> recommendHeroGoList; // 0x460
	private UnityEngine.GameObject m_recommendPrefab; // 0x468
	private UnityEngine.Coroutine autoPickCoroutine; // 0x470
	private UnityEngine.Coroutine autoBanCoroutine; // 0x478
	private UnityEngine.Coroutine waitCustomEnd; // 0x480
	private System.Int32 m_disagreeRestartCount; // 0x488
	private const System.String FX_UI_ChooseHeroBP1_add; // 0x0
	private System.Collections.Generic.List<System.UInt32> m_roadRepeats; // 0x490
	private UnityEngine.Coroutine m_CoDisplayShareSkin; // 0x498
