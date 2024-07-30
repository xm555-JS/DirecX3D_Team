#include "stdafx.h"
#include "..\Public\Level_Edit_UI.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Camera.h"

CLevel_Edit_UI::CLevel_Edit_UI()
	: CLevel()
{

}

HRESULT CLevel_Edit_UI::Initialize()
{
	if (FAILED(__super::Initialize()))
		__debugbreak();

	/*
	if (FAILED(Ready_Layer_BackGround(ID_UINT(BACKGROUND))))
		__debugbreak();	*/

	//USEGAMEINSTANCE;
	//((CCursor*)pGameInstance->Get_Object(LEVEL_STATIC, ID_UINT(CURSOR)))->Set_CurCursorMode(CCursor::MODE_DEFAULT);
	//ShowCursor(false);
	FAILEDCHECK(Ready_Layer_Sky(ID_UINT(SKY)));
	FAILEDCHECK(Ready_Layer_Camera(ID_UINT(CAMERA)));

	USEGAMEINSTANCE;
	NULLCHECK(pGameInstance->Add_GameObject(LEVEL_EDIT_UI, ID_UINT(UI), ID_UINT(UI, UI_CURSOR)));

	return S_OK;
}

void CLevel_Edit_UI::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		

	//if (GetKeyState(VK_SPACE) & 0x8000)
	//{
	//	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	//	Safe_AddRef(pGameInstance);

	//	pGameInstance->Change_Level(LEVEL_LOADING, CLevel_Loading::Create(LEVEL_GAMEPLAY));

	//	Safe_Release(pGameInstance);
	//}
}

HRESULT CLevel_Edit_UI::Render()
{
	if (FAILED(__super::Render()))
		__debugbreak();

	SetWindowText(g_hWnd, TEXT("Level_Tool_UI"));

	return S_OK;
}

HRESULT CLevel_Edit_UI::Ready_Layer_BackGround(ID_UINT iLayerID)
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

HRESULT CLevel_Edit_UI::Ready_Layer_Sky(ID_UINT iID)
{
	USEGAMEINSTANCE;

	pGameInstance->Add_GameObject(LEVEL_EDIT_UI, ID_UINT(iID), ID_UINT(SKY));

	return S_OK;
}

HRESULT CLevel_Edit_UI::Ready_Layer_Camera(ID_UINT iID)
{

	USEGAMEINSTANCE;
	CCamera::CAMERADESC CameraDesc;

	CameraDesc.vEye = _float4(0.0f, 10.f, -10.f, 1.f);
	CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);

	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	CCamera* pCamera_Free = static_cast<CCamera*>(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iID), ID_UINT(CAMERA, CAMERA_FREE), &CameraDesc));
	NULLCHECK(pCamera_Free);

	//CCamera* pCamera_Fixed = static_cast<CCamera*>(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iID), ID_UINT(CAMERA, CAMERA_FIXED), &CameraDesc));
	//NULLCHECK(pCamera_Fixed);

	pGameInstance->Set_CurrentCamera(pCamera_Free);
	return S_OK;
}

CLevel_Edit_UI * CLevel_Edit_UI::Create()
{
	CLevel_Edit_UI*		pInstance = new CLevel_Edit_UI();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Edit_UI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Edit_UI::Free()
{
	__super::Free();

	//ShowCursor(true);


}

