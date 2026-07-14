//#pragma once
//#include <Windows.h>
//
//namespace Offsets
//{
//	namespace General
//	{
//		DWORD_PTR
//			IsInGame = 0x00B4B424; // 1.12.1.5875 (Byte)
//	}
//
//	namespace Camera
//	{
//		DWORD_PTR
//			CameraPtr = 0x0074B2BC, // 1.12.1.5875
//			CameraPtrOffset = 0x000065B8, // 1.12.1.5875
//			CameraPosition = 0x00000008, // 1.12.1.5875 (CVec3)
//			CameraFollowingGUID = 0x00000088; // 1.12.1.5875 (GUID)
//	}
//
//	namespace ObjectManager
//	{
//		DWORD_PTR
//			ObjectDescriptors = 0x00000008, // 1.12.1.5875
//			ObjectMovementData = 0x00000118, // 1.12.1.5875 (UInt)
//			ObjectMovementDataPosition = 0x00000010, // 1.12.1.5875 (CVec3)
//			ObjectMovementDataRotation = 0x0000001C, // 1.12.1.5875 (Float)
//			ObjectMovementDataState = 0x000000A0, // 1.12.1.5875 (UInt)
//			ObjectMovementDataFlags = 0x00000040, // 1.12.1.5875 (UInt)
//			ObjectMovementDataRunSpeed = 0x0000008C, // 1.12.1.5875 (Float)
//			PlayerObjectTracking = 0x00002EB0; // 1.12.1.5875 (Byte)
//	}
//
//	namespace Hacks
//	{
//		DWORD_PTR
//			SuperFly = 0x006341BC, // 1.12.1.5875 (Array) - On: { 0x90, 0x90 } - Off: { 0x74, 0x25 }
//			NoFallDamage = 0x007C63DA, // 1.12.1.5875 (Array) - On: { 0x31, 0xC9, 0x90 } - Off: { 0x8B, 0x4F, 0x78 }
//			AntiJump = 0x007C625F, // 1.12.1.5875 (Array) - On: { 0xEB } - Off: { 0x75 }
//			AntiMove = 0x00615CF5, // 1.12.1.5875 (Array) - On: { 0xFE } - Off: { 0xF8 }
//			AntiRoot = 0x006163DB, // 1.12.1.5875 (Array) - On: { 0xEB, 0xF9 } - Off: { 0x8A, 0x47 }
//			InfiniteJump = 0x007C625E, // 1.12.1.5875 (Array) - On: { 0x10 } - Off: { 0x30 }
//			JumpGravity = 0x007C6272, // 1.12.1.5875 (Float)
//			JumpGravityWater = 0x007C6269, // 1.12.1.5875 (Float)
//			WallClimb = 0x0080DFFC, // 1.12.1.5875 (Float)
//			HeartbeatInterval = 0x00615BA7; // 1.12.1.5875 (UInt)  - Speedhack anti-disconnect
//	}
//
//	namespace macros {
//		DWORD_PTR
//			Script_RegisterCVar = 0x488B00,
//			Script_GetCVar = 0x488BA0,
//			Script_SetCVar = 0x488C10,
//			Script_SetCVarDefault = 0x488CF0,
//			Script_SetWorldDetail = 0x488DD0,
//			Script_SetWaterDetail = 0x488ED0,
//			Script_SetFarclip = 0x488F30,
//			Script_SetTerrainMip = 0x488FE0,
//			Script_SetDoodadAnim = 0x489090,
//			Script_SetTexLodBias = 0x489140,
//			Script_SetGamma = 0x4891F0,
//			Script_SetBaseMip = 0x4892B0,
//			Script_SetCursor = 0x489490,
//			Script_EquipCursorItem = 0x489660,
//			Script_TargetUnit = 0x4899D0,
//			Script_AssistUnit = 0x489B80,
//			Script_AssistByName = 0x489C40,
//			Script_TargetByName = 0x489D60,
//			Script_FollowUnit = 0x489E00,
//			Script_FollowByName = 0x489EC0,
//			Script_InitiateTrade = 0x48A120,
//			Script_CanInspect = 0x48A1B0,
//			Script_NotifyInspect = 0x48A310,
//			Script_InviteToParty = 0x48A3B0,
//			Script_InviteByName = 0x48A420,
//			Script_UninviteFromParty = 0x48A510,
//			Script_UninviteFromRaid = 0x48A580,
//			Script_UninviteByName = 0x48A610,
//			Script_PromoteToPartyLeader = 0x48A7C0,
//			Script_PromoteByName = 0x48A830,
//			Script_PickupPlayerMoney = 0x48ABC0,
//			Script_GuildUninviteByName = 0x48AFB0,
//			Script_GuildPromoteByName = 0x48B050,
//			Script_GuildDemoteByName = 0x48B0F0,
//			Script_GuildSetLeaderByName = 0x48B190,
//			Script_GuildSetMOTD = 0x48B270,
//			Script_CheckInteractDistance = 0x48BA00,
//			Script_SetupFullscreenScale = 0x48C270,
//			Script_RandomRoll = 0x48C7B0,
//			Script_NewGMTicket = 0x48C9D0,
//			Script_UpdateGMTicket = 0x48CA60,
//			Script_GMSurveyGetQuestion = 0x48CC00,
//			Script_GMSurveyAnswerSubmit = 0x48CD30,
//			Script_GMSurveyCommentSubmit = 0x48CEF0,
//			Script_GetCoinIcon = 0x48D4E0,
//			Script_DropItemOnUnit = 0x48D960,
//			Script_GetItemQualityColor = 0x48DFB0,
//			Script_GetItemInfo = 0x48E070,
//			Script_GetAddOnInfo = 0x48E390,
//			Script_GetAddOnMetadata = 0x48E530,
//			Script_GetAddOnDependencies = 0x48E5E0,
//			Script_EnableAddOn = 0x48E690,
//			Script_DisableAddOn = 0x48E760,
//			Script_IsAddOnLoadOnDemand = 0x48E840,
//			Script_IsAddOnLoaded = 0x48E8E0,
//			Script_LoadAddOn = 0x48E980,
//			Script_SendChatMessage = 0x49F1E0,
//			Script_SendAddonMessage = 0x49F920,
//			Script_GetLanguageByIndex = 0x49FBE0,
//			Script_DoEmote = 0x49FD30,
//			Script_JoinChannelByName = 0x49FF00,
//			Script_LeaveChannelByName = 0x4A0000,
//			Script_SetGuildRecruitmentMode = 0x4A0060,
//			Script_SetChannelPassword = 0x4A03A0,
//			Script_GetChannelName = 0x4A05E0,
//			Script_GetChatTypeIndex = 0x4A0A80,
//			Script_GetChatWindowInfo = 0x4A0BA0,
//			Script_GetChatWindowMessages = 0x4A0D20,
//			Script_GetChatWindowChannels = 0x4A0DC0,
//			Script_AddChatWindowMessages = 0x4A0E80,
//			Script_RemoveChatWindowMessages = 0x4A0F40,
//			Script_AddChatWindowChannel = 0x4A1000,
//			Script_RemoveChatWindowChannel = 0x4A1260,
//			Script_SetChatWindowName = 0x4A13F0,
//			Script_SetChatWindowSize = 0x4A1470,
//			Script_SetChatWindowColor = 0x4A14F0,
//			Script_SetChatWindowAlpha = 0x4A15D0,
//			Script_SetChatWindowLocked = 0x4A1650,
//			Script_SetChatWindowDocked = 0x4A16B0,
//			Script_SetChatWindowShown = 0x4A1730,
//			Script_GetSavedInstanceInfo = 0x4A1920,
//			Script_GetMapZones = 0x4A7D10,
//			Script_SetMapZoom = 0x4A7DB0,
//			Script_ProcessMapClick = 0x4A7F30,
//			Script_UpdateMapHighlight = 0x4A7FA0,
//			Script_GetPlayerMapPosition = 0x4A8610,
//			Script_GetMapLandmarkInfo = 0x4A8740,
//			Script_GetWorldLocMapPosition = 0x4A88F0,
//			Script_GetMapOverlayInfo = 0x4A8A00,
//			Script_CreateWorldMapArrowFrame = 0x4A8BB0,
//			Script_CreateMiniWorldMapArrowFrame = 0x4A8C60,
//			Script_PositionWorldMapArrowFrame = 0x4A8D20,
//			Script_PositionMiniWorldMapArrowFrame = 0x4A8F20,
//			Script_GetBattlefieldInfo = 0x4AB1F0,
//			Script_JoinBattlefield = 0x4AB290,
//			Script_SetSelectedBattlefield = 0x4AB300,
//			Script_AcceptBattlefieldPort = 0x4AB3B0,
//			Script_GetBattlefieldStatus = 0x4AB4A0,
//			Script_GetBattlefieldPortExpiration = 0x4AB620,
//			Script_GetBattlefieldEstimatedWaitTime = 0x4AB790,
//			Script_GetBattlefieldTimeWaited = 0x4AB820,
//			Script_ShowBattlefieldList = 0x4AB8C0,
//			Script_GetBattlefieldScore = 0x4AB9D0,
//			Script_GetBattlefieldStatInfo = 0x4ABD00,
//			Script_GetBattlefieldStatData = 0x4ABDC0,
//			Script_GetBattlefieldPosition = 0x4ABF90,
//			Script_GetBattlefieldFlagPosition = 0x4AC230,
//			Script_SetSendMailMoney = 0x4AE0F0,
//			Script_SetSendMailCOD = 0x4AE180,
//			Script_GetStationeryInfo = 0x4AE230,
//			Script_SelectStationery = 0x4AE380,
//			Script_GetPackageInfo = 0x4AE450,
//			Script_SelectPackage = 0x4AE550,
//			Script_SendMail = 0x4AE800,
//			Script_GetInboxHeaderInfo = 0x4AEBC0,
//			Script_GetInboxText = 0x4AF110,
//			Script_GetInboxInvoiceInfo = 0x4AF360,
//			Script_GetInboxItem = 0x4AF5D0,
//			Script_TakeInboxMoney = 0x4AF7D0,
//			Script_TakeInboxItem = 0x4AF8E0,
//			Script_TakeInboxTextItem = 0x4AFA60,
//			Script_ReturnInboxItem = 0x4AFBA0,
//			Script_DeleteInboxItem = 0x4AFCD0,
//			Script_InboxItemCanDelete = 0x4AFE00,
//			Script_GetSpellTabInfo = 0x4B3CE0,
//			Script_GetShapeshiftFormInfo = 0x4B45C0,
//			Script_CastShapeshiftForm = 0x4B4810,
//			Script_GetShapeshiftFormCooldown = 0x4B49A0,
//			Script_CastSpellByName = 0x4B4AB0,
//			Script_FlagTutorial = 0x4B59B0,
//			Script_GetBinding = 0x4B7F60,
//			Script_SetBinding = 0x4B8000,
//			Script_GetBindingKey = 0x4B80A0,
//			Script_GetBindingAction = 0x4B8120,
//			Script_RunBinding = 0x4B8180,
//			Script_LoadBindings = 0x4B8220,
//			Script_SaveBindings = 0x4B8260,
//			Script_GetRaidTargetIndex = 0x4BB4B0,
//			Script_GetRaidRosterInfo = 0x4BB560,
//			Script_SetRaidRosterSelection = 0x4BB820,
//			Script_SetRaidSubgroup = 0x4BB990,
//			Script_SwapRaidSubgroup = 0x4BBB00,
//			Script_PromoteToAssistant = 0x4BBD20,
//			Script_DemoteAssistant = 0x4BBDF0,
//			Script_SetRaidTarget = 0x4BBEC0,
//			Script_GetPetActionInfo = 0x4BDC50,
//			Script_GetPetActionCooldown = 0x4BDFA0,
//			Script_IsPetAttackActive = 0x4BE0E0,
//			Script_PickupPetAction = 0x4BE180,
//			Script_TogglePetAutocast = 0x4BE290,
//			Script_CastPetAction = 0x4BE330,
//			Script_ClickTradeButton = 0x4BFDF0,
//			Script_ClickTargetTradeButton = 0x4C0080,
//			Script_GetTradeTargetItemInfo = 0x4C00F0,
//			Script_GetTradeTargetItemLink = 0x4C0360,
//			Script_GetTradePlayerItemInfo = 0x4C0450,
//			Script_GetTradePlayerItemLink = 0x4C0650,
//			Script_PickupTradeMoney = 0x4C0790,
//			Script_SetTradeMoney = 0x4C0820,
//			Script_SetSlot = 0x4C1880,
//			Script_GetLootSlotInfo = 0x4C2C60,
//			Script_GetLootSlotLink = 0x4C2D20,
//			Script_LootSlotIsItem = 0x4C2D90,
//			Script_LootSlotIsCoin = 0x4C2E00,
//			Script_LootSlot = 0x4C2E70,
//			Script_GetMasterLootCandidate = 0x4C2F10,
//			Script_GiveMasterLoot = 0x4C2FD0,
//			Script_GetLootRollItemInfo = 0x4C3050,
//			Script_GetLootRollItemLink = 0x4C31F0,
//			Script_GetLootRollTimeLeft = 0x4C32D0,
//			Script_RollOnLoot = 0x4C3370,
//			Script_ConfirmLootRoll = 0x4C33E0,
//			Script_MakeMinigameMove = 0x4C4D80,
//			Script_GetWorldStateUIInfo = 0x4C5A70,
//			Script_KeyRingButtonIDToInvSlotID = 0x4C8150,
//			Script_GetInventoryItemTexture = 0x4C82A0,
//			Script_GetInventoryItemBroken = 0x4C8590,
//			Script_GetInventoryItemCount = 0x4C8680,
//			Script_GetInventoryItemQuality = 0x4C88D0,
//			Script_GetInventoryItemCooldown = 0x4C8A60,
//			Script_GetInventoryItemLink = 0x4C8C10,
//			Script_SetInventoryPortaitTexture = 0x4C9150,
//			Script_GetGuildInfo = 0x4C9330,
//			Script_GetInventoryAlertStatus = 0x4C94B0,
//			Script_UnstablePet = 0x4CAF20,
//			Script_GetStablePetInfo = 0x4CB230,
//			Script_ClickStablePet = 0x4CB420,
//			Script_PickupStablePet = 0x4CB7A0,
//			Script_SetPetStablePaperdoll = 0x4CB870,
//			Script_CalculateAuctionDeposit = 0x4CE1C0,
//			Script_StartAuction = 0x4CE770,
//			Script_GetNumAuctionItems = 0x4CED20,
//			Script_GetAuctionItemInfo = 0x4CEE40,
//			Script_GetAuctionItemLink = 0x4CF2F0,
//			Script_GetAuctionItemTimeLeft = 0x4CF470,
//			Script_PlaceAuctionBid = 0x4CF610,
//			Script_GetAuctionItemSubClasses = 0x4CF9C0,
//			Script_GetAuctionInvTypes = 0x4CFAB0,
//			Script_SortAuctionItems = 0x4CFC00,
//			Script_SetSelectedAuctionItem = 0x4CFDA0,
//			Script_GetSelectedAuctionItem = 0x4CFEC0,
//			Script_IsAuctionSortReversed = 0x4D0030,
//			Script_SetSelectedAuctionItem = 0x4D0260,
//			Script_GetGuildRosterInfo = 0x4D1200,
//			Script_GetGuildRosterLastOnline = 0x4D14A0,
//			Script_GuildRosterSetPublicNote = 0x4D15E0,
//			Script_GuildRosterSetOfficerNote = 0x4D1700,
//			Script_SetGuildRosterSelection = 0x4D1820,
//			Script_GuildControlGetRankName = 0x4D1E90,
//			Script_GuildControlSetRank = 0x4D1FA0,
//			Script_GuildControlSetRankFlag = 0x4D2070,
//			Script_GuildControlSaveRank = 0x4D20D0,
//			Script_GuildControlAddRank = 0x4D2210,
//			Script_SetGuildInfoText = 0x4D2380,
//			Script_GetSkillLineInfo = 0x4D3610,
//			Script_AbandonSkill = 0x4D3AB0,
//			Script_CollapseSkillHeader = 0x4D3B70,
//			Script_ExpandSkillHeader = 0x4D3BD0,
//			Script_AddSkillUp = 0x4D3C30,
//			Script_RemoveSkillUp = 0x4D3C70,
//			Script_BuySkillTier = 0x4D3E50,
//			Script_SetSelectedSkill = 0x4D4020,
//			Script_StartDuelUnit = 0x4D4C90,
//			Script_GetFactionInfo = 0x4D64F0,
//			Script_FactionToggleAtWar = 0x4D6950,
//			Script_SetFactionInactive = 0x4D69B0,
//			Script_SetFactionActive = 0x4D6A00,
//			Script_CollapseFactionHeader = 0x4D6A50,
//			Script_ExpandFactionHeader = 0x4D6AA0,
//			Script_IsFactionInactive = 0x4D6AF0,
//			Script_SetWatchedFactionIndex = 0x4D6B60,
//			Script_SetSelectedFaction = 0x4D6BB0,
//			Script_GetTrainerServiceInfo = 0x4D8DC0,
//			Script_SelectTrainerService = 0x4D8E60,
//			Script_GetTrainerServiceIcon = 0x4D8F50,
//			Script_GetTrainerServiceSkillLine = 0x4D9160,
//			Script_GetTrainerServiceCost = 0x4D92F0,
//			Script_GetTrainerServiceLevelReq = 0x4D93A0,
//			Script_GetTrainerServiceSkillReq = 0x4D9410,
//			Script_GetTrainerServiceAbilityReq = 0x4D9600,
//			Script_GetTrainerServiceStepReq = 0x4D9930,
//			Script_GetTrainerServiceDescription = 0x4D9B40,
//			Script_IsTrainerServiceSkillStep = 0x4D9DD0,
//			Script_IsTrainerServiceLearnSpell = 0x4D9E70,
//			Script_IsTrainerServiceTradeSkill = 0x4D9F70,
//			Script_GetTrainerServiceStepIncrease = 0x4DA030,
//			Script_BuyTrainerService = 0x4DA210,
//			Script_SetTrainerServiceTypeFilter = 0x4DA260,
//			Script_SetTrainerSkillLineFilter = 0x4DA3F0,
//			Script_GetTrainerServiceTypeFilter = 0x4DA510,
//			Script_GetTrainerSkillLineFilter = 0x4DA590,
//			Script_CollapseTrainerSkillLine = 0x4DA6C0,
//			Script_ExpandTrainerSkillLine = 0x4DA740,
//			Script_TaxiNodeName = 0x4DCB30,
//			Script_TaxiNodeTaxiNodeLocation = 0x4DCB80,
//			Script_TaxiNodeCost = 0x4DCC20,
//			Script_TakeTaxiNode = 0x4DCCA0,
//			Script_TaxiNodeGetType = 0x4DCCF0,
//			Script_TaxiNodeSetCurrent = 0x4DCD40,
//			Script_GetQuestLogTitle = 0x4DF930,
//			Script_SelectQuestLogEntry = 0x4DFAE0,
//			Script_IsUnitOnQuest = 0x4DFE10,
//			Script_GetQuestLogLeaderBoard = 0x4E0110,
//			Script_GetQuestLogRewardInfo = 0x4E0B00,
//			Script_GetQuestLogItemLink = 0x4E0EE0,
//			Script_GetQuestIndexForTimer = 0x4E15F0,
//			Script_CollapseQuestHeader = 0x4E1730,
//			Script_ExpandQuestHeader = 0x4E1780,
//			Script_IsQuestWatched = 0x4E1890,
//			Script_GetQuestIndexForWatch = 0x4E19B0,
//			Script_SelectGossipOption = 0x4E2A30,
//			Script_SelectGossipAvailableQuest = 0x4E2AA0,
//			Script_SelectGossipActiveQuest = 0x4E2AE0,
//			Script_GetPlayerBuff = 0x4E45D0,
//			Script_GetPlayerBuffTexture = 0x4E4740,
//			Script_GetPlayerBuffDispelType = 0x4E4800,
//			Script_GetPlayerBuffTimeLeft = 0x4E48B0,
//			Script_CancelPlayerBuff = 0x4E49A0,
//			Script_GetActionTexture = 0x4E6E10,
//			Script_GetActionCount = 0x4E6E70,
//			Script_GetActionCooldown = 0x4E6ED0,
//			Script_GetActionAutocast = 0x4E6F90,
//			Script_GetActionText = 0x4E7050,
//			Script_HasAction = 0x4E70D0,
//			Script_UseAction = 0x4E7140,
//			Script_PickupAction = 0x4E71D0,
//			Script_PlaceAction = 0x4E7240,
//			Script_IsAttackAction = 0x4E7280,
//			Script_IsCurrentAction = 0x4E72F0,
//			Script_IsAutorepeatAction = 0x4E7360,
//			Script_IsUsableAction = 0x4E73D0,
//			Script_IsConsumableAction = 0x4E7470,
//			Script_IsEquippedAction = 0x4E74E0,
//			Script_GetPartyMember = 0x4E9090,
//			Script_SetLootMethod = 0x4E92A0,
//			Script_SetLooThreshold = 0x4E9500,
//			Script_LFGQuery = 0x4E9760,
//			Script_GetLFGResults = 0x4E98C0,
//			Script_GetLFGTypeEntries = 0x4E9AA0,
//			Script_% s:SetMaskTexture = 0x4EE4A0,
//			Script_% s : SetIconTexture = 0x4EE640,
//			Script_% s : SetBlipTexture = 0x4EE7E0,
//			Script_% s : SetArrowModel = 0x4EE980,
//			Script_% s : SetPlayerModel = 0x4EEAB0,
//			Script_SetZoom = 0x4EEC60,
//			Script_CreateMacro = 0x4F15B0,
//			Script_GetMacroInfo = 0x4F1760,
//			Script_DeleteMacro = 0x4F1850,
//			Script_EditMacro = 0x4F18B0,
//			Script_GetMacroIconInfo = 0x4F1A30,
//			Script_PickupMacro = 0x4F1AE0,
//			Script_GetMacroIndexByName = 0x4F1B30,
//			Script_GetTalentTabInfo = 0x4F3040,
//			Script_GetNumTalents = 0x4F3160,
//			Script_GetTalentInfo = 0x4F3200,
//			Script_GetTalentPrereqs = 0x4F34D0,
//			Script_LearnTalent = 0x4F36A0,
//			Script_GetPetitionNameInfo = 0x4F4510,
//			Script_BuyGuildCharter = 0x4F5260,
//			Script_GetCraftInfo = 0x4F6E90,
//			Script_SelectCraft = 0x4F70F0,
//			Script_GetTradeSkillIcon = 0x4F7160,
//			Script_GetCraftSkillLine = 0x4F7210,
//			Script_GetCraftItemLink = 0x4F72A0,
//			Script_GetCraftNumReagents = 0x4F7420,
//			Script_GetCraftReagentInfo = 0x4F74D0,
//			Script_GetCraftReagentItemLink = 0x4F7730,
//			Script_GetCraftSpellFocus = 0x4F78B0,
//			Script_GetCraftDescription = 0x4F7A90,
//			Script_CollapseCraftSkillLine = 0x4F7C10,
//			Script_ExpandCraftSkillLine = 0x4F7C90,
//			Script_DoCraft = 0x4F7D10,
//			Script_BankButtonIDToInvSlotID = 0x4F8530,
//			Script_ContainerIDToInventoryID = 0x4F94E0,
//			Script_GetContainerNumSlots = 0x4F9560,
//			Script_GetContainerItemInfo = 0x4F9670,
//			Script_GetContainerItemLink = 0x4F9930,
//			Script_GetContainerItemCooldown = 0x4F99B0,
//			Script_PickupContainerItem = 0x4F9B30,
//			Script_SplitContainerItem = 0x4F9F70,
//			Script_UseContainerItem = 0x4FA0E0,
//			Script_ShowContainerSellCursor = 0x4FA460,
//			Script_SetBagPortaitTexture = 0x4FA4F0,
//			Script_GetBagName = 0x4FA670,
//			Script_GetMerchantItemInfo = 0x4FB150,
//			Script_GetBuybackItemInfo = 0x4FB310,
//			Script_GetMerchantItemLink = 0x4FB580,
//			Script_GetMerchantItemMaxStack = 0x4FB670,
//			Script_BuyMerchantItem = 0x4FB850,
//			Script_BuybackItem = 0x4FB950,
//			Script_ShowMerchantSellCursor = 0x4FBAB0,
//			Script_ShowBuybackSellCursor = 0x4FBBB0,
//			Script_GetTradeSkillInfo = 0x4FD820,
//			Script_SelectTradeSkill = 0x4FD9B0,
//			Script_GetTradeSkillCooldown = 0x4FDA20,
//			Script_GetTradeSkillNumMade = 0x4FDC50,
//			Script_GetTradeSkillItemStats = 0x4FDEC0,
//			Script_GetTradeSkillItemLink = 0x4FF410,
//			Script_GetTradeSkillNumReagents = 0x4FF510,
//			Script_GetTradeSkillReagentInfo = 0x4FF5C0,
//			Script_GetTradeReagentSkillItemLink = 0x4FF800,
//			Script_GetTradeSkillTools = 0x4FF980,
//			Script_SetTradeSkillSubClassFilter = 0x4FFC70,
//			Script_GetTradeSkillSubClassFilter = 0x4FFD90,
//			Script_SetTradeSkillInvSlotFilter = 0x4FFE60,
//			Script_GetTradeSkillInvSlotFilter = 0x4FFFD0,
//			Script_CollapseTradeSkillSubClass = 0x5000C0,
//			Script_ExpandTradeSkillSubClass = 0x500140,
//			Script_DoTradeSkill = 0x500280,
//			Script_GetAvailableTitle = 0x501AC0,
//			Script_GetActiveTitle = 0x501B30,
//			Script_GetGetAvailableLevel = 0x501BA0,
//			Script_GetGetActiveLevel = 0x501C20,
//			Script_SelectAvailableQuest = 0x501CA0,
//			Script_SelectActiveQuest = 0x501CE0,
//			Script_GetQuestItemLink = 0x502090,
//			Script_CycleVariation = 0x502CA0,
//			Script_%s:GetUpperEmblemTexture = 0x503160,
//			Script_%s : GetLowerEmblemTexture = 0x503540,
//			Script_SetUnit = 0x505D70,
//			Script_SetRotation = 0x505F00;
//	}
//}
//
//class WowHelper
//{
//};
//


