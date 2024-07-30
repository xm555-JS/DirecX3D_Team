#include "stdafx.h"
#include "..\Public\Level_Menu.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "UI.h"
//#include "Button.h"
#include "CustomFont.h"
#include "Event_Manager.h"

CLevel_Menu::CLevel_Menu()
	: CLevel()
{

}

HRESULT CLevel_Menu::Initialize()
{
	if (FAILED(__super::Initialize()))
		__debugbreak();


	if (FAILED(Ready_Layer_BackGround(ID_UINT(BACKGROUND))))
		__debugbreak();

	//USEGAMEINSTANCE;
	//((CCursor*)pGameInstance->Get_Object(LEVEL_STATIC, ID_UINT(CURSOR)))->Set_CurCursorMode(CCursor::MODE_DEFAULT);
	//ShowCursor(false);

	return S_OK;
}

void CLevel_Menu::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	USEGAMEINSTANCE;
	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		pGameInstance->Change_Level(LEVEL_LOADING, CLevel_Loading::Create(LEVEL_GAMEPLAY));
	}
	//if (KEY_DOWN(VK_SHIFT))
	//{
	//	pGameInstance->Change_Level(LEVEL_LOADING, CLevel_Loading::Create(LEVEL_TOOL_UI));
	//}
}

HRESULT CLevel_Menu::Render()
{
	if (FAILED(__super::Render()))
		__debugbreak();

	SetWindowText(g_hWnd, TEXT("Level_Menu"));

	return S_OK;
}

HRESULT CLevel_Menu::Ready_Layer_BackGround(ID_UINT iLayerID)
{
	USEGAMEINSTANCE;

	///* For.BackGround */
	//_uint	iTextureIdx = 1;
	//if (nullptr == (pGameInstance->Add_GameObject(LEVEL_MENU, iLayerID, ID_UINT(BACKGROUND), &iTextureIdx)))
	//	__debugbreak();

	///* For.Logo*/
	//if (nullptr == (pGameInstance->Add_GameObject(LEVEL_MENU, iLayerID, ID_UINT(LOGO),
	//	&CUI::UIDESC(g_iWinCX >> 1, 300.f, 512.f, 111.25f))))
	//	__debugbreak();

	//json	SaveData;

	//SaveData["Text"] = L"종료하기";
	//SaveData["Color"] = json{ 143.f / 256.f, 200.f / 256.f, 188.f / 256.f, 1.f };
	//SaveData["FontSize"] = 0.7f;
	//SaveData["Position"] = json{ 50.f, 600.f };
	//SaveData["Size"] = json{ 200.f, 100.f };


	//pGameInstance->Save_Json(JSONFILEPATH("ButtonDesc/Button_Exit"), SaveData);

	//CButton::BUTTONDESC	tButtonDesc(vButtonPosition.x, vButtonPosition.y, vButtonSize.x, vButtonSize.y,
	//	TEXT("새 게임"), vButtonColor, fButtonFontSize);

	//EVENTDESC	tEvent;
	//tEvent = { LEVEL_CHANGE, (void*)LEVEL_LOADING, (void*)LEVEL_INDUCTION };

	///* For.Button*/
	//CButton*	pButton;
	//pButton = (CButton*)pGameInstance->Add_GameObject(LEVEL_MENU, iLayerID, ID_UINT(BUTTON), TEXT("ButtonDesc/Button_NewGame"));
	//if(nullptr == pButton)
	//	__debugbreak();
	//pButton->Set_Event(tEvent);

	//
	//tEvent = { LEVEL_CHANGE, (void*)LEVEL_LOADING, (void*)LEVEL_OPTICAL };
	///* For.Button*/
	//pButton = (CButton*)pGameInstance->Add_GameObject(LEVEL_MENU, iLayerID, ID_UINT(BUTTON), TEXT("ButtonDesc/Button_SelectLevel"));
	//if (nullptr == pButton)
	//	__debugbreak();
	//pButton->Set_Event(tEvent);


	//tEvent = { LEVEL_CHANGE, (void*)LEVEL_LOADING, (void*)LEVEL_EDIT };
	///* For.Button*/
	//pButton = (CButton*)pGameInstance->Add_GameObject(LEVEL_MENU, iLayerID, ID_UINT(BUTTON), TEXT("ButtonDesc/Button_LevelEdit"));
	//if (nullptr == pButton)
	//	__debugbreak();
	//pButton->Set_Event(tEvent);


	//tEvent.eEventType = GAME_EXIT;
	///* For.Button*/
	//pButton = (CButton*)pGameInstance->Add_GameObject(LEVEL_MENU, iLayerID, ID_UINT(BUTTON), TEXT("ButtonDesc/Button_Exit"));
	//if (nullptr == pButton)
	//	__debugbreak();
	//pButton->Set_Event(tEvent);


	return S_OK;
}


CLevel_Menu* CLevel_Menu::Create()
{
	CLevel_Menu* pInstance = new CLevel_Menu();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Menu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Menu::Free()
{
	__super::Free();

	//ShowCursor(true);


}

