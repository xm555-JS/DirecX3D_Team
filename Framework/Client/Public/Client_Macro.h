#pragma once

#define		SOUNDMGR	CSound_Manager::Get_Instance()

#define JSONPATH_EFFECT	"../Bin/Resources/Json/Effect/"

#ifdef _DEBUG
#define		DEBUGFLOAT				CImgui_Manager::Get_Instance()->Get_DebugFloat()
#define		DEBUGFLOAT4(Index)		CImgui_Manager::Get_Instance()->Get_DebugFloat4(Index)
#define		DEBUGVECTOR(Index)		CImgui_Manager::Get_Instance()->Get_DebugVector(Index)
#endif // DEBUG

#define		PLAYERPTR		GAMEINSTANCE->Get_Object(LEVEL_GAMEPLAY, ID_UINT(PLAYER));

#define EFFECT_POS		CEffect_Manager::Get_Instance()->Create_Effect_Positioning
#define EFFECT_POS_ROT	CEffect_Manager::Get_Instance()->Create_Effect_Positioning_Rotaion
#define	EFFECT_EXCUTE	CEffect_Manager::Get_Instance()->ExcuteEffect
//#define	EFFECT_EXCUTE(szEffectName, matTransform, fTimeSpeed)\
//	CEffect_Manager::Get_Instance()->ExcuteEffect;
//#define	EFFECT_EXCUTE(szEffectName, pOwner, eFOLLOW_MODE, szBoneName)\
//	CEffect_Manager::Get_Instance()->ExcuteEffect(szEffectName, XMMatrixIdentity(), pOwner, eFOLLOW_MODE, szBoneName);

//#define FIXCURSOR(bFix) CCamera_Target_Position* pCamera = ((CCamera_Target_Position*)pGameInstance->Get_CurrentCamera()); \
//pCamera->Set_CursorFix(bFix);

#define FIXCURSOR(bFix) pGameInstance->Set_CursorFix(bFix);

 
#define	_PLAYER
#define	_ENEMYNORMAL
#define	_ENEMYBOSS
#define	_MAP
#define _MAPINSTANCE
#define _ITEM
#define _STRUCT
#define _STRUCT_ALL
#define _WEAPON
#define	_TERRAIN
#define	_EFFECT
#define	_UI
#define	_SKYBOX
#define _NPC
#define _NOISE_TEX
#define _EQUIP

#define _VILLAGE_ISLAND
#define _STONE_ISLAND
#define _DRAGON
#define _ROBELLA_ISLAND
#define _MECHANICALCITY_ISLAND
#define _INVEN

//AnimToolLevel
//#define	_PLAYERMODEL
//#define	_ENEMYMODEL
#define	_BOSSMODEL
//#define _NPCMODEL

// Effect Tool
#ifdef _DEBUG
#define _EFFECTTOOL
#endif

// Map Tool
//#define _MAPTOOL
	//CImgui_Manager::Get_Instance()->Get_Window(CImgui_Manager::TOOL_MAP)->Is_Enable()	


//#define _TEST_IN
////#define _TEST_HAN
////#define _TEST_SHIN
//#define _TEST_JJS
////#define _TEST_JJ
//#define _TEST_YJS