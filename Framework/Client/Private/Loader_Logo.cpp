#include "stdafx.h"
#include "..\Public\Loader_Logo.h"
#include "GameInstance.h"

#include "Quest.h"
#include "Order.h"

/* For.UI */
#include "UI_Logo.h"
#include "UI_Logo_Back.h"
#include "UI_Logo_Start.h"
#include "UI_Main_Icon.h"
#include "UI_Menu_Button.h"
#include "UI_Menu_Back.h"
#include "UI_Shortkey.h"
#include "UI_Dash_Bar.h"
#include "UI_Player_Hp_Bar.h"
#include "UI_Player_Hp_Back.h"
#include "UI_Main_Button.h"
#include "UI_Skill_Gauge.h"
#include "UI_Skill_Enable.h"
#include "UI_Stamina_Bar.h"
#include "UI_Stamina_Back.h"
#include "UI_Inven_Button.h"
#include "UI_Inven_Icon.h"
#include "UI_Weapon_Skill.h"
#include "UI_Skill_M.h"
#include "UI_Skill_L.h"
#include "UI_Skill_R.h"
#include "UI_Food_Window.h"
#include "UI_Boss_Hp_Bar.h"
#include "UI_Boss_Name.h"
#include "UI_Boss_Hp_Back.h"
#include "UI_Damage_Num.h"
#include "UI_Guide_Window.h"
#include "UI_Guide_Hand_Anim.h"
#include "UI_Food_Icon.h"
#include "UI_Item.h"
#include "UI_CurFood_Icon.h"
#include "UI_Food_Num.h"
#include "UI_CurFood_Num.h"
#include "UI_Attacked_Num.h"
#include "UI_Heal_Num.h"
#include "UI_Item_Num.h"
#include "UI_Boss_Hp_Lerp.h"
#include "UI_Quest_Mark.h"
#include "UI_Gun_CrossHair.h"
#include "UI_Guide_Talk_Anim.h"
#include "UI_Alert_Window.h"
#include "UI_Alert_Icon.h"
#include "UI_Talk_Window.h"
#include "UI_Proceeding.h"
#include "UI_PlaceMark.h"
#include "UI_Dist_Num.h"
#include "UI_Quest_Notice.h"
#include "UI_Attack_Target.h"
#include "UI_Item_Detail.h"
#include "UI_Cook_Button.h"
#include "UI_Cook_Icon.h"
#include "UI_Cook_Food.h"
#include "UI_Cook_Ingredient.h"
#include "UI_Cook_FoodSelect.h"
#include "UI_Cook_Detail.h"
#include "UI_Cook_IngrdNum.h"
#include "UI_Equip_Button.h"
#include "UI_Equip_Weapon_M.h"
#include "UI_Equip_Weapon_L.h"
#include "UI_Equip_Weapon_R.h"
#include "UI_Equip_Icon.h"
#include "UI_Equip_Detail.h"
#include "UI_Cook_GaugeBar.h"
#include "UI_Cook_GaugeNum.h"
#include "UI_Skill_Time.h"
#include "UI_Equip_Remove.h"
#include "UI_Order_Icon.h"
#include "UI_Order_Button.h"
#include "UI_Order_Anim.h"
#include "UI_Order_Item.h"
#include "UI_Order_Result.h"
#include "UI_Order_ResultButton.h"
#include "UI_Order_ItemName.h"
#include "UI_Order_TenResult.h"
#include "UI_PlaceName.h"
#include "UI_Energy_Gear.h"
#include "UI_Minimap_Arrow.h"
#include "UI_Minimap.h"
#include "UI_Quest_Complete.h"
#include "UI_Minigame_Start.h"
#include "UI_Minigame_Notice.h"
#include "UI_Minigame_Explain.h"
#include "UI_Minigame_Gain.h"
#include "UI_Minigame_Result.h"
#include "UI_Crystal_Hp_Bar.h"
#include "UI_Fin.h"


/* For.UISolid*/
#include "UI_Enemy_Hp_Bar.h"
#include "UI_Enemy_Name.h"
#include "UI_Enemy_Indicator.h"
#include "UI_Enemy_Rank.h"
#include "UI_Enemy_Attack.h"