#pragma once


#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <sstream> // 用于构建动态字符串

// 智能指针，自动关闭 Windows 句柄
struct HandleDeleter {
    void operator()(HANDLE handle) const {
        if (handle != nullptr && handle != INVALID_HANDLE_VALUE) {
            CloseHandle(handle);
        }
    }
};
using UniqueHandle = std::unique_ptr<void, HandleDeleter>;

// 进程管理类
class ProcessManager {
public:
    // 构造函数：根据进程名查找并打开进程
    ProcessManager(const wchar_t* procName) {
        m_pid = GetProcessIdByName(procName);
        if (m_pid == 0) {
            throw std::runtime_error("Failed to find process ID."); // 找不到进程ID时抛出异常
        }
        std::cout << "找到目标进程 PID = " << m_pid << std::endl;

        // 打开进程句柄，并使用UniqueHandle自动管理
        m_hProcess = UniqueHandle(OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_pid));
        if (!m_hProcess) {
            throw std::runtime_error("Failed to open process handle."); // 无法打开进程句柄时抛出异常
        }
    }

    // 构造函数：根据进程名查找并打开进程
    ProcessManager(const DWORD pid) {
        m_pid = pid;
        std::cout << "找到目标进程 PID = " << m_pid << std::endl;

        // 打开进程句柄，并使用UniqueHandle自动管理
        m_hProcess = UniqueHandle(OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid));
        if (!m_hProcess) {
            throw std::runtime_error("Failed to open process handle."); // 无法打开进程句柄时抛出异常
        }
    }


    // 获取进程ID
    DWORD GetPid() const {
        return m_pid;
    }

    // 获取进程句柄
    HANDLE GetHandle() const {
        return m_hProcess.get();
    }

    // 静态方法：通过进程名查找进程ID
    static DWORD GetProcessIdByName(const wchar_t* procName) {
        PROCESSENTRY32W pe = { sizeof(pe) };
        UniqueHandle hSnap(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)); // 使用UniqueHandle管理快照句柄
        if (!hSnap) return 0;

        if (Process32FirstW(hSnap.get(), &pe)) {
            do {
                if (_wcsicmp(pe.szExeFile, procName) == 0) {
                    return pe.th32ProcessID;
                }
            } while (Process32NextW(hSnap.get(), &pe));
        }
        return 0;
    } 
     

    // 从进程内存中读取指定类型的数据
    template <typename T>
    T ReadMemory(uintptr_t address) {
        T buffer{};
        SIZE_T bytesRead = 0;
        if (!ReadProcessMemory(m_hProcess.get(), reinterpret_cast<LPCVOID>(address), &buffer, sizeof(T), &bytesRead) || bytesRead != sizeof(T)) {
            std::cerr << "读取内存失败，错误码: " << GetLastError() << std::endl;
            return T{}; // 返回默认构造的值
        }
        return buffer;
    }

    // 从进程内存中读取字符串
    std::string ReadString(uintptr_t address, size_t maxLength = 256) {
        std::vector<char> buffer(maxLength, 0); // 使用vector动态分配缓冲区
        SIZE_T bytesRead = 0;
        if (!ReadProcessMemory(m_hProcess.get(), reinterpret_cast<LPCVOID>(address), buffer.data(), maxLength, &bytesRead)) {
            std::cerr << "读取内存失败，错误码: " << GetLastError() << std::endl;
            return "";
        }
        // 确保字符串以空字符结尾，防止读取到垃圾数据
        buffer[bytesRead < maxLength ? bytesRead : maxLength - 1] = '\0';
        return std::string(buffer.data());
    }

