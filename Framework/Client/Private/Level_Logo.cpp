#include "stdafx.h"
#include "..\Public\Level_Logo.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "UI.h"
#include "Camera.h"
#include "CustomFont.h"
#include "Event_Manager.h"
#include "Sound_Manager.h"


CLevel_Logo::CLevel_Logo()
	: CLevel()
{

}

HRESULT CLevel_Logo::Initialize()
{
	FAILEDCHECK(__super::Initialize());

	FAILEDCHECK(Ready_Layer_Camera(ID_UINT(CAMERA)));
	FAILEDCHECK(Ready_Layer_UI(ID_UINT(UI)));

	//USEGAMEINSTANCE;
	//((CCursor*)pGameInstance->Get_Object(LEVEL_STATIC, ID_UINT(CURSOR)))->Set_CurCursorMode(CCursor::MODE_DEFAULT);

	MGR(CSound_Manager)->ChangeBGM(TEXT("Logo.mp3"), 0.3f, 0.1f);

	return S_OK;
}

void CLevel_Logo::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		

	USEGAMEINSTANCE;

	RECT rcSelect;
	SetRect(&rcSelect, 0.f, 0.f, g_iWinCX, g_iWinCY);

	POINT	ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (PtInRect(&rcSelect, ptMouse))
	{
		if (KEY_DOWN(VK_LBUTTON))
		{
			MGR(CSound_Manager)->PlaySound(TEXT("UI_Start.wav"), CSound_Manager::CHANNEL_UI, 0.3f);
		}
		if (KEY_UP(VK_LBUTTON))
		{
			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);

			pGameInstance->Change_Level(LEVEL_LOADING, CLevel_Loading::Create(LEVEL_GAMEPLAY));

			Safe_Release(pGameInstance);
		}
	}
}

HRESULT CLevel_Logo::Render()
{
	FAILEDCHECK(__super::Render());

	SetWindowText(g_hWnd, TEXT("로고레벨임. "));

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Camera(ID_UINT iLayerID)
{
	USEGAMEINSTANCE;
	CCamera::CAMERADESC CameraDesc;

	CameraDesc.vEye = _float4(0.0f, 10.f, -10.f, 1.f);
	CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);

	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	CCamera* pCamera_Free = static_cast<CCamera*>(pGameInstance->Add_GameObject(LEVEL_LOGO, ID_UINT(iLayerID), ID_UINT(CAMERA, CAMERA_FREE), &CameraDesc));
	NULLCHECK(pCamera_Free);

	//CCamera* pCamera_Fixed = static_cast<CCamera*>(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iID), ID_UINT(CAMERA, CAMERA_FIXED), &CameraDesc));
	//NULLCHECK(pCamera_Fixed);

	pGameInstance->Set_CurrentCamera(pCamera_Free);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_UI(ID_UINT iLayerID)
{
	USEGAMEINSTANCE;
	NULLCHECK(pGameInstance->Add_GameObject(LEVEL_LOGO, ID_UINT(UI), ID_UINT(UI, UI_CURSOR)));

	json Data;

	pGameInstance->Load_Json(pGameInstance->json_Complete_Path(TEXT("UI/LogoUI")).c_str(), &Data);

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

		CGameObject* pGameObject = pGameInstance->Add_GameObject(LEVEL_LOGO, IDPrototype, IDPrototype, &UIDesc);
		if (!pGameObject)
		{
#ifdef DEBUG
			cout << "UI ID : " << (_uint)IDPrototype.tNums.Num0 << " / " << (_uint)IDPrototype.tNums.Num1 << " / " << (_uint)IDPrototype.tNums.Num2 << " / " << (_uint)IDPrototype.tNums.Num3 << " is false" << endl;
#endif // DEBUG
			continue;
		}

		CUI* pUI = (CUI*)pGameObject;
		pUI->Set_FontText(strTemp);
	}

	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create()
{
	CLevel_Logo*		pInstance = new CLevel_Logo();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();

}

