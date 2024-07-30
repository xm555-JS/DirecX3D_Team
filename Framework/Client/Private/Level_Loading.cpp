#include "stdafx.h"
#include "..\Public\Level_Loading.h"
#include "Loader.h"
#include "Level_Logo.h"
//#include "Level_Menu.h"
#include "Level_GamePlay.h"
#include "Level_Edit_Anim.h"
#include "Level_Edit_UI.h"
//#include "Level_Optical.h"
#include "GameInstance.h"

#include "Loading_Anim.h"
#include "UI_Loading_Bar.h"
#include "Camera.h"

CLevel_Loading::CLevel_Loading()
	: CLevel()
{

}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevel)
{
	//ShowCursor(false);
	USEGAMEINSTANCE;

	if (FAILED(__super::Initialize()))
		__debugbreak();

	m_eNextLevel = eNextLevel;

	m_pLoader = CLoader::Create(eNextLevel);
	if (nullptr == m_pLoader)
		__debugbreak();


	FAILEDCHECK(Ready_Layer_Camera(ID_UINT(CAMERA)));
	FAILEDCHECK(Ready_Layer_UI(ID_UINT(UI)));

	//NULLCHECK(pGameInstance->Add_GameObject(LEVEL_LOADING, ID_UINT(UI), ID_UINT(UI,UI_TEST)));

	//USEGAMEINSTANCE;
	///* For.Prototype_Component_Texture_BackGround */
	//if (FAILED(pGameInstance->Add_Component_Prototype(LEVEL_LOADING, ID_UINT(TEXTURE, TEXTURE_BACKGROUND),
	//	CTexture::Create(TEXT("../Bin/Resources/Textures/BackGround/BackGround%d.png"), 3))))
	//	__debugbreak();

	///* For.BackGround */
	//_uint	iTextureIdx = 0;
	//if (nullptr == (pGameInstance->Add_GameObject(LEVEL_LOADING, ID_UINT(BACKGROUND), ID_UINT(BACKGROUND), &iTextureIdx)))
	//	__debugbreak();

	//_uint	iRandomNum = Random(0, 2);
	_uint	iLoading_Anim_Num = 0;
	m_pLoading_Anim = CLoading_Anim::Create(&iLoading_Anim_Num);




	return S_OK;
}

void CLevel_Loading::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	

	m_pLoading_Anim->Tick(fTimeDelta);

	if (true == m_pLoader->Get_CurLoader()->is_Finished())
	{
		if(m_pLoadingBar)
			m_pLoadingBar->Set_Finish_Loading();
		m_fAccTime += fTimeDelta;
	}


	if (1.f < m_fAccTime)
	{
		//if (GetKeyState(VK_RETURN) & 0x8000)
		if (m_pLoading_Anim->Is_Finish() ||
			GetKeyState(VK_RETURN) & 0x8000)
		{
			CLevel*			pLevel = nullptr;

			switch (m_eNextLevel)
			{
			case LEVEL_LOGO:
				pLevel = CLevel_Logo::Create();
				break;
			//case LEVEL_MENU:
			//	pLevel = CLevel_Menu::Create();
			//	break;
			case LEVEL_EDIT_ANIM:
				pLevel = CLevel_Edit_Anim::Create();
				break;
			case LEVEL_GAMEPLAY:
				pLevel = CLevel_GamePlay::Create();
				break;
			case LEVEL_EDIT_UI:
				pLevel = CLevel_Edit_UI::Create();
				break;
			//case LEVEL_OPTICAL:
			//	pLevel = CLevel_Optical::Create();
			//	break;
			}

			if (nullptr == pLevel)
				return;

			CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);

			pGameInstance->Change_Level(m_eNextLevel, pLevel);

			Safe_Release(pGameInstance);
		}
	}
}

HRESULT CLevel_Loading::Render()
{
	if (FAILED(__super::Render()))
		__debugbreak();

	SetWindowText(g_hWnd, m_pLoader->Get_CurLoader()->Get_LoadingText());

	USEGAMEINSTANCE;

	m_pLoading_Anim->Render();

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_UI(ID_UINT iLayerID)
{
	USEGAMEINSTANCE;
	NULLCHECK(pGameInstance->Add_GameObject(LEVEL_LOADING, ID_UINT(UI), ID_UINT(UI, UI_CURSOR)));

	json Data;

	pGameInstance->Load_Json(pGameInstance->json_Complete_Path(TEXT("UI/LoadingUI")).c_str(), &Data);

	_uint iPrototypeID;

	for (auto& elem : Data)
	{
		CUI::UIDesc UIDesc;
		string strTemp = "";
		iPrototypeID = elem["iPrototypeID"];
		UIDesc.fInitX = elem["fInitX"];
		UIDesc.fInitY = elem["fInitY"];
		UIDesc.fInitSizeX = elem["fInitSizeX"];
		UIDesc.fInitSizeY = elem["fInitSizeY"];
		UIDesc.fAngle = elem["fAngle"];
		UIDesc.iTextureIdx = elem["iTextureIdx"];
		UIDesc.iRenderLayer = elem["iRenderLayer"];
		strTemp = elem["strText"];
		UIDesc.vFontPos.x = elem["vFontPosx"];
		UIDesc.vFontPos.y = elem["vFontPosy"];
		UIDesc.vFontColor.x = elem["vFontColorx"];
		UIDesc.vFontColor.y = elem["vFontColory"];
		UIDesc.vFontColor.z = elem["vFontColorz"];
		UIDesc.vFontColor.w = elem["vFontColorw"];
		UIDesc.fFontSize = elem["fFontSize"];
		UIDesc.bRenderFontText = elem["bRenderFontText"];
		UIDesc.bRenderFontNum = elem["bRenderFontNum"];

		ID_UINT IDPrototype;
		IDPrototype.iID = iPrototypeID;

		CGameObject* pGameObject = pGameInstance->Add_GameObject(LEVEL_LOADING, IDPrototype, IDPrototype, &UIDesc);
		CUI* pUI = (CUI*)pGameObject;
		pUI->Set_FontText(strTemp);

		CUI_Loading_Bar* pLoadingBar = dynamic_cast<CUI_Loading_Bar*>(pGameObject);
		if (pLoadingBar)
			m_pLoadingBar = pLoadingBar;
	}

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_Camera(ID_UINT iLayerID)
{
	USEGAMEINSTANCE;
	CCamera::CAMERADESC CameraDesc;

	CameraDesc.vEye = _float4(0.0f, 10.f, -10.f, 1.f);
	CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);

	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	CCamera* pCamera_Free = static_cast<CCamera*>(pGameInstance->Add_GameObject(LEVEL_LOADING, ID_UINT(iLayerID), ID_UINT(CAMERA, CAMERA_FREE), &CameraDesc));
	NULLCHECK(pCamera_Free);

	//CCamera* pCamera_Fixed = static_cast<CCamera*>(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iID), ID_UINT(CAMERA, CAMERA_FIXED), &CameraDesc));
	//NULLCHECK(pCamera_Fixed);

	pGameInstance->Set_CurrentCamera(pCamera_Free);

	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(LEVEL eNextLevel)
{
	CLevel_Loading*		pInstance = new CLevel_Loading();

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		MSG_BOX("Failed to Created : CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoading_Anim);
	Safe_Release(m_pLoader);

	//ShowCursor(true);

}