private:
    DWORD m_pid = 0; // 进程ID
    UniqueHandle m_hProcess; // 进程句柄，使用智能指针管理
};

// 命名管道客户端类
class PipeClient {
public:
    PipeClient(const std::string& pipeName) : m_pipeName(pipeName) {}

    // 连接管道并读取数据
    bool ReadData(uint64_t& data) {
        std::cout << "尝试连接管道...\n";
        UniqueHandle hPipe; // 使用智能指针管理管道句柄
        for (int i = 0; i < 10; ++i) { // 尝试连接多次
            hPipe = UniqueHandle(CreateFileA(
                m_pipeName.c_str(),
                GENERIC_READ,
                0,
                nullptr,
                OPEN_EXISTING,
                0,
                nullptr));

            if (hPipe) break; // 连接成功则跳出循环
            Sleep(500); // 否则等待500毫秒后重试
        }

        if (!hPipe) {
            std::cerr << "管道连接失败，请确认DLL注入并创建了管道。\n";
            return false;
        }

        DWORD bytesRead = 0;
        if (!ReadFile(hPipe.get(), &data, sizeof(data), &bytesRead, nullptr) || bytesRead != sizeof(data)) {
            std::cerr << "读取管道数据失败\n";
            return false;
        }

        std::cout << "收到玩家 GUID: " << data << std::endl;
        return true;
    }
    // 连接管道并读取数据
    bool readDataU8(std::string& data) {
        std::cout << "尝试连接管道...\n";
        UniqueHandle hPipe;

        char buffer[1024] = { 0 };

        for (int i = 0; i < 10; ++i) {
            hPipe = UniqueHandle(CreateFileA(
                m_pipeName.c_str(),
                GENERIC_READ,
                0,
                nullptr,
                OPEN_EXISTING,
                0,
                nullptr));

            if (hPipe) break;
            Sleep(500);
        }

        if (!hPipe) {
            std::cerr << "管道连接失败，请确认DLL注入并创建了管道。\n";
            return false;
        }

        DWORD bytesRead = 0;

        BOOL success = ReadFile(
            hPipe.get(),
            buffer,
            sizeof(buffer),
            &bytesRead,
            NULL);

        if (success && bytesRead > 0) {
            data.assign(buffer, bytesRead); // 安全赋值，不依赖 null
            std::cout << "收到数据（UTF-8）: " << data << std::endl;
            return true;
        }
        else {
            std::cerr << "读取管道失败，错误码: " << GetLastError() << std::endl;
            return false;
        }
    }




private:
    std::string m_pipeName; // 管道名称
};

std::string WoWGetPlayerName(DWORD pid);




const std::string pipeName_KylePlayerGuidPipe = "KylePlayerGuidPipe"; // 命名管道名称
const std::string pipeName_KylePlayerNamePipe = "KylePlayerNamePipe"; // 命名管道名称

