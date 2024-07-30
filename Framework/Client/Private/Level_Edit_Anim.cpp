#include "stdafx.h"
#include "..\Public\Level_Edit_Anim.h"
#include "GameInstance.h"

#if defined(USE_IMGUI)
#include "Imgui_Manager.h"
#endif

//#include "Camera_Tool_Free.h"
//#include "Camera_Tool_Player.h"
#include "Camera.h"

#include "Level_Loading.h"

CLevel_Edit_Anim::CLevel_Edit_Anim()
	: CLevel()
{

}

HRESULT CLevel_Edit_Anim::Initialize()
{
	//ShowCursor(true);


 	if (FAILED(__super::Initialize()))
		__debugbreak();

	FAILEDCHECK(Ready_Lights());

	FAILEDCHECK(Ready_Layer_Sky(ID_UINT(SKY)));
	FAILEDCHECK(Ready_Layer_Camera(ID_UINT(CAMERA)));
	FAILEDCHECK(Ready_Layer_BackGround(ID_UINT(BACKGROUND)));
	//FAILEDCHECK(Ready_Layer_Player(ID_UINT(PLAYER)));

	return S_OK;
}

void CLevel_Edit_Anim::Tick(_float fTimeDelta)
{
#if defined(USE_IMGUI)
	//CImgui_Manager::Get_Instance()->Tick(fTimeDelta);

#if 0

	USEGAMEINSTANCE;
	if (KEY_DOWN(VK_ESCAPE))
		pGameInstance->Change_Level(LEVEL_LOADING, CLevel_Loading::Create(LEVEL_GAMEPLAY));

#endif // 0

	//if (true == CImgui_Manager::Get_Instance()->Get_PlayerCameraState())
	//{
	//	USEGAMEINSTANCE;
	//	pGameInstance->Set_CurrentCamera(m_Cameras[CAMERA_TOOL_PLAYER]);
	//}
	//else
	//{
	//	USEGAMEINSTANCE;
	//	pGameInstance->Set_CurrentCamera(m_Cameras[CAMERA_TOOL_FREE]);
	//}

#endif
	__super::Tick(fTimeDelta);
}

HRESULT CLevel_Edit_Anim::Render()
{
	if (FAILED(__super::Render()))
		__debugbreak();

#if defined(USE_IMGUI)
	//CImgui_Manager::Get_Instance()->Render();
#endif

	SetWindowText(g_hWnd, TEXT("Level_Edit."));

	return S_OK;
}

void CLevel_Edit_Anim::Add_FreeCamera()
{
	USEGAMEINSTANCE;
	CCamera::CAMERADESC CameraDesc;

	CameraDesc.vEye = _float4(0.0f, 10.f, -10.f, 1.f);
	CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);

	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = g_fNear;
	CameraDesc.fFar = g_fFar;

	m_Cameras[CAMERA_FREE].push_back(static_cast<CCamera*>(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(CAMERA), ID_UINT(CAMERA, CAMERA_FREE), &CameraDesc)));
	NULLCHECK(m_Cameras[CAMERA_FREE].back());
}


HRESULT CLevel_Edit_Anim::Ready_Lights()
{
	//USEGAMEINSTANCE;

	//LIGHTDESC			LightDesc;
	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	////LightDesc.vPosition = _float4(100.f, 100.f, 0.f, 1.f);
	//LightDesc.fRange = 10000.f;
	//LightDesc.eType = tagLightDesc::TYPE_DIRECTIONAL;
	////LightDesc.eType = tagLightDesc::TYPE_POINT;
	//LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	////LightDesc.vDiffuse = _float4(231.f / 255.f, 183.f / 255.f, 175.f / 255.f, 1.f);
	//LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	//LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	//LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	//NULLCHECK(pGameInstance->Add_Light(LightDesc));


	//LIGHTDESC			LightDesc;
	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	//
	//LightDesc.eType = tagLightDesc::TYPE_DIRECTIONAL;
	//LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	//LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	//LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	//
	//if (FAILED(pGameInstance->Add_Light(LightDesc)))
	//	__debugbreak();

	return S_OK;
}

HRESULT CLevel_Edit_Anim::Ready_Layer_Sky(ID_UINT iID)
{
	USEGAMEINSTANCE;

	pGameInstance->Add_GameObject(LEVEL_EDIT_ANIM, ID_UINT(iID), ID_UINT(SKY));

	return S_OK;
}

HRESULT CLevel_Edit_Anim::Ready_Layer_Camera(ID_UINT iID)
{
	USEGAMEINSTANCE;
	CCamera::CAMERADESC CameraDesc;

	CameraDesc.vEye = _float4(0.0f, 10.f, -10.f, 1.f);
	CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);

	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	CCamera* pCamera_Free = static_cast<CCamera*>(pGameInstance->Add_GameObject(LEVEL_EDIT_ANIM, ID_UINT(iID), ID_UINT(CAMERA, CAMERA_FREE), &CameraDesc));
	NULLCHECK(pCamera_Free);
	m_Cameras[CAMERA_FREE].push_back(pCamera_Free);

	CCamera* pCamera_Fixed = static_cast<CCamera*>(pGameInstance->Add_GameObject(LEVEL_EDIT_ANIM, ID_UINT(iID), ID_UINT(CAMERA, CAMERA_FIXED), &CameraDesc));
	NULLCHECK(pCamera_Fixed);
	m_Cameras[CAMERA_FIXED].push_back(pCamera_Fixed);


	pGameInstance->Set_CurrentCamera(pCamera_Fixed);
	return S_OK;
}


HRESULT CLevel_Edit_Anim::Ready_Layer_BackGround(ID_UINT iID)
{
	USEGAMEINSTANCE;

	pGameInstance->Add_GameObject(LEVEL_EDIT_ANIM, ID_UINT(iID), ID_UINT(BACKGROUND));

	return S_OK;
}

HRESULT CLevel_Edit_Anim::Ready_Layer_Player(ID_UINT iID)
{
	USEGAMEINSTANCE;

	//pGameInstance->Add_GameObject(LEVEL_EDIT_ANIM, ID_UINT(iID), ID_UINT(PLAYER, PLAYER_DEFAULT));

	return S_OK;
}

HRESULT CLevel_Edit_Anim::Ready_Layer_Fixedbody(ID_UINT iLayerID)
{


	return S_OK;
}


CLevel_Edit_Anim * CLevel_Edit_Anim::Create()
{
	CLevel_Edit_Anim*		pInstance = new CLevel_Edit_Anim();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Edit_Anim");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Edit_Anim::Free()
{
	__super::Free();
#if defined(USE_IMGUI)
	//CImgui_Manager::Get_Instance()->Destroy_Instance();
#endif

	//ShowCursor(false);

}