CLoader_Logo::CLoader_Logo()
{
}

HRESULT CLoader_Logo::Initialize()
{
	return S_OK;
}

HRESULT CLoader_Logo::Loading_Object()
{
	USEGAMEINSTANCE;
	FAILEDCHECK(Loading_UI_Object());


	///* For.Prototype_GameObject_Logo */
	//if (FAILED(pGameInstance->Add_GameObject_Prototype(ID_UINT(LOGO),
	//	CLogo::Create())))
	//	__debugbreak();

	// MakeSpriteFont "폰트이름" /FontSize:32 /FastPack /CharacterRegion:0x0020-0x00FF /CharacterRegion:0x3131-0x3163 /CharacterRegion:0xAC00-0xD800 /DefaultCharacter:0xAC00 출력파일이름.spritefont
	//wsprintf(m_szFPS, TEXT("에프피에스 : %d"), m_iNumRender);
	//pGameInstance->Render_Font(TEXT("Font_NotoSans_Black"), TEXT("Loding"), _float2(0.f, 0.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), 0.7f);

	return S_OK;
}

HRESULT CLoader_Logo::Loading_Texture()
{
	USEGAMEINSTANCE;

	FAILEDCHECK(Loading_UI_Texture());
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(QUEST), CQuest::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(ORDER), COrder::Create()));

	///* For.Prototype_Component_Texture_Default */
	//if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_LOGO, ID_UINT(TEXTURE, TEXTURE_DEFAULT),
	//	CTexture::Create(TEXT("../Bin/Resources/Textures/Default%d.jpg"), 2))))
	//	__debugbreak();

	///* For.Prototype_Component_Texture_BackGround */
	//if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_LOGO, ID_UINT(TEXTURE, TEXTURE_BACKGROUND),
	//	CTexture::Create(TEXT("../Bin/Resources/Textures/BackGround/BackGround%d.png"), 3))))
	//	__debugbreak();

	///* For.Prototype_Component_Texture_Logo */
	//if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_LOGO, ID_UINT(TEXTURE, TEXTURE_LOGO),
	//	CTexture::Create(TEXT("../Bin/Resources/Textures/Logo/Logo%d.png"), 1))))
	//	__debugbreak();

	return S_OK;
}

HRESULT CLoader_Logo::Loading_Model()
{
	USEGAMEINSTANCE;


	return S_OK;
}

HRESULT CLoader_Logo::Loading_Shader()
{
	USEGAMEINSTANCE;

	//_uint iTemp;
	//iTemp = ID_UINT(UI, UI_SKILL_ENABLE).iID;

	return S_OK;
}

HRESULT CLoader_Logo::Loading_UI_Object()
{
	USEGAMEINSTANCE;
#if defined(_UI)
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_LOGO), CUI_Logo::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_LOGO_BACK), CUI_Logo_Back::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_LOGO_START), CUI_Logo_Start::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_SHORTKEY), CUI_Shortkey::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_MAIN_ICON), CUI_Main_Icon::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_MENU_BUTTON), CUI_Menu_Button::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_MENU_BACK), CUI_Menu_Back::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_DASH_BAR), CUI_Dash_Bar::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_PLAYER_HP_BAR), CUI_Player_Hp_Bar::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_PLAYER_HP_BACK), CUI_Player_Hp_Back::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_MAIN_BUTTON), CUI_Main_Button::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_SKILL_GAUGE), CUI_Skill_Gauge::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_SKILL_ENABLE), CUI_Skill_Enable::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_STAMINA_BAR), CUI_Stamina_Bar::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_STAMINA_BACK), CUI_Stamina_Back::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_INVEN_BUTTON), CUI_Inven_Button::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_INVEN_ICON), CUI_Inven_Icon::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_WEAPON_SKILL), CUI_Weapon_Skill::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_SKILL_R), CUI_Skill_R::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_SKILL_L), CUI_Skill_L::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_SKILL_M), CUI_Skill_M::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_FOOD_WINDOW), CUI_Food_Window::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_BOSS_HP_BAR), CUI_Boss_Hp_Bar::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_BOSS_NAME), CUI_Boss_Name::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_BOSS_HP_BACK), CUI_Boss_Hp_Back::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_DAMAGE_NUM), CUI_Damage_Num::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_GUIDE_WINDOW), CUI_Guide_Window::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_GUIDE_HAND_ANIM), CUI_Guide_Hand_Anim::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_FOOD_ICON), CUI_Food_Icon::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_ITEM), CUI_Item::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_CURFOOD_ICON), CUI_CurFood_Icon::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_FOOD_NUM), CUI_Food_Num::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_CURFOOD_NUM), CUI_CurFood_Num::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_ATTACKED_NUM), CUI_Attacked_Num::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_HEAL_NUM), CUI_Heal_Num::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_ITEM_NUM), CUI_Item_Num::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_BOSS_HP_LERP), CUI_Boss_Hp_Lerp::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_QUEST_MARK), CUI_Quest_Mark::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_GUN_CROSSHAIR), CUI_Gun_CrossHair::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_GUIDE_TALK_ANIM), CUI_Guide_Talk_Anim::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_ALERT_WINDOW), CUI_Alert_Window::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_ALERT_ICON), CUI_Alert_Icon::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_TALK_WINDOW), CUI_Talk_Window::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_PROCEEDING), CUI_Proceeding::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_PLACEMARK), CUI_PlaceMark::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_DIST_NUM), CUI_Dist_Num::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_QUEST_NOTICE), CUI_Quest_Notice::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_ATTACK_TARGET), CUI_Attack_Target::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_ITEM_DETAIL), CUI_Item_Detail::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_COOK_BUTTON), CUI_Cook_Button::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_COOK_ICON), CUI_Cook_Icon::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_COOK_FOOD), CUI_Cook_Food::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_COOK_INGREDIENT), CUI_Cook_Ingredient::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_COOK_FOODSELECT), CUI_Cook_FoodSelect::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_COOK_DETAIL), CUI_Cook_Detail::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_COOK_INGRDNUM), CUI_Cook_IngrdNum::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_EQUIP_BUTTON), CUI_Equip_Button::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_EQUIP_WEAPON_L), CUI_Equip_Weapon_L::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_EQUIP_WEAPON_M), CUI_Equip_Weapon_M::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_EQUIP_WEAPON_R), CUI_Equip_Weapon_R::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_EQUIP_ICON), CUI_Equip_Icon::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_EQUIP_DETAIL), CUI_Equip_Detail::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_COOK_GAUGENUM), CUI_Cook_GaugeNum::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_COOK_GAUGEBAR), CUI_Cook_GaugeBar::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_SKILL_TIME), CUI_Skill_Time::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_EQUIP_REMOVE), CUI_Equip_Remove::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_ORDER_ICON), CUI_Order_Icon::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_ORDER_BUTTON), CUI_Order_Button::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_ORDER_ANIM), CUI_Order_Anim::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_ORDER_ITEM), CUI_Order_Item::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_ORDER_RESULT), CUI_Order_Result::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_ORDER_RESULTBUTTON), CUI_Order_ResultButton::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_ORDER_ITEMNAME), CUI_Order_ItemName::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_ORDER_TENRESULT), CUI_Order_TenResult::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_PLACENAME), CUI_PlaceName::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_ENERGY_GEAR), CUI_Energy_Gear::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_MINIMAP_ARROW), CUI_Minimap_Arrow::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_MINIMAP), CUI_Minimap::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_QUEST_COMPLETE), CUI_Quest_Complete::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_MINIGAME_START), CUI_Minigame_Start::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_MINIGAME_NOTICE), CUI_Minigame_Notice::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_MINIGAME_EXPLAIN), CUI_Minigame_Explain::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_MINIGAME_GAIN), CUI_Minigame_Gain::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_MINIGAME_RESULT), CUI_Minigame_Result::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_CRYSTAL_HP_BAR), CUI_Crystal_Hp_Bar::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UI, UI_FIN), CUI_Fin::Create()));


	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UISOLID, UISOLID_ENEMY_HP_BAR), CUI_Enemy_Hp_Bar::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UISOLID, UISOLID_ENEMY_NAME), CUI_Enemy_Name::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UISOLID, UISOLID_ENEMY_INDICATOR), CUI_Enemy_Indicator::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UISOLID, UISOLID_ENEMY_RANK), CUI_Enemy_Rank::Create()));
	FAILEDCHECK(pGameInstance->Add_GameObject_Prototype(ID_UINT(UISOLID, UISOLID_ENEMY_ATTACK), CUI_Enemy_Attack::Create()));
#endif

	return S_OK;
}

HRESULT CLoader_Logo::Loading_UI_Texture()
{
#define UIPATH(path) towstring(strPath + path).c_str()
	string strPath = "../Bin/Resources/Textures/UI/";

#define TEXTURE_UI_ID(ID) ID_UINT(TEXTURE, TEXTURE_UI, ID)
	
	USEGAMEINSTANCE;
#if defined(_UI)
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_LOGO),
		CTexture::Create(UIPATH("Logo/Logo%d.png"), 4)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_SHORTKEY),
		CTexture::Create(UIPATH("Shortkey/Shortkey%d.png"), 17)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_MAIN),
		CTexture::Create(UIPATH("Main/Main%d.png"), 20)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_MENU),
		CTexture::Create(UIPATH("Menu/Menu%d.png"), 21)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_PLAYER_STATUS),
		CTexture::Create(UIPATH("Player_Status/Status%d.png"), 4)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_SKILL),
		CTexture::Create(UIPATH("Skill/Skill/Skill%d.png"), 4)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_INVENTORY),
		CTexture::Create(UIPATH("Inventory/Inven%d.png"), 37)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_FOOD_INVEN),
		CTexture::Create(UIPATH("Food_Window/Window%d.png"), 1)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_SKILL_SLOTICON),
		CTexture::Create(UIPATH("Skill/Skill_SlotIcon/Skill%d.png"), 4)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_WEAPON_SLOTICON),
		CTexture::Create(UIPATH("Skill/Weapon_SlotIcon/Weapon%d.png"), 5)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_ENEMY_BLOOD),
		CTexture::Create(UIPATH("Enemy/Enemy_Blood/Blood%d.png"), 6)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_ENEMY_NAME),
		CTexture::Create(UIPATH("Enemy/Enemy_Name/Name%d.png"), 13)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_BOSS_BLOOD),
		CTexture::Create(UIPATH("Boss/Boss_Blood/Blood%d.png"), 6)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_BOSS_NAME),
		CTexture::Create(UIPATH("Boss/Boss_Name/Elem%d.png"), 13)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_ENEMY_INDICATOR),
		CTexture::Create(UIPATH("Enemy_Indicator/Indicator%d.png"), 1)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_DAMAGE_NUMBER),
		CTexture::Create(UIPATH("Damage_Number/%d.png"), 12)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_WINDOW),
		CTexture::Create(UIPATH("Guide_Window/Window/Window%d.png"), 3)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_HAND_ANIM),
		CTexture::Create(UIPATH("Guide_Window/Hand_Anim/Hand%d.png"), 4)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_ITEM),
		CTexture::Create(UIPATH("Item/Item%d.png"), 20)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_FOOD_ICON),
		CTexture::Create(UIPATH("Food_Icon/Cook%d.png"), 17)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_ITEM_NUM),
		CTexture::Create(UIPATH("Item_Num/%d.png"), 11)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_QUEST_MARK),
		CTexture::Create(UIPATH("Quest/Quest_Mark/Mark%d.png"), 3)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_ENEMY_RANK),
		CTexture::Create(UIPATH("Enemy/Enemy_Rank/Rank%d.png"), 2))); 
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_2GUN_CROSSHAIR),
		CTexture::Create(UIPATH("CrossHair/Gun/CrossHair%d.png"), 2)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_TALK_ANIM),
		CTexture::Create(UIPATH("Guide_Window/Talk_Anim/Talk%d.png"), 4)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_ALERT_WINDOW),
		CTexture::Create(UIPATH("Alert_Window/%d.png"), 1)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_INGREDIENT),
		CTexture::Create(UIPATH("Ingredient/Ingrd%d.png"), 19)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_TALK_WINDOW),
		CTexture::Create(UIPATH("Talk_Window/Talk%d.png"), 6)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_PROCEEDING),
		CTexture::Create(UIPATH("Proceeding/%d.png"), 4)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_PLACEMARK),
		CTexture::Create(UIPATH("PlaceMark/Mark%d.png"), 4)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_QUEST_NOTICE),
		CTexture::Create(UIPATH("Quest/Quest_Notice/%d.png"), 4)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_ATTACK_TARGET),
		CTexture::Create(UIPATH("Attack_Target/%d.png"), 1)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_ENEMY_ATTACK),
		CTexture::Create(UIPATH("Enemy/Enemy_Attack/%d.png"), 1)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_ITEM_DETAIL),
		CTexture::Create(UIPATH("Item_Detail/Detail%d.png"), 21)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_COOK_ICON),
		CTexture::Create(UIPATH("Cook_Window/Cook_Icon/%d.png"), 11)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_COOK_DETAIL),
		CTexture::Create(UIPATH("Cook_Window/Cook_Detail/%d.png"), 19)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_COOK_INGRD_NUM),
		CTexture::Create(UIPATH("Cook_Window/Ingrd_Num/%d.png"), 23)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_EQUIP_ICON),
		CTexture::Create(UIPATH("Equip_Window/Equip_Icon/%d.png"), 21)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_EQUIP_WEAPON),
		CTexture::Create(UIPATH("Equip_Window/Equip_Weapon/%d.png"), 5)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_EQUIP_DETAIL),
		CTexture::Create(UIPATH("Equip_Window/Equip_Detail/%d.png"), 5)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_COOK_PROGRESS),
		CTexture::Create(UIPATH("Cook_Window/Cook_Progress/%d.png"), 13)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_ORDER_ICON),
		CTexture::Create(UIPATH("Order_Window/Order_Icon/%d.png"), 19)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_ORDER_ANIM),
		CTexture::Create(UIPATH("Order_Window/Order_Anim/Gacha (%d).png"), 367)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_ORDER_ITEM),
		CTexture::Create(UIPATH("Order_Window/Order_Item/%d.png"), 18)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_ORDER_RESULT),
		CTexture::Create(UIPATH("Order_Window/Order_Result/%d.png"), 20)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_ORDER_ITEMNAME),
		CTexture::Create(UIPATH("Order_Window/Order_ItemName/%d.png"), 14)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_ORDER_TENRESULT),
		CTexture::Create(UIPATH("Order_Window/Order_TenResult/%d.png"), 14)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_PLACENAME),
		CTexture::Create(UIPATH("PlaceName/%d.png"), 8)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_ENERGY_GEAR),
		CTexture::Create(UIPATH("Energy_Gear/%d.png"), 2)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_MINIMAP),
		CTexture::Create(UIPATH("MiniMap/%d.png"), 6)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_DIST_NUM),
		CTexture::Create(UIPATH("Dist_Num/%d.png"), 11)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_QUEST_COMPLETE),
		CTexture::Create(UIPATH("Quest/Quest_Complete/%d.png"), 6)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_MINIGAME_START),
		CTexture::Create(UIPATH("Minigame/Start/%d.png"), 7)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_MINIGAME_ICON),
		CTexture::Create(UIPATH("Minigame/Icon/%d.png"), 13)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_MINIGAME_EXPLAIN),
		CTexture::Create(UIPATH("Minigame/Explain/%d.png"), 6)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_MINIGAME_GAIN),
		CTexture::Create(UIPATH("Minigame/Gain/%d.png"), 4)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_MINIGAME_RESULT),
		CTexture::Create(UIPATH("Minigame/Result/%d.png"), 12)));
	FAILEDCHECK(pGameInstance->Add_Component_Prototype(LEVEL_STATIC, TEXTURE_UI_ID(TEXTURE_UI_CRYSTAL_BLOOD),
		CTexture::Create(UIPATH("Boss/Crystal_Blood/%d.png"), 4)));
#endif

	return S_OK;
}

CLoader_Logo * CLoader_Logo::Create()
{
	CLoader_Logo*		pInstance = new CLoader_Logo();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLoader_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader_Logo::Free()
{
}

