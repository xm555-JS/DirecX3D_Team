#include "stdafx.h"
#include "Level_GamePlay.h"
#include "GameInstance.h"

#include "Camera.h"
#include "Camera_Target_Position.h"
#include "Camera_Target_Position_Lerp.h"
#include "Camera_Action.h"

#include "Navigation.h"

#include "Level_Loading.h"
#include "Enemy.h"
#include "UI_Logo_Back.h"

#include "Imgui_Manager.h"

#include "GameObject.h"
#include "Cell.h"
#include "Player.h"
#include "Weapon.h"
#include "Enemy_Bullet_Dragon.h"

#include "Portal.h"
#include "Region.h"

#include "Effect_Rect.h"

#include "MapObj_Instance.h"

#include "Light_Sun.h"
#include "Light_Target.h"
#include "RenderTarget.h"

#include "Light.h"

#include "Imgui_Manager.h"
#include "Leviathan_Crystal.h"

#include "Easing_Utillity.h"
#include "Enemy_Boss_Leviathan.h"

CLevel_GamePlay::CLevel_GamePlay()
	: CLevel()
{
	USEGAMEINSTANCE;
	m_pDevice = DEVICE;
	m_pContext = CONTEXT;
}

void CLevel_GamePlay::Add_FreeCamera()
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

void CLevel_GamePlay::On_EventMessege(EVENTDESC tEvent)
{
	if (EVENT_MONSTER_DEAD == tEvent.eEventType)
	{
		if (ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_WEAVER).iID == ((CEnemy*)tEvent.lParam)->Get_PrototypeID())
		{
			m_bStartCreateCrystal = true;
		}
	}

	if (EVENT_LEVIATHAN_APEAR == tEvent.eEventType)
	{
		m_bLeviathanPhase = true;
	}

	if (EVENT_LEVIATHAN_DEAD == tEvent.eEventType)
	{
		m_bLeviathanPhase = false;
	}
}

HRESULT CLevel_GamePlay::Initialize()
{
	USEGAMEINSTANCE;
	pGameInstance->Add_CallbackList(this, EVENT_MONSTER_DEAD); // 레벨 바뀔 때 마다 Prototype 새로 가져오기.
	pGameInstance->Add_CallbackList(this, EVENT_LEVIATHAN_APEAR); // 레벨 바뀔 때 마다 Prototype 새로 가져오기.
	pGameInstance->Add_CallbackList(this, EVENT_LEVIATHAN_DEAD); // 레벨 바뀔 때 마다 Prototype 새로 가져오기.

	FAILEDCHECK(__super::Initialize());

	FAILEDCHECK(Ready_Lights());

#if defined(_SKYBOX)
	FAILEDCHECK(Ready_Layer_Sky(ID_UINT(SKY)));
#endif

	FAILEDCHECK(Ready_Layer_Camera(ID_UINT(CAMERA)));
	FAILEDCHECK(Ready_Layer_BackGround(ID_UINT(BACKGROUND)));

#if defined(_PLAYER)
	FAILEDCHECK(Ready_Layer_Player(ID_UINT(PLAYER)));
#endif

#if defined(_ENEMYNORMAL)
	FAILEDCHECK(Ready_Layer_EnemyNormal(ID_UINT(ENEMY, ENEMY_NORMAL)));
#endif

#if defined(_ENEMYBOSS)
	FAILEDCHECK(Ready_Layer_EnemyBoss(ID_UINT(ENEMY, ENEMY_BOSS)));
#endif

#if defined(_TEST_SHIN)
	//FAILEDCHECK(Ready_Layer_EnemyBoss(ID_UINT(ENEMY, ENEMY_BOSS)));
#endif

#if defined(_MAP)
	FAILEDCHECK(Ready_Layer_Map(ID_UINT(MAP)));
	//Ready_Layer_MiniGame();
#endif

#if defined(_ITEM)
	FAILEDCHECK(Ready_Layer_Item(ID_UINT(MAP)));
#endif	

	////Load_Navigation();
#if defined(_UI)
	FAILEDCHECK(Ready_Layer_UI(ID_UINT(UI)));
#endif

#if defined(_NPC)
	Ready_Layer_Npc(ID_UINT(NPC));
#endif

	FAILEDCHECK(Ready_Layer_ActionCamera(ID_UINT(CAMERA)));


	//// Point Instance Test
	//GAMEINSTANCE->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(EFFECT, EFFECT_PARTICLE_POINT), ID_UINT(EFFECT, EFFECT_PARTICLE_POINT));

	//// Mesh Effect Test
	//MESHEFFECT_DESC MeshEffect_Desc;
	//MeshEffect_Desc.iBillBoardMode = false;
	//MeshEffect_Desc.bIsClampSampling = false;
	//MeshEffect_Desc.bIsColorFilled = false;
	//MeshEffect_Desc.bIsLoop = true;
	//MeshEffect_Desc.fLifeTime = { 0.f, 100.f };
	//MeshEffect_Desc.fUVScale = { 0.f, 0.f };
	//MeshEffect_Desc.fUVSpeedXY = { 2.f, 0.f };
	//MeshEffect_Desc.iMaskIndex = 0;
	//MeshEffect_Desc.iMeshIndex = 13; // 돌맹이 16번째 모델
	//MeshEffect_Desc.iShaderPath = 0;
	//MeshEffect_Desc.vColor = { 1.f, 1.f, 1.f, 1.f };
	//MeshEffect_Desc.vRight = { 1.f, 0.f, 0.f };
	//MeshEffect_Desc.vUp = { 0.f, 1.f, 0.f };
	//MeshEffect_Desc.vLook = { 0.f, 0.f, 1.f };
	//MeshEffect_Desc.vPosition = { -1.f, -1.f, -1.f };
	//GAMEINSTANCE->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(EFFECT, EFFECT_MESH), ID_UINT(EFFECT, EFFECT_MESH), &MeshEffect_Desc);
	//USEGAMEINSTANCE;
	//pGameInstance->Set_SaturationRate(0.f);

	pGameInstance->Set_FogColor(XMVectorSet(0.85f, 0.85f, 1.f, 0.f));
	pGameInstance->Set_FogRate(1.7f);

	//pGameInstance->Set_Brightness(1.175f);
	//pGameInstance->Set_SaturationRate(1.35);
	m_fBrightness_Ori = 0.96f;
	m_fSaturationRate_Ori = 1.327f;
	pGameInstance->Set_Brightness(m_fBrightness_Ori);
	pGameInstance->Set_SaturationRate(m_fSaturationRate_Ori);
	m_fScreenTimeAcc = 0.f;

	// .7.7
	pGameInstance->Set_DOFRate(0.32f);

	for (size_t i = 0; i < NUMSUNLAYER; ++i)
	{
		Move_LightRevolution(m_pSun[i], XMLoadFloat4(&m_vLightRevolutionAxis), XMLoadFloat4(&m_pSun[i]->Get_LightDesc()->vPosition), 0.7f);
		Render_Light_Depth(m_pSun[i]);

		//ID3D11Resource* pBuffer = nullptr;
		//m_pSun[i]->Get_RendetTarget()->Get_SRV()->GetResource(&pBuffer);
		//SaveDDSTextureToFile(m_pContext.Get(), pBuffer, TEXT("../Bin/Resources/Textures/RenderTarget/Hight.dds"));
	}


	//pGameInstance->Creat_DDSFromRT(TEXT("Target_Depth"), TEXT("../Bin/Resources/Textures/RenderTarget/Depth.dds"));

	pGameInstance->Set_CursorFix(true);

	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	USEGAMEINSTANCE;

	Update_LightState();

	if (KEY_DOWN(VK_RETURN))
	{
		Load_UI("Fin");
	}
	//m_fTickTime_Record += fTimeDelta;
	//if (KEY_DOWN(VK_SPACE))
	//{
	//	pGameInstance->Play_ActionCamera(TEXT("Final_Cut_Scene"), 0.f);
	//	// 카메라 촬영
	//	//cout << "CamPos: " << pGameInstance->Get_CamPosition().x << ", " << pGameInstance->Get_CamPosition().y << ", " << pGameInstance->Get_CamPosition().z << " //// CamLook ";
	//	// COUT_CAM_POS_LOOKPOS(pGameInstance->Get_CurrentCamera()->Get_Transform()->Get_WorldMatrix().r[3], pGameInstance->Get_CurrentCamera()->Get_Transform()->Get_WorldMatrix().r[2]);
	//	// cout << " /// UntilTime: " << m_fTickTime_Record << endl;
	//}


	//if (KEY_DOWN('Z'))
	//{
		//CPortal::PORTALDESC PortalDesc;
		//PortalDesc.m_vPos = _float4(138.44f, 22.38f, 118.12f, 1.f);
		//PortalDesc.m_vTargetPos = _float4(295.f, 0.f, 240.f, 1.f);
		//PortalDesc.m_bUseAble = true;
		//PortalDesc.m_ePortalType = PORTAL_VILLAGE;
		//
		//pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(MAP, MAP_PORTAL), ID_UINT(MAP, MAP_PORTAL), &PortalDesc);


	//}

	if (m_bStartCreateCrystal)
		CreateCrystal(fTimeDelta);

	//pGameInstance->Set_Brightness(DEBUGFLOAT4(0).x);
	//pGameInstance->Set_SaturationRate(DEBUGFLOAT4(0).y);

	//pGameInstance->Set_RadialBlur(_float2(0.5f, 0.5f), DEBUGFLOAT4(0).x, DEBUGFLOAT4(0).y, DEBUGFLOAT4(0).z);

	//if (KEY_DOWN('K'))
	//{
	////	//pGameInstance->Play_ActionCamera(TEXT("Test0"), 1.5f);
	////	//pGameInstance->Play_MAC(TEXT("MC_Test"), 1.5f);

	////	//pGameInstance->Set_CurrentCamera(m_pTempCamera);
	////	//((CCamera_Action*)m_pTempCamera)->Start_ActionCamera();

	//	pGameInstance->Add_RadialBlur(_float2(0.5f, 0.5f), 10.f, 1.f, 1.f, 1.f, DEBUGFLOAT4(0).w, false, true);
	//	pGameInstance->Add_RadialCA(_float2(0.5f, 0.5f), DEBUGFLOAT4(0), 4.f, 1.f);
	//	pGameInstance->Add_DirCA(_float2(DEBUGFLOAT4(1).x, DEBUGFLOAT4(1).y), DEBUGFLOAT4(0), 1.f, 1.f);
	//	pGameInstance->Add_Camera_Shaking(DEBUGVECTOR(1), DEBUGFLOAT4(1).y, DEBUGFLOAT, 1.f);
	//}


	_float fPlayTime = 5.f;
	if (m_bLeviathanPhase)
	{
		m_fScreenTimeAcc += fTimeDelta;
		if (fPlayTime < m_fScreenTimeAcc)
			m_fScreenTimeAcc = fPlayTime;
	}
	else
	{
		m_fScreenTimeAcc -= fTimeDelta;
		if (0.f > m_fScreenTimeAcc)
			m_fScreenTimeAcc = 0.f;
	}
	_float fCurBrightness = XMVectorGetX(CEasing_Utillity::sinfInOut(XMVectorSet(m_fBrightness_Ori, 0.f, 0.f, 0.f), XMVectorSet(0.7f, 0.f, 0.f, 0.f), m_fScreenTimeAcc, fPlayTime));
	_float fCurSaturationRate = XMVectorGetX(CEasing_Utillity::sinfInOut(XMVectorSet(m_fSaturationRate_Ori, 0.f, 0.f, 0.f), XMVectorSet(0.7f, 0.f, 0.f, 0.f), m_fScreenTimeAcc, fPlayTime));
	pGameInstance->Set_Brightness(fCurBrightness);
	pGameInstance->Set_SaturationRate(fCurSaturationRate);

	if (KEY_DOWN('K'))
	{
		m_bLeviathanPhase = true;

		//pGameInstance->Set_NextBrightness(0.7f, 1.f);
		//pGameInstance->Set_NextSaturationRate(0.7f, 1.f);
	}
	if (KEY_DOWN('L'))
	{
		m_bLeviathanPhase = false;
		//pGameInstance->Set_NextBrightness(m_fBrightness_Ori, 1.f);
		//pGameInstance->Set_NextSaturationRate(m_fSaturationRate_Ori, 1.f);
	}


	//if (KEY_DOWN('L'))
	//{
	//	pGameInstance->Stop_RadialBlur();
	//}


	//pGameInstance->Set_Debugint(0, DEBUGFLOAT4(0).x);
	//pGameInstance->Set_Debugfloat(0, DEBUGFLOAT4(0).x);
	//pGameInstance->Set_Debugfloat(1, DEBUGFLOAT4(0).y);
	//pGameInstance->Set_Debugfloat(2, DEBUGFLOAT4(0).z);
	//pGameInstance->Set_Debugfloat(3, DEBUGFLOAT4(1).y);
	//pGameInstance->Set_Debugfloat(4, DEBUGFLOAT4(1).z);
	//pGameInstance->Set_Debugfloat(5, DEBUGFLOAT4(1).y);

	//pGameInstance->Set_Debugfloat(1, DEBUGFLOAT4(0).y);
	//pGameInstance->Set_Debugfloat(2, DEBUGFLOAT4(0).z);
	//pGameInstance->Set_Debugfloat(3, DEBUGFLOAT4(0).w);

	//pGameInstance->Set_Brightness(DEBUGFLOAT4(1).x);
	//pGameInstance->Set_SaturationRate(DEBUGFLOAT4(1).y);

	//pGameInstance->Set_Debugfloat(0, DEBUGFLOAT4(0).x);
	//pGameInstance->Set_Debugfloat(1, DEBUGFLOAT4(0).y);
	//pGameInstance->Set_Debugfloat(2, DEBUGFLOAT4(0).z);
	//pGameInstance->Set_Debugfloat(3, DEBUGFLOAT4(0).w);
	//pGameInstance->Set_Debugfloat(4, DEBUGFLOAT4(1).z);


	////m_pSun->Set_Position(DEBUGVECTOR(0));
	//for (size_t i = 0; i < NUMSUNLAYER; ++i)
	//{
	//	Render_Light_Depth(m_pSun[i]);
	//	//Move_LightRevolution(m_pSun[i], XMLoadFloat4(&m_vLightRevolutionAxis), XMLoadFloat4(&m_pSun[i]->Get_LightDesc()->vPosition), 0.01f * fTimeDelta);
	//	//Move_LightRevolution(m_pSun[i], XMLoadFloat4(&m_vLightRevolutionAxis), XMLoadFloat4(&m_pSun[i]->Get_LightDesc()->vPosition), DEBUGFLOAT);
	//	//pGameInstance->Add_Camera_Shaking(XMVectorSet(1.f, 0.f, 0.f, 0.f), 0.03f, 1.2f, 0.12f);
	//}


	//if (KEY_DOWN(VK_SPACE))
	//{
	//	pGameInstance->Set_NextCamera(m_Cameras[CAMERA_FREE][0], 2.5f);
	//}

	//if (KEY_DOWN(VK_DOWN))
	//{
	//	pGameIstance->Set_NextCamera(m_Cameras[CAMERA_TARGET_POSITION][0], 1.5f);
	//}

	//if (KEY_DOWN(VK_SPACE))
	//{
	//	pGameInstance->Change_Level(LEVEL_LOADING, CLevel_Loading::Create(LEVEL_EDIT_ANIM));
	//}

	//m_pTest->Set_Pos(DEBUGVECTOR);


	/////////////////////////////////////////////////////////////////////////////////////////////  
	// EFFECT TEST
	/////////////////////////////////////////////////////////////////////////////////////////////  
	/*static _float  TimeAcc = 0;
	TimeAcc += fTimeDelta;
	if (TimeAcc > 2.f)
	{
		USEGAMEINSTANCE;
		PARTICLE_DESC ParticleDesc;
		ParticleDesc.bIsColorFilled = false;
		ParticleDesc.bIsLoop = false;
		ParticleDesc.fLifeTime = { 0.f, 3.f };
		ParticleDesc.fRangeX = { 1.f, 2.f };
		ParticleDesc.fRangeY = { 1.f, 2.f };
		ParticleDesc.fRangeZ = { 1.f, 2.f };
		ParticleDesc.fSpeedX = { 1.f, 2.f };
		ParticleDesc.fSpeedY = { 1.f, 2.f };
		ParticleDesc.fSpeedZ = { 1.f, 2.f };
		ParticleDesc.iStartPointMode = 0;
		ParticleDesc.iBillBoardMode = true;
		ParticleDesc.iNumInstance = 20.f;
		ParticleDesc.vColor = _float4(1.f, 1.f, 1.f, 1.f);
		ParticleDesc.vPosition = _float3(0.f, 0.f, 0.f);
		pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(EFFECT, EFFECT_PARTICLE_RECT), ID_UINT(EFFECT, EFFECT_PARTICLE_RECT), &ParticleDesc);


		TimeAcc = 0.f;
	}*/
	/////////////////////////////////////////////////////////////////////////////////////////////  
	// EFFECT TEST End
	/////////////////////////////////////////////////////////////////////////////////////////////  
}


void CLevel_GamePlay::LateTick(_float TimeDelta)
{
	USEGAMEINSTANCE;
	pGameInstance->Check_Collider(ID_UINT(COL_PLAYER_ATTACK), ID_UINT(COL_ENEMY_HITRANGE));
	pGameInstance->Check_Collider(ID_UINT(COL_PLAYER_ATTACK_RANGE), ID_UINT(COL_ENEMY_HITRANGE));
	pGameInstance->Check_Collider(ID_UINT(COL_ENEMY_ATTACKRANGE), ID_UINT(COL_PLAYER_BODY));
	pGameInstance->Check_Collider(ID_UINT(COL_ENEMY_ATTACK), ID_UINT(COL_PLAYER_BODY));
	pGameInstance->Check_Collider(ID_UINT(COL_ENEMY_CHASERANGE), ID_UINT(COL_PLAYER_BODY));
	pGameInstance->Check_Collider(ID_UINT(COL_PLAYER_BODY), ID_UINT(COL_TRIGGER));
	pGameInstance->Check_Collider(ID_UINT(COL_PLAYER_BODY), ID_UINT(COL_NPC));
	pGameInstance->Check_Collider(ID_UINT(COL_PLAYER_BODY), ID_UINT(COL_TRIGGER_PORTAL));
	pGameInstance->Check_Collider(ID_UINT(COL_PLAYER_UIINTRC), ID_UINT(COL_ITEM));
	pGameInstance->Check_Collider(ID_UINT(COL_PLAYER_UIINTRC), ID_UINT(COL_ENEMY_ATTACKRANGE));
	pGameInstance->Check_Collider(ID_UINT(COL_ENEMY_VOLUME), ID_UINT(COL_ENEMY_VOLUME));
	pGameInstance->Check_Collider(ID_UINT(COL_PLAYER_VOLUME), ID_UINT(COL_ENEMY_VOLUME));
	pGameInstance->Check_Collider(ID_UINT(COL_PLAYER_BODY), ID_UINT(COL_ENEMY_BOSS_VOLUME));
	pGameInstance->Check_Collider(ID_UINT(COL_PLAYER_BODY), ID_UINT(COL_REGION));
	pGameInstance->Check_Collider(ID_UINT(COL_PLAYER_EVASION), ID_UINT(COL_ENEMY_HITRANGE));
	pGameInstance->Check_Collider(ID_UINT(COL_PLAYER_EVASION), ID_UINT(COL_PLAYER_BODY));
	pGameInstance->Check_Collider(ID_UINT(COL_PLAYER_BODY), ID_UINT(COL_MINIGAME));

}

HRESULT CLevel_GamePlay::Render()
{
	if (FAILED(__super::Render()))
		__debugbreak();

	SetWindowText(g_hWnd, TEXT("Level GamePlay"));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Lights()
{
	USEGAMEINSTANCE;
	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	_vector vLightPos, vLightRevolutionNormal;
	_vector vSunDir = XMVectorSet(0.f, 3.f, 1.f, 0.f);

	for (size_t i = 0; i < NUMSUNLAYER; ++i)
	{
		//vLightPos = XMVector3Normalize(XMVectorSet(0.f, 3.f, 1.f, 0.f)) * 300.f;
		//vLightPos = XMVector3Normalize(XMVectorSet(0.f, 3.f, 1.f, 0.f)) * powf(4.f, i) * 10.f;
		//vLightPos = XMVector3Normalize(XMVectorSet(0.f, 5.f, 1.f, 0.f)) * 600.f;
		//vLightPos = XMVector3Normalize(XMVectorSet(0.f, 5.f, 1.f, 0.f)) * 1500.f;
		vLightPos = XMVector3Normalize(XMVectorSet(0.f, 5.f, 1.f, 0.f)) * 1000.f;
		//vLightPos = XMVector3Normalize(XMVectorSet(0.f, 1.f, 0.f, 0.f)) * 1000.f;
		vLightRevolutionNormal = XMVector3Normalize(XMVector3Cross(vLightPos, XMVectorSet(1.f, 0.f, 0.f, 0.f)));
		XMStoreFloat4(&m_vLightRevolutionAxis, vLightRevolutionNormal);


		vLightPos = XMVectorSetW(vLightPos + XMVectorSet(256.f, 0.f, 256.f, 1.f), 1.f);
		//vLightPos = XMVectorSetW(vLightPos + XMVectorSet(256.f, 0.f, 256.001f, 1.f), 1.f);
		//LightDesc.vPosition = _float4(100.f, 100.f, 0.f, 1.f);
		XMStoreFloat4(&LightDesc.vPosition, vLightPos);
		LightDesc.fRange = 10000.f;
		//LightDesc.eType = tagLightDesc::TYPE_DIRECTIONAL;
		LightDesc.eType = tagLightDesc::TYPE_POINT;
		//LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
		XMStoreFloat4(&LightDesc.vDirection, XMVector3Normalize(XMVectorSet(256.f, 10.f, 256.f, 1.f) - vLightPos));
		//LightDesc.vDiffuse = _float4(231.f / 255.f, 183.f / 255.f, 175.f / 255.f, 1.f);
		LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
		LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
		LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

		// Light Option
		LightDesc.eOption = tagLightDesc::OPTION_SUN;
		//LightDesc.iDepthCX = 1280 * 6;
		//LightDesc.iDepthCY = 720 * 6;
		LightDesc.iDepthCX = 1280;
		LightDesc.iDepthCY = 720;


		CLight_Sun* pSun = CLight_Sun::Create(LightDesc);
		NULLCHECK(m_pSun[i] = pGameInstance->Add_Light(pSun));
		//NULLCHECK(m_pSun[i] = pGameInstance->Add_Light(LightDesc));
		//if (i < NUMSUNLAYER - 1)
		//	m_pSun[i]->Set_UseForShadow(true);
		pGameInstance->Set_Sun(m_pSun[i]);
		m_pSun[i]->Set_ProjMatrix(XMMatrixPerspectiveFovLH(XMConvertToRadians(40.f), g_iWinCX / g_iWinCY, g_fNear, 1500.f));
		m_pSun[i]->Set_Far(3000.f);
	}

	_matrix Temp = XMMatrixPerspectiveFovLH(XMConvertToRadians(40.f), g_iWinCX / g_iWinCY, g_fNear, 1000.f);

#if defined(_PLAYER)
	LightDesc.eOption = tagLightDesc::OPTION_TARGET;
	LightDesc.iDepthCX = g_iWinCX;
	LightDesc.iDepthCY = g_iWinCY;
	CLight_Target* pTargetLight = CLight_Target::Create(LightDesc);
	NULLCHECK(m_pPlayerLight = pGameInstance->Add_Light(pTargetLight));
	m_pPlayerLight->Set_ProjMatrix(XMMatrixPerspectiveFovLH(XMConvertToRadians(8.f), g_iWinCX / g_iWinCY, g_fNear, g_fFar));
	m_pPlayerLight->Set_UseForShadow(true);
#endif

	m_pDepthShader = (CShader*)pGameInstance->Clone_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXMODEL));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Sky(ID_UINT iID)
{
	USEGAMEINSTANCE;

	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iID), ID_UINT(SKY));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(ID_UINT iID)
{

	USEGAMEINSTANCE;
	CCamera::CAMERADESC CameraDesc;

	//CameraDesc.vEye = _float4(0.0f, 10.f, -10.f, 1.f);
	//CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.vEye = _float4(0.0f, 0.f, -1.f, 1.f);
	CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);

	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = g_fNear;
	CameraDesc.fFar = g_fFar;

	m_Cameras[CAMERA_FREE].push_back(static_cast<CCamera*>(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iID), ID_UINT(CAMERA, CAMERA_FREE), &CameraDesc)));
	NULLCHECK(m_Cameras[CAMERA_FREE].back());


	m_Cameras[CAMERA_TARGET_POSITION].push_back(static_cast<CCamera*>(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iID), ID_UINT(CAMERA, CAMERA_TARGET_POSITION), &CameraDesc)));
	NULLCHECK(m_Cameras[CAMERA_TARGET_POSITION].back());
	/*CCamera* pCamera_Fixed = static_cast<CCamera*>(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iID), ID_UINT(CAMERA, CAMERA_FIXED), &CameraDesc));
	NULLCHECK(pCamera_Fixed);*/

	/*CameraDesc.vEye = _float4(310.4f, 12.45f, 241.9f, 1.f);
	CameraDesc.vAt = _float4(307.3f, 12.9f, 235.5f, 1.f);*/
	CameraDesc.vEye = _float4(310.0f, 12.75f, 240.9f, 1.f);
	CameraDesc.vAt = _float4(309.47f, 12.9f, 237.67f, 1.f);

	m_pInvenCamera = (CCamera*)pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iID), ID_UINT(CAMERA, CAMERA_FIXED), &CameraDesc);

	CameraDesc.vEye = _float4(310.72f, 12.45f, 240.2f, 1.f);
	CameraDesc.vAt = _float4(312.41f, 12.5f, 238.2f, 1.f);

	m_pEquipCamera = (CCamera*)pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iID), ID_UINT(CAMERA, CAMERA_FIXED), &CameraDesc);

	//m_Cameras[CAMERA_TARGET_POSITION_LERP].push_back(static_cast<CCamera*>(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iID), ID_UINT(CAMERA, CAMERA_TARGET_POSITION_LERP), &CameraDesc)));
	//NULLCHECK(m_Cameras[CAMERA_TARGET_POSITION_LERP].back());

	m_pPlayerTargetCamera = (CCamera*)pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iID), ID_UINT(CAMERA, CAMERA_TARGET_POSITION_LERP), &CameraDesc);
	pGameInstance->Set_CurrentCamera(m_pPlayerTargetCamera);

	CameraDesc.vEye = _float4(285.46f, 12.f, 251.77f, 1.f);
	CameraDesc.vAt = _float4(284.f, 11.5f, 250.f, 1.f); // 284

	NULLCHECK(m_pCookCamera = dynamic_cast<CCamera*>(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iID), ID_UINT(CAMERA, CAMERA_FIXED), &CameraDesc)));

	CameraDesc.vEye = _float4(279.21f, 14.82f, 244.34f, 1.f);
	CameraDesc.vAt = _float4(277.46f, 14.23f, 244.34f, 1.f); // 284

	NULLCHECK(m_pShirleyCamera = dynamic_cast<CCamera*>(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iID), ID_UINT(CAMERA, CAMERA_FIXED), &CameraDesc)));

	//pGameInstance->Set_CurrentCamera(m_Cameras[CAMERA_TARGET_POSITION]);
	//pGameInstance->Set_CurrentCamera(m_Cameras[CAMERA_FREE].back());
	//pGameInstance->Set_CurrentCamera(m_Cameras[CAMERA_FREE].back());
	//pGameInstance->Set_CurrentCamera(m_Cameras[CAMERA_TARGET_POSITION].back());
	//pGameInstance->Set_CurrentCamera(m_Cameras[CAMERA_TARGET_POSITION_LERP].back());

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(ID_UINT iID)
{
	USEGAMEINSTANCE;

#if defined(_TERRAIN)
	CGameObject* pMapObj = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(MAP, MAP_TERRAIN), ID_UINT(MAP, MAP_TERRAIN));
	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(MAP, MAP_WATER), ID_UINT(MAP, MAP_WATER));
	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(MAP, MAP_WATER_BASE), ID_UINT(MAP, MAP_WATER_BASE));
	//	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(MAP, MAP_NAVI), ID_UINT(MAP, MAP_NAVI));
	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(MAP, MAP_TRIGGER), ID_UINT(MAP, MAP_TRIGGER));

	for (size_t i = 0; i < NUMSUNLAYER; ++i)
		m_pSun[i]->Add_RenderObject(pMapObj);
#endif

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(ID_UINT iID)
{
	/*CGameObject* pCrystal_Robella = GAMEINSTANCE->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_NORMAL, LEVIATHAN_CRYSTAL), ID_UINT(ENEMY, ENEMY_NORMAL, LEVIATHAN_CRYSTAL), &UNITDESC(ELEMENTAL_FIRE, 5000.f, 0.f, 1.f));
	NULLCHECK(pCrystal_Robella);
	pCrystal_Robella->Set_Pos(XMVectorSet(295.f, 0.f, 240.f, 1.f));
	((CNavigation*)pCrystal_Robella->Get_Component(ID_UINT(NAVIGATION)))->Find_CurrentIndex(((CTransform*)pCrystal_Robella->Get_Transform()));
	((CNavigation*)pCrystal_Robella->Get_Component(ID_UINT(NAVIGATION)))->SetUp_OnTerrain(((CTransform*)pCrystal_Robella->Get_Transform()));*/

	//NULLCHECK(GAMEINSTANCE->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iID), ID_UINT(iID)));
	CGameObject* pPlayer = nullptr;
	pPlayer = GAMEINSTANCE->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iID), ID_UINT(iID),
		&CPlayer::PLAYERDESC(ELEMENTAL_DEFAULT, 10000.f, 10.f, 1.f, 3.f, 1.f, 1.f));

	//pPlayer->Set_Pos(XMVectorSet(256.f, 0.f, 256.f, 1.f)); // 맵 중앙
	//pPlayer->Set_Pos(XMVectorSet(107.f, 33.f, 346.f, 1.f));//몬스터광야
	pPlayer->Set_Pos(XMVectorSet(295.f, 0.f, 240.f, 1.f));//마을
	//pPlayer->Set_Pos(XMVectorSet(430.13f, 152.f, 289.51f, 1.f));//웨버
	//pPlayer->Set_Pos(XMVectorSet(138.44f, 22.39f, 123.12f, 1.f));//로벨라
	//pPlayer->Set_Pos(XMVectorSet(241.78f, 22.f, 142.46f, 1.f));//로벨라초입
	//pPlayer->Set_Pos(XMVectorSet(100.f, 153.f, 430.27, 1.f));//용 보스
	//pPlayer->Set_Pos(XMVectorSet(310.f, 12.f, 239.f, 1.f)); //인벤토리
	//pPlayer->Set_Pos(XMVectorSet(312.41f, 12.f, 238.2f, 1.f));//장비
	((CCamera_Target_Position*)m_Cameras[CAMERA_TARGET_POSITION].back())->Set_TargetObject(pPlayer);
	((CCamera_Target_Position*)m_Cameras[CAMERA_TARGET_POSITION].back())->Set_Offset(XMVectorSet(0.f, 2.f, -4.f, 0.f));

	//((CCamera_Target_Position_Lerp*)m_Cameras[CAMERA_TARGET_POSITION_LERP].back())->Set_TargetObject(pPlayer);
	//((CCamera_Target_Position_Lerp*)m_Cameras[CAMERA_TARGET_POSITION_LERP].back())->Set_Offset(XMVectorSet(0.f, 2.f, -4.f, 0.f));
	//((CCamera_Target_Position_Lerp*)m_Cameras[CAMERA_TARGET_POSITION_LERP].back())->Set_PosLerpRate(0.1f);
	//((CCamera_Target_Position_Lerp*)m_Cameras[CAMERA_TARGET_POSITION_LERP].back())->Set_LookLerpRate(0.3f);

	((CCamera_Target_Position_Lerp*)m_pPlayerTargetCamera)->Set_TargetObject(pPlayer);
	((CCamera_Target_Position_Lerp*)m_pPlayerTargetCamera)->Set_Offset(XMVectorSet(0.f, 1.8f, -3.6f, 0.f));
	((CCamera_Target_Position_Lerp*)m_pPlayerTargetCamera)->Set_PosLerpRate(0.1f);
	((CCamera_Target_Position_Lerp*)m_pPlayerTargetCamera)->Set_LookLerpRate(0.3f);

#if defined(_MAP)
	((CNavigation*)pPlayer->Get_Component(ID_UINT(NAVIGATION)))->Find_CurrentIndex(pPlayer->Get_Transform());;
#endif

#if defined(_PLAYER)
	m_pPlayer = pPlayer;
	for (size_t i = 0; i < NUMSUNLAYER; ++i)
	{
		//m_pSun[i]->Add_RenderObject(pPlayer);
		m_pSun[i]->Set_LookAtObject(pPlayer);
	}

	m_pPlayerLight->Add_RenderObject(pPlayer);
	((CLight_Target*)m_pPlayerLight)->Set_TargetObject(pPlayer);
	((CPlayer*)pPlayer)->Set_WeaponShadow(m_pPlayerLight);

#endif

#ifdef _TEST_HAN
	GAMEINSTANCE->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(EFFECT), ID_UINT(EFFECT, EFFECT_TRAIL));
#endif


	//((CCamera_Action*)m_pTempCamera)->Set_TargetObejct(pPlayer);


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_EnemyNormal(ID_UINT iID)
{
	USEGAMEINSTANCE;
	//_float TestPosition = 1.f;

	CGameObject* pEnemyNormal = nullptr;
	//CGameObject* pWeapon = nullptr;


#ifdef _TEST_JJ
#pragma region Enemy_Gang_Swarm

	//TestPosition = 3.f;
	//NULLCHECK(pEnemyNormal = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iID), ID_UINT(ENEMY, ENEMY_NORMAL, HYENAGANG_MINIONS),
	//	&UNITDESC(ELEMENTAL_DEFAULT, 1000.f, 10.f, 1.f)));
	//pEnemyNormal->Set_Pos(XMVectorSet(163 + TestPosition, 33.f, 333.f + TestPosition, 1.f));
	//((CEnemy*)pEnemyNormal)->Set_StartPos(XMVectorSet(163 + TestPosition, 33.f, 333.f + TestPosition, 1.f));
	//((CEnemy*)pEnemyNormal)->Set_PreHp(((CEnemy*)pEnemyNormal)->Get_UnitInfo()->fHP);
	//((CNavigation*)pEnemyNormal->Get_Component(ID_UINT(NAVIGATION)))->Find_CurrentIndex(((CTransform*)pEnemyNormal->Get_Transform()));
	//((CNavigation*)pEnemyNormal->Get_Component(ID_UINT(NAVIGATION)))->SetUp_OnTerrain(((CTransform*)pEnemyNormal->Get_Transform()));

	//TestPosition = 0.f;
	//NULLCHECK(pEnemyNormal = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iID), ID_UINT(ENEMY, ENEMY_NORMAL, HYENAGANG_SHOOTER),
	//	&UNITDESC(ELEMENTAL_DEFAULT, 1000.f, 10.f, 1.f)));
	//pEnemyNormal->Set_Pos(XMVectorSet(163 + TestPosition, 33.f, 333.f + TestPosition, 1.f));
	//((CEnemy*)pEnemyNormal)->Set_StartPos(XMVectorSet(163 + TestPosition, 33.f, 333.f + TestPosition, 1.f));
	//((CEnemy*)pEnemyNormal)->Set_PreHp(((CEnemy*)pEnemyNormal)->Get_UnitInfo()->fHP);
	//((CNavigation*)pEnemyNormal->Get_Component(ID_UINT(NAVIGATION)))->Find_CurrentIndex(((CTransform*)pEnemyNormal->Get_Transform()));
	//((CNavigation*)pEnemyNormal->Get_Component(ID_UINT(NAVIGATION)))->SetUp_OnTerrain(((CTransform*)pEnemyNormal->Get_Transform()));

	//TestPosition = -3.f;
	//NULLCHECK(pEnemyNormal = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iID), ID_UINT(ENEMY, ENEMY_NORMAL, GANGSTER_CAP),
	//	&UNITDESC(ELEMENTAL_DEFAULT, 1000.f, 10.f, 1.f)));
	//pEnemyNormal->Set_Pos(XMVectorSet(163 + TestPosition, 33.f, 333.f + TestPosition, 1.f));
	//((CEnemy*)pEnemyNormal)->Set_StartPos(XMVectorSet(163 + TestPosition, 33.f, 333.f + TestPosition, 1.f));
	//((CEnemy*)pEnemyNormal)->Set_PreHp(((CEnemy*)pEnemyNormal)->Get_UnitInfo()->fHP);
	//((CNavigation*)pEnemyNormal->Get_Component(ID_UINT(NAVIGATION)))->Find_CurrentIndex(((CTransform*)pEnemyNormal->Get_Transform()));
	//((CNavigation*)pEnemyNormal->Get_Component(ID_UINT(NAVIGATION)))->SetUp_OnTerrain(((CTransform*)pEnemyNormal->Get_Transform()));

#pragma endregion Enemy_Gang_Swarm

#pragma region Enemy_Mutation_Swarm

	//TestPosition = 3.f;
	//NULLCHECK(pEnemyNormal = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iID), ID_UINT(ENEMY, ENEMY_NORMAL, MUTATION),
	//	&UNITDESC(ELEMENTAL_DEFAULT, 1000.f, 10.f, 1.f)));
	//pEnemyNormal->Set_Pos(XMVectorSet(127.f + TestPosition, 33.f, 358.f + TestPosition, 1.f));
	//((CEnemy*)pEnemyNormal)->Set_StartPos(XMVectorSet(127.f + TestPosition, 33.f, 358.f + TestPosition, 1.f));
	//((CEnemy*)pEnemyNormal)->Set_PreHp(((CEnemy*)pEnemyNormal)->Get_UnitInfo()->fHP);
	//((CNavigation*)pEnemyNormal->Get_Component(ID_UINT(NAVIGATION)))->Find_CurrentIndex(((CTransform*)pEnemyNormal->Get_Transform()));
	//((CNavigation*)pEnemyNormal->Get_Component(ID_UINT(NAVIGATION)))->SetUp_OnTerrain(((CTransform*)pEnemyNormal->Get_Transform()));

	//TestPosition = 0.f;
	//NULLCHECK(pEnemyNormal = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iID), ID_UINT(ENEMY, ENEMY_NORMAL, MUTATION_MECHANIC),
	//	&UNITDESC(ELEMENTAL_DEFAULT, 1000.f, 10.f, 1.f)));
	//pEnemyNormal->Set_Pos(XMVectorSet(127.f + TestPosition, 33.f, 358.f + TestPosition, 1.f));
	//((CEnemy*)pEnemyNormal)->Set_StartPos(XMVectorSet(127.f + TestPosition, 33.f, 358.f + TestPosition, 1.f));
	//((CEnemy*)pEnemyNormal)->Set_PreHp(((CEnemy*)pEnemyNormal)->Get_UnitInfo()->fHP);
	//((CNavigation*)pEnemyNormal->Get_Component(ID_UINT(NAVIGATION)))->Find_CurrentIndex(((CTransform*)pEnemyNormal->Get_Transform()));
	//((CNavigation*)pEnemyNormal->Get_Component(ID_UINT(NAVIGATION)))->SetUp_OnTerrain(((CTransform*)pEnemyNormal->Get_Transform()));

	//TestPosition = 15.f;
	//NULLCHECK(pEnemyNormal = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iID), ID_UINT(ENEMY, ENEMY_NORMAL, HABAKA_ADMIRAL),
	//	&UNITDESC(ELEMENTAL_DEFAULT, 5000.f, 10.f, 1.f)));
	//pEnemyNormal->Set_Pos(XMVectorSet(127.f + TestPosition, 33.f, 358.f + TestPosition, 1.f));
	//((CEnemy*)pEnemyNormal)->Set_StartPos(XMVectorSet(127.f + TestPosition, 33.f, 358.f + TestPosition, 1.f));
	//((CEnemy*)pEnemyNormal)->Set_PreHp(((CEnemy*)pEnemyNormal)->Get_UnitInfo()->fHP);
	//((CNavigation*)pEnemyNormal->Get_Component(ID_UINT(NAVIGATION)))->Find_CurrentIndex(((CTransform*)pEnemyNormal->Get_Transform()));
	//((CNavigation*)pEnemyNormal->Get_Component(ID_UINT(NAVIGATION)))->SetUp_OnTerrain(((CTransform*)pEnemyNormal->Get_Transform()));

#pragma endregion Enemy_Mutation_Swarm

#pragma region Enemy_MechanicalCity_Swarm

	//TestPosition = 3.f;
	//NULLCHECK(pEnemyNormal = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iID), ID_UINT(ENEMY, ENEMY_NORMAL, MECHANICALCITY_GUNNER),
	//	&UNITDESC(ELEMENTAL_DEFAULT, 1000.f, 10.f, 1.f)));
	//pEnemyNormal->Set_Pos(XMVectorSet(165.f + TestPosition, 33.f, 362.f + TestPosition, 1.f));
	//((CEnemy*)pEnemyNormal)->Set_StartPos(XMVectorSet(165.f + TestPosition, 33.f, 362.f + TestPosition, 1.f));
	//((CEnemy*)pEnemyNormal)->Set_PreHp(((CEnemy*)pEnemyNormal)->Get_UnitInfo()->fHP);
	//((CNavigation*)pEnemyNormal->Get_Component(ID_UINT(NAVIGATION)))->Find_CurrentIndex(((CTransform*)pEnemyNormal->Get_Transform()));
	//((CNavigation*)pEnemyNormal->Get_Component(ID_UINT(NAVIGATION)))->SetUp_OnTerrain(((CTransform*)pEnemyNormal->Get_Transform()));

	//TestPosition = 0.f;
	//NULLCHECK(pEnemyNormal = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iID), ID_UINT(ENEMY, ENEMY_NORMAL, MECHANICALCITY_SWORDGHOST),
	//	&UNITDESC(ELEMENTAL_DEFAULT, 1000.f, 10.f, 1.f)));
	//pEnemyNormal->Set_Pos(XMVectorSet(165.f + TestPosition, 33.f, 362.f + TestPosition, 1.f));
	//((CEnemy*)pEnemyNormal)->Set_StartPos(XMVectorSet(165.f + TestPosition, 33.f, 362.f + TestPosition, 1.f));
	//((CEnemy*)pEnemyNormal)->Set_PreHp(((CEnemy*)pEnemyNormal)->Get_UnitInfo()->fHP);
	//((CNavigation*)pEnemyNormal->Get_Component(ID_UINT(NAVIGATION)))->Find_CurrentIndex(((CTransform*)pEnemyNormal->Get_Transform()));
	//((CNavigation*)pEnemyNormal->Get_Component(ID_UINT(NAVIGATION)))->SetUp_OnTerrain(((CTransform*)pEnemyNormal->Get_Transform()));

	//TestPosition = -3.f;
	//NULLCHECK(pEnemyNormal = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iID), ID_UINT(ENEMY, ENEMY_NORMAL, MECHANICALCITY_DOUBLEEDGEDSWORDADEPT),
	//	&UNITDESC(ELEMENTAL_DEFAULT, 2000.f, 10.f, 1.f)));
	//pEnemyNormal->Set_Pos(XMVectorSet(165.f + TestPosition, 33.f, 362.f + TestPosition, 1.f));
	//((CEnemy*)pEnemyNormal)->Set_StartPos(XMVectorSet(165.f + TestPosition, 33.f, 362.f + TestPosition, 1.f));
	//((CEnemy*)pEnemyNormal)->Set_PreHp(((CEnemy*)pEnemyNormal)->Get_UnitInfo()->fHP);
	//((CNavigation*)pEnemyNormal->Get_Component(ID_UINT(NAVIGATION)))->Find_CurrentIndex(((CTransform*)pEnemyNormal->Get_Transform()));
	//((CNavigation*)pEnemyNormal->Get_Component(ID_UINT(NAVIGATION)))->SetUp_OnTerrain(((CTransform*)pEnemyNormal->Get_Transform()));

	//TestPosition = 6.f;
	//NULLCHECK(pEnemyNormal = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iID), ID_UINT(ENEMY, ENEMY_NORMAL, MECHANICALCITY_DOORKEEPER),
	//	&UNITDESC(ELEMENTAL_DEFAULT, 1000.f, 10.f, 1.f)));
	//pEnemyNormal->Set_Pos(XMVectorSet(165.f + TestPosition, 33.f, 362.f + TestPosition, 1.f));
	//((CEnemy*)pEnemyNormal)->Set_StartPos(XMVectorSet(165.f + TestPosition, 33.f, 362.f + TestPosition, 1.f));
	//((CEnemy*)pEnemyNormal)->Set_PreHp(((CEnemy*)pEnemyNormal)->Get_UnitInfo()->fHP);
	//((CNavigation*)pEnemyNormal->Get_Component(ID_UINT(NAVIGATION)))->Find_CurrentIndex(((CTransform*)pEnemyNormal->Get_Transform()));
	//((CNavigation*)pEnemyNormal->Get_Component(ID_UINT(NAVIGATION)))->SetUp_OnTerrain(((CTransform*)pEnemyNormal->Get_Transform()));

#pragma endregion Enemy_MechanicalCity_Swarm

#endif

#pragma region Enemy_LoadJson

	json jLoad;

	pGameInstance->Load_Json(pGameInstance->json_Complete_Path(towstring("Enemy/Monster").c_str()).c_str(), &jLoad);

	for (auto& ObjectData : jLoad)
	{
		GAMEOBJECT_DESC tInfo;
		tInfo.iObjectID = ObjectData["iObjectID"];
		tInfo.iModelID = ObjectData["iModelID"];

		tInfo.WorldMatrix.r[0] = XMVectorSet(ObjectData["vRight.x"], ObjectData["vRight.y"], ObjectData["vRight.z"], 0.f);
		tInfo.WorldMatrix.r[1] = XMVectorSet(ObjectData["vUp.x"], ObjectData["vUp.y"], ObjectData["vUp.z"], 0.f);
		tInfo.WorldMatrix.r[2] = XMVectorSet(ObjectData["vLook.x"], ObjectData["vLook.y"], ObjectData["vLook.z"], 0.f);
		tInfo.WorldMatrix.r[3] = XMVectorSet(ObjectData["vPosition.x"], ObjectData["vPosition.y"], ObjectData["vPosition.z"], 1.f);

		ID_UINT iID, iModelID;
		iID.iID = tInfo.iObjectID;
		iModelID.iID = tInfo.iModelID;

		if (iModelID.tNums.Num2 == GANGSTER_CAP)
		{
			pEnemyNormal = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, iID, iID, &UNITDESC(ELEMENTAL_DEFAULT, 5000.f, 10.f, 1.f));
		}
		else if (iModelID.tNums.Num2 == MECHANICALCITY_DOORKEEPER)
		{
			pEnemyNormal = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, iID, iID, &UNITDESC(ELEMENTAL_DEFAULT, 5000.f, 10.f, 1.f));
		}
		else if (iModelID.tNums.Num2 == HABAKA_ADMIRAL)
		{
			pEnemyNormal = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, iID, iID, &UNITDESC(ELEMENTAL_DEFAULT, 5000.f, 10.f, 1.f));
		}
		else
		{
			pEnemyNormal = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, iID, iID, &UNITDESC(ELEMENTAL_DEFAULT, 2000.f, 10.f, 1.f));
		}

		pGameInstance->Get_Objects(LEVEL_GAMEPLAY, iID)->back()->Get_Transform()->Set_WorldMatrix(tInfo.WorldMatrix);
		((CEnemy*)pEnemyNormal)->Set_PreHp(((CEnemy*)pEnemyNormal)->Get_UnitInfo()->fHP);
		((CEnemy*)pEnemyNormal)->Set_StartPos(tInfo.WorldMatrix.r[3]);
		((CNavigation*)pEnemyNormal->Get_Component(ID_UINT(NAVIGATION)))->Find_CurrentIndex(((CTransform*)pEnemyNormal->Get_Transform()));
		((CNavigation*)pEnemyNormal->Get_Component(ID_UINT(NAVIGATION)))->SetUp_OnTerrain(((CTransform*)pEnemyNormal->Get_Transform()));
	}
#pragma endregion Enemy_LoadJson
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_EnemyBoss(ID_UINT iID)
{
	USEGAMEINSTANCE;

	CGameObject* pEnemyBoss = nullptr;
	NULLCHECK(pEnemyBoss = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_FROSTDRAGON), ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_FROSTDRAGON)));
	NULLCHECK(pEnemyBoss = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_WEAVER), ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_WEAVER)));
	NULLCHECK(pEnemyBoss = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_ROBELLA), ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_ROBELLA)));
	NULLCHECK(pEnemyBoss = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_LEVIATHAN), ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_LEVIATHAN)));

	//pGameInstance->Get_Object(LEVEL_GAMEPLAY, ID_UINT(iID), 0)->Get_Transform()->Set_WorldMatrix();		//저장된 매트리스 불러다 셋팅

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Map(ID_UINT iID)
{
	USEGAMEINSTANCE;
	//CPortal::PORTALDESC PortalDesc;
	//PortalDesc.m_vPos = _float4(148.39f, 87.2f, 457.f, 1.f);
	//PortalDesc.m_vTargetPos = _float4(100.f, 160.f, 435.27, 1.f);
	//PortalDesc.m_bUseAble = true;
	//PortalDesc.m_ePortalType = PORTAL_DRAGON;

	//pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(MAP, MAP_PORTAL), ID_UINT(MAP, MAP_PORTAL), &PortalDesc); // 드래곤

	//CPortal::PORTALDESC PortalDesc;
	//PortalDesc.m_vPos = _float4(138.44f, 22.38f, 118.12f, 1.f);
	//PortalDesc.m_vTargetPos = _float4(295.f, 0.f, 240.f, 1.f);
	//PortalDesc.m_bUseAble = true;
	//PortalDesc.m_ePortalType = PORTAL_VILLAGE;

	//pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(MAP, MAP_PORTAL), ID_UINT(MAP, MAP_PORTAL), &PortalDesc); // 마을

	//CPortal::PORTALDESC PortalDesc;
	//PortalDesc.m_vPos = _float4(445.9f, 55.52f, 308.f, 1.f);
	//PortalDesc.m_vTargetPos = _float4(295.f, 0.f, 240.f, 1.f); // 웨버쪽 위치 네비깔리면 받아오기
	//PortalDesc.m_bUseAble = true;
	//PortalDesc.m_ePortalType = PORTAL_WEAVER;

	//pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(MAP, MAP_PORTAL), ID_UINT(MAP, MAP_PORTAL), &PortalDesc); // 웨버
	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(MAP, MAP_REGION), ID_UINT(MAP, MAP_REGION),
		&CRegion::REGIONDESC({ 279.75, 11.f, 224.7f,1.f }, { 100.f, 300.f, 100.f }, REGION_VILLAGE));

	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(MAP, MAP_REGION), ID_UINT(MAP, MAP_REGION),
		&CRegion::REGIONDESC({ 80.f, 20.f, 425.f, 1.f }, { 390.f, 400.f, 390.f }, REGION_DRAGON));

	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(MAP, MAP_REGION), ID_UINT(MAP, MAP_REGION),
		&CRegion::REGIONDESC({ 102.675, 0.f, 16.08f, 1.f }, { 290.f, 500.f, 290.f }, REGION_ROBELLA));

	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(MAP, MAP_REGION), ID_UINT(MAP, MAP_REGION),
		&CRegion::REGIONDESC({ 585.6f, 0.f, 205.47f, 1.f }, { 440.f, 400.f, 440.f }, REGION_WEAVER));

	//pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(MAP, MAP_REGION), ID_UINT(MAP, MAP_REGION));

#if 0
	/*For.Map*/
	m_iModelNum = 1;
	//pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(MAP, MAP_ROCK, MODEL_ROCK), ID_UINT(MAP, MAP_ROCK, MODEL_ROCK), &m_iModelNum);

	m_iModelNum = 2;
	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(MAP, MAP_TILE, MODEL_TILE), ID_UINT(MAP, MAP_TILE, MODEL_TILE), &m_iModelNum);
	m_iModelNum = 3;
	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(MAP, MAP_TILE, MODEL_TILE), ID_UINT(MAP, MAP_TILE, MODEL_TILE), &m_iModelNum);
	m_iModelNum = 4;
	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(MAP, MAP_TILE, MODEL_TILE), ID_UINT(MAP, MAP_TILE, MODEL_TILE), &m_iModelNum);
	m_iModelNum = 5;
	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(MAP, MAP_TILE, MODEL_TILE), ID_UINT(MAP, MAP_TILE, MODEL_TILE), &m_iModelNum);
	m_iModelNum = 6;
	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(MAP, MAP_TILE, MODEL_TILE), ID_UINT(MAP, MAP_TILE, MODEL_TILE), &m_iModelNum);
	m_iModelNum = 7;
	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(MAP, MAP_TILE, MODEL_TILE), ID_UINT(MAP, MAP_TILE, MODEL_TILE), &m_iModelNum);
#endif // 0

#if defined(_MAPINSTANCE)
#if defined(_VILLAGE_ISLAND)
	json jLoad;

	pGameInstance->Load_Json(pGameInstance->json_Complete_Path(towstring("Map/00.Map_Village_Island").c_str()).c_str(), &jLoad);
	//pGameInstance->Load_Json(pGameInstance->json_Complete_Path(towstring("Map/1221_Map_J").c_str()).c_str(), &jLoad);

	json jSourLoad = jLoad["MapObj"];

	for (auto& jLoadData : jSourLoad)
	{
		GAMEOBJECT_DESC MapDesc;

		MapDesc.iObjectID = jLoadData["iObjectID"];
		MapDesc.iModelID = jLoadData["iModelID"];

		MapDesc.WorldMatrix.r[0] = XMVectorSet(jLoadData["vRight.x"], jLoadData["vRight.y"], jLoadData["vRight.z"], 0.f);
		MapDesc.WorldMatrix.r[1] = XMVectorSet(jLoadData["vUp.x"], jLoadData["vUp.y"], jLoadData["vUp.z"], 0.f);
		MapDesc.WorldMatrix.r[2] = XMVectorSet(jLoadData["vLook.x"], jLoadData["vLook.y"], jLoadData["vLook.z"], 0.f);
		MapDesc.WorldMatrix.r[3] = XMVectorSet(jLoadData["vPosition.x"], jLoadData["vPosition.y"], jLoadData["vPosition.z"], 1.f);

		Add_MapObjData(MapDesc.iModelID, MapDesc);

		ID_UINT iID, iModelID;
		iID.iID = MapDesc.iObjectID;
		iModelID.iID = MapDesc.iModelID;
		_uint iModelNum = 0;
		iModelNum = iModelID.tNums.Num2 + iModelID.tNums.Num3 * 255;
	}
#endif

#if defined(_STONE_ISLAND)
	json jLoad2;

	pGameInstance->Load_Json(pGameInstance->json_Complete_Path(towstring("Map/01.Map_Stone_Island").c_str()).c_str(), &jLoad2);
	//pGameInstance->Load_Json(pGameInstance->json_Complete_Path(towstring("Map/1221_Map_J").c_str()).c_str(), &jLoad);

	json jSourLoad2 = jLoad2["MapObj"];

	for (auto& jLoadData : jSourLoad2)
	{
		GAMEOBJECT_DESC MapDesc;

		MapDesc.iObjectID = jLoadData["iObjectID"];
		MapDesc.iModelID = jLoadData["iModelID"];

		MapDesc.WorldMatrix.r[0] = XMVectorSet(jLoadData["vRight.x"], jLoadData["vRight.y"], jLoadData["vRight.z"], 0.f);
		MapDesc.WorldMatrix.r[1] = XMVectorSet(jLoadData["vUp.x"], jLoadData["vUp.y"], jLoadData["vUp.z"], 0.f);
		MapDesc.WorldMatrix.r[2] = XMVectorSet(jLoadData["vLook.x"], jLoadData["vLook.y"], jLoadData["vLook.z"], 0.f);
		MapDesc.WorldMatrix.r[3] = XMVectorSet(jLoadData["vPosition.x"], jLoadData["vPosition.y"], jLoadData["vPosition.z"], 1.f);

		Add_MapObjData(MapDesc.iModelID, MapDesc);

		ID_UINT iID, iModelID;
		iID.iID = MapDesc.iObjectID;
		iModelID.iID = MapDesc.iModelID;
		_uint iModelNum = 0;
		iModelNum = iModelID.tNums.Num2 + iModelID.tNums.Num3 * 255;
	}
#endif

#if defined(_DRAGON)
	json jLoad_Struct;

	pGameInstance->Load_Json(pGameInstance->json_Complete_Path(towstring("Map/02.Map_Dragon").c_str()).c_str(), &jLoad_Struct);
	//pGameInstance->Load_Json(pGameInstance->json_Complete_Path(towstring("Map/1221_Map_J").c_str()).c_str(), &jLoad);

	json jSourLoad_Struct = jLoad_Struct["MapObj"];

	for (auto& jLoadData : jSourLoad_Struct)
	{
		GAMEOBJECT_DESC MapDesc;

		MapDesc.iObjectID = jLoadData["iObjectID"];
		MapDesc.iModelID = jLoadData["iModelID"];

		MapDesc.WorldMatrix.r[0] = XMVectorSet(jLoadData["vRight.x"], jLoadData["vRight.y"], jLoadData["vRight.z"], 0.f);
		MapDesc.WorldMatrix.r[1] = XMVectorSet(jLoadData["vUp.x"], jLoadData["vUp.y"], jLoadData["vUp.z"], 0.f);
		MapDesc.WorldMatrix.r[2] = XMVectorSet(jLoadData["vLook.x"], jLoadData["vLook.y"], jLoadData["vLook.z"], 0.f);
		MapDesc.WorldMatrix.r[3] = XMVectorSet(jLoadData["vPosition.x"], jLoadData["vPosition.y"], jLoadData["vPosition.z"], 1.f);

		Add_MapObjData(MapDesc.iModelID, MapDesc);

		ID_UINT iID, iModelID;
		iID.iID = MapDesc.iObjectID;
		iModelID.iID = MapDesc.iModelID;
		_uint iModelNum = 0;
		iModelNum = iModelID.tNums.Num2 + iModelID.tNums.Num3 * 255;
	}
#endif

#if defined(_ROBELLA_ISLAND)

	json jLoad4;

	pGameInstance->Load_Json(pGameInstance->json_Complete_Path(towstring("Map/03.Map_Robella_Island").c_str()).c_str(), &jLoad4);
	//pGameInstance->Load_Json(pGameInstance->json_Complete_Path(towstring("Map/1221_Map_J").c_str()).c_str(), &jLoad);

	json jSourLoad4 = jLoad4["MapObj"];

	for (auto& jLoadData : jSourLoad4)
	{
		GAMEOBJECT_DESC MapDesc;

		MapDesc.iObjectID = jLoadData["iObjectID"];
		MapDesc.iModelID = jLoadData["iModelID"];

		MapDesc.WorldMatrix.r[0] = XMVectorSet(jLoadData["vRight.x"], jLoadData["vRight.y"], jLoadData["vRight.z"], 0.f);
		MapDesc.WorldMatrix.r[1] = XMVectorSet(jLoadData["vUp.x"], jLoadData["vUp.y"], jLoadData["vUp.z"], 0.f);
		MapDesc.WorldMatrix.r[2] = XMVectorSet(jLoadData["vLook.x"], jLoadData["vLook.y"], jLoadData["vLook.z"], 0.f);
		MapDesc.WorldMatrix.r[3] = XMVectorSet(jLoadData["vPosition.x"], jLoadData["vPosition.y"], jLoadData["vPosition.z"], 1.f);

		Add_MapObjData(MapDesc.iModelID, MapDesc);

		ID_UINT iID, iModelID;
		iID.iID = MapDesc.iObjectID;
		iModelID.iID = MapDesc.iModelID;
		_uint iModelNum = 0;
		iModelNum = iModelID.tNums.Num2 + iModelID.tNums.Num3 * 255;
	}

#endif // DEBUG

#if defined(_MECHANICALCITY_ISLAND)
	json jLoad6;

	pGameInstance->Load_Json(pGameInstance->json_Complete_Path(towstring("Map/04.Map_MechanicalCity_Island").c_str()).c_str(), &jLoad6);

	json jSourLoad6 = jLoad6["MapObj"];

	for (auto& jLoadData : jSourLoad6)
	{
		GAMEOBJECT_DESC MapDesc;

		MapDesc.iObjectID = jLoadData["iObjectID"];
		MapDesc.iModelID = jLoadData["iModelID"];

		MapDesc.WorldMatrix.r[0] = XMVectorSet(jLoadData["vRight.x"], jLoadData["vRight.y"], jLoadData["vRight.z"], 0.f);
		MapDesc.WorldMatrix.r[1] = XMVectorSet(jLoadData["vUp.x"], jLoadData["vUp.y"], jLoadData["vUp.z"], 0.f);
		MapDesc.WorldMatrix.r[2] = XMVectorSet(jLoadData["vLook.x"], jLoadData["vLook.y"], jLoadData["vLook.z"], 0.f);
		MapDesc.WorldMatrix.r[3] = XMVectorSet(jLoadData["vPosition.x"], jLoadData["vPosition.y"], jLoadData["vPosition.z"], 1.f);

		Add_MapObjData(MapDesc.iModelID, MapDesc);

		ID_UINT iID, iModelID;
		iID.iID = MapDesc.iObjectID;
		iModelID.iID = MapDesc.iModelID;
		_uint iModelNum = 0;
		iModelNum = iModelID.tNums.Num2 + iModelID.tNums.Num3 * 255;
	}
#endif

#if defined(_INVEN)

	json jLoad5;

	pGameInstance->Load_Json(pGameInstance->json_Complete_Path(towstring("Map/Inven_New").c_str()).c_str(), &jLoad5);

	json jSourLoad5 = jLoad5["MapObj"];

	for (auto& jLoadData : jSourLoad5)
	{
		GAMEOBJECT_DESC MapDesc;

		MapDesc.iObjectID = jLoadData["iObjectID"];
		MapDesc.iModelID = jLoadData["iModelID"];

		MapDesc.WorldMatrix.r[0] = XMVectorSet(jLoadData["vRight.x"], jLoadData["vRight.y"], jLoadData["vRight.z"], 0.f);
		MapDesc.WorldMatrix.r[1] = XMVectorSet(jLoadData["vUp.x"], jLoadData["vUp.y"], jLoadData["vUp.z"], 0.f);
		MapDesc.WorldMatrix.r[2] = XMVectorSet(jLoadData["vLook.x"], jLoadData["vLook.y"], jLoadData["vLook.z"], 0.f);
		MapDesc.WorldMatrix.r[3] = XMVectorSet(jLoadData["vPosition.x"], jLoadData["vPosition.y"], jLoadData["vPosition.z"], 1.f);

		Add_MapObjData(MapDesc.iModelID, MapDesc);

		ID_UINT iID, iModelID;
		iID.iID = MapDesc.iObjectID;
		iModelID.iID = MapDesc.iModelID;
		_uint iModelNum = 0;
		iModelNum = iModelID.tNums.Num2 + iModelID.tNums.Num3 * 255;
	}
#endif

#if defined(_EQUIP)
	json jEquip;

	pGameInstance->Load_Json(pGameInstance->json_Complete_Path(towstring("Map/Equip_New").c_str()).c_str(), &jEquip);

	json jSourLoadEquip = jEquip["MapObj"];

	for (auto& jLoadData : jSourLoadEquip)
	{
		GAMEOBJECT_DESC tInfo;
		tInfo.iObjectID = jLoadData["iObjectID"];
		tInfo.iModelID = jLoadData["iModelID"];

		tInfo.WorldMatrix.r[0] = XMVectorSet(jLoadData["vRight.x"], jLoadData["vRight.y"], jLoadData["vRight.z"], 0.f);
		tInfo.WorldMatrix.r[1] = XMVectorSet(jLoadData["vUp.x"], jLoadData["vUp.y"], jLoadData["vUp.z"], 0.f);
		tInfo.WorldMatrix.r[2] = XMVectorSet(jLoadData["vLook.x"], jLoadData["vLook.y"], jLoadData["vLook.z"], 0.f);
		tInfo.WorldMatrix.r[3] = XMVectorSet(jLoadData["vPosition.x"], jLoadData["vPosition.y"], jLoadData["vPosition.z"], 1.f);

		ID_UINT iID, iModelID;
		iID.iID = tInfo.iObjectID;
		iModelID.iID = tInfo.iModelID;
		_uint iModelNum = 0;
		iModelNum = iModelID.tNums.Num2 + iModelID.tNums.Num3 * 255;

		if (MODEL_EQUIPMENT == iID.tNums.Num1)
		{
			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, iID, iID, &iModelNum);

			pGameInstance->Get_Objects(LEVEL_GAMEPLAY, iID)->back()->Get_Transform()->Set_WorldMatrix(tInfo.WorldMatrix);
		}
	}
#endif

	// Instance Load
	for (auto& MapIter : m_MapObjLists)
	{
		CMapObj_Instance::MAPOBJINSTANCEDESC MapDesc;
		MapDesc.NumModel = MapIter.second.size();
		MapDesc.ModelID = MapIter.first;

		ID_UINT Item;
		Item.iID = MapIter.first;
		if (MODEL_ITEM == Item.tNums.Num1)
			continue;

		ID_UINT Trigger;
		Trigger.iID = MapIter.first;
		if (MODEL_TRIGGER == Trigger.tNums.Num1)
			continue;

#if !defined(_STRUCT)
		ID_UINT Temp;
		Temp.iID = MapIter.first;
		if (MODEL_STRUCTS == Temp.tNums.Num1)
			continue;

		if (MODEL_STRUCTS_DESERT == Temp.tNums.Num1)
			continue;

		if (MODEL_ROOM == Temp.tNums.Num1)
			continue;
#endif

		MapDesc.ModelID.tNums.Num0 = MODEL_STATIC_INSTANCE;

		list<_matrix>	Matrixs;
		for (auto& iter : MapIter.second)
		{
			Matrixs.push_back(iter.WorldMatrix);
		}

		CGameObject* pMapObj = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(MAP, MAP_INSTANCE), ID_UINT(MAP, MAP_INSTANCE), &MapDesc);
		((CModel*)pMapObj->Get_Component(ID_UINT(MODEL_STATIC_INSTANCE)))->Set_InsDesc(Matrixs);

		for (size_t i = 0; i < NUMSUNLAYER; ++i)
			m_pSun[i]->Add_RenderObject(pMapObj);
	}
#endif



	//Load_Navigation();

	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(SKY, MAP_CLOUD_SKY), ID_UINT(SKY, MAP_CLOUD_SKY));
	for (size_t i = 0; i < 20; ++i)
	{
		pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(SKY, MAP_CLOUD), ID_UINT(SKY, MAP_CLOUD));
	}

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI(ID_UINT iLayerID)
{
	USEGAMEINSTANCE;
	CUI_Logo_Back* pLogo_Back = (CUI_Logo_Back*)pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(UI, UI_LOGO_BACK), ID_UINT(UI, UI_LOGO_BACK),
		&CUI::UIDESC(640.f, 360.f, 1280.f, 720.f, 0.f, 16, 10));
	if (!pLogo_Back)
		__debugbreak();
	pLogo_Back->Set_FadeOut();

	NULLCHECK(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(UI), ID_UINT(UI, UI_CURSOR)));
	NULLCHECK(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(UI, UI_ATTACK_TARGET), ID_UINT(UI, UI_ATTACK_TARGET)));

	//#ifdef _UI_PLACEMARK

		/*pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(UI, UI_PLACEMARK), ID_UINT(UI, UI_PLACEMARK),
			&CUI::UIDESC(200.f, 200.f, 38.f, 38.f, 0.f, 0, 400));
		pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(UI, UI_PLACEMARK), ID_UINT(UI, UI_PLACEMARK),
			&CUI::UIDESC(200.f, 200.f, 43.f, 43.f, 0.f, 1, 400));
		pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(UI, UI_PLACEMARK), ID_UINT(UI, UI_PLACEMARK),
			&CUI::UIDESC(200.f, 200.f, 32.f, 32.f, 0.f, 2, 400));
		pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(UI, UI_PLACEMARK), ID_UINT(UI, UI_PLACEMARK),
			&CUI::UIDESC(200.f, 200.f, 27.f, 27.f, 0.f, 3, 400));*/

			//endif // _UI_PLACEMARK

	Load_UI("MainMenuUI");
	Load_UI("MainStatus");
	Load_UI("GunCrossHair");
	Load_UI("Minimap_Arrow");

	//Load_UI("Quest_Complete");
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Item(ID_UINT iLayerID)
{
	USEGAMEINSTANCE;

	json jLoad;

	pGameInstance->Load_Json(pGameInstance->json_Complete_Path(towstring("Map/Item").c_str()).c_str(), &jLoad);

	json jSourLoad = jLoad["MapObj"];

	for (auto& jLoadData : jSourLoad)
	{
		GAMEOBJECT_DESC tInfo;
		tInfo.iObjectID = jLoadData["iObjectID"];
		tInfo.iModelID = jLoadData["iModelID"];

		tInfo.WorldMatrix.r[0] = XMVectorSet(jLoadData["vRight.x"], jLoadData["vRight.y"], jLoadData["vRight.z"], 0.f);
		tInfo.WorldMatrix.r[1] = XMVectorSet(jLoadData["vUp.x"], jLoadData["vUp.y"], jLoadData["vUp.z"], 0.f);
		tInfo.WorldMatrix.r[2] = XMVectorSet(jLoadData["vLook.x"], jLoadData["vLook.y"], jLoadData["vLook.z"], 0.f);
		tInfo.WorldMatrix.r[3] = XMVectorSet(jLoadData["vPosition.x"], jLoadData["vPosition.y"], jLoadData["vPosition.z"], 1.f);

		ID_UINT iID, iModelID;
		iID.iID = tInfo.iObjectID;
		iModelID.iID = tInfo.iModelID;
		_uint iModelNum = 0;
		iModelNum = iModelID.tNums.Num2 + iModelID.tNums.Num3 * 255;

		if (MODEL_ITEM == iID.tNums.Num1)
		{
			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, iID, iID, &iModelNum);
			pGameInstance->Get_Objects(LEVEL_GAMEPLAY, iID)->back()->Get_Transform()->Set_WorldMatrix(tInfo.WorldMatrix);
		}
	}


	json jLoad1;

	pGameInstance->Load_Json(pGameInstance->json_Complete_Path(towstring("Map/Shelf_Item").c_str()).c_str(), &jLoad1);

	json jSourLoad1 = jLoad1["MapObj"];

	for (auto& jLoadData : jSourLoad1)
	{
		GAMEOBJECT_DESC tInfo;
		tInfo.iObjectID = jLoadData["iObjectID"];
		tInfo.iModelID = jLoadData["iModelID"];

		tInfo.WorldMatrix.r[0] = XMVectorSet(jLoadData["vRight.x"], jLoadData["vRight.y"], jLoadData["vRight.z"], 0.f);
		tInfo.WorldMatrix.r[1] = XMVectorSet(jLoadData["vUp.x"], jLoadData["vUp.y"], jLoadData["vUp.z"], 0.f);
		tInfo.WorldMatrix.r[2] = XMVectorSet(jLoadData["vLook.x"], jLoadData["vLook.y"], jLoadData["vLook.z"], 0.f);
		tInfo.WorldMatrix.r[3] = XMVectorSet(jLoadData["vPosition.x"], jLoadData["vPosition.y"], jLoadData["vPosition.z"], 1.f);

		ID_UINT iID, iModelID;
		iID.iID = tInfo.iObjectID;
		iModelID.iID = tInfo.iModelID;
		_uint iModelNum = 0;
		iModelNum = iModelID.tNums.Num2 + iModelID.tNums.Num3 * 255;

		if (MODEL_SHELF_ITEM == iID.tNums.Num2)
		{
			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, iID, iID, &iModelNum);
			pGameInstance->Get_Objects(LEVEL_GAMEPLAY, iID)->back()->Get_Transform()->Set_WorldMatrix(tInfo.WorldMatrix);
		}
	}


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Npc(ID_UINT iLayerID)
{
	USEGAMEINSTANCE;
	CGameObject* pGameObject = nullptr;
	NULLCHECK(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iLayerID), ID_UINT(NPC, NPC_SHIRLEY)));
	NULLCHECK(pGameObject = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(NPC, NPC_COOKER), ID_UINT(NPC, NPC_COOKER)));

	//((CCamera_Target_Position*)m_pCookCamera)->Set_TargetObject(pGameObject);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_ActionCamera(ID_UINT iID)
{
#if defined(_PLAYER)

	USEGAMEINSTANCE;
	CCamera::CAMERADESC CameraDesc;
	CameraDesc.vEye = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.vAt = _float4(0.f, 0.f, 1.f, 1.f);
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = g_fNear;
	CameraDesc.fFar = g_fFar;

	CCamera_Action* pActionCamera = nullptr;
	NULLCHECK(pActionCamera = dynamic_cast<CCamera_Action*>(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iID), ID_UINT(CAMERA, CAMERA_ACTION), &CameraDesc)));
	pActionCamera->Reserve_NumActionDesc(4);
	pActionCamera->Set_StartDesc(_float3(0.f, 100.f, 0.f), _float3(256.f, 15.f, 256.f));
	pActionCamera->Set_EndDesc(_float3(512.f, 100.f, 512.f), _float3(256.f, 15.f, 256.f), 15, 3.f);
	pActionCamera->Set_ActionCamDesc(_float3(412.f, 100.f, 0.f), _float3(256.f, 15.f, 256.f), 15, 3.f, 1);
	pActionCamera->Set_ActionCamDesc(_float3(0.f, 100.f, 412.f), _float3(256.f, 15.f, 256.f), 15, 3.f, 2);
	pGameInstance->Add_Camera(TEXT("Test0"), pActionCamera);

	NULLCHECK(pActionCamera = dynamic_cast<CCamera_Action*>(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iID), ID_UINT(CAMERA, CAMERA_ACTION), &CameraDesc)));
	pActionCamera->Reserve_NumActionDesc(4);
	pActionCamera->Set_StartDesc(_float3(0.f, 1.8f, -3.6f), _float3(0.f, 0.f, 0.f));
	pActionCamera->Set_EndDesc(_float3(0.f, 1.8f, -3.6f), _float3(0.f, 0.f, 0.f), 15, 2.f);
	pActionCamera->Set_ActionCamDesc(_float3(3.f, 1.f, -1.f), _float3(0.f, 0.f, 0.f), 15, 1.5f, 1);
	pActionCamera->Set_ActionCamDesc(_float3(-1.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), 15, 1.f, 2);
	pActionCamera->Set_TargetObejct(m_pPlayer);
	pGameInstance->Add_Camera(TEXT("Test1"), pActionCamera);

	pGameInstance->Add_MC(TEXT("MC_Test"), TEXT("Test0"));
	pGameInstance->Add_MC(TEXT("MC_Test"), TEXT("Test1"));
#endif

#if defined(_ENEMYBOSS)
	// 좌 - 우 - 울부짖는거 뒤로빠지면서 전체 모습.
	/*NULLCHECK(pActionCamera = dynamic_cast<CCamera_Action*>(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(CAMERA), ID_UINT(CAMERA, CAMERA_ACTION), &CameraDesc)));
	pActionCamera->Reserve_NumActionDesc(2);
	pActionCamera->Set_TargetObejct(pGameInstance->Get_Object(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_FROSTDRAGON)));
	pActionCamera->Set_StartDesc(_float3(15.f, -3.f, 6.f), _float3(0.f, 1.f, 0.f));
	pActionCamera->Set_EndDesc(_float3(10.f, -3.f, 8.f), _float3(0.f, 1.f, 0.f), 15, 1.5f);
	pGameInstance->Add_Camera(TEXT("Dragon_CameraAction1"), pActionCamera);
	NULLCHECK(pActionCamera = dynamic_cast<CCamera_Action*>(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(CAMERA), ID_UINT(CAMERA, CAMERA_ACTION), &CameraDesc)));
	pActionCamera->Reserve_NumActionDesc(2);
	pActionCamera->Set_TargetObejct(pGameInstance->Get_Object(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_FROSTDRAGON)));
	pActionCamera->Set_StartDesc(_float3(-15.f, -3.f, 6.f), _float3(0.f, 1.f, 0.f));
	pActionCamera->Set_EndDesc(_float3(-10.f, -3.f, 8.f), _float3(0.f, 1.f, 0.f), 15, 1.5f);
	pGameInstance->Add_Camera(TEXT("Dragon_CameraAction2"), pActionCamera);
	NULLCHECK(pActionCamera = dynamic_cast<CCamera_Action*>(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(CAMERA), ID_UINT(CAMERA, CAMERA_ACTION), &CameraDesc)));
	pActionCamera->Reserve_NumActionDesc(2);
	pActionCamera->Set_TargetObejct(pGameInstance->Get_Object(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_FROSTDRAGON)));
	pActionCamera->Set_StartDesc(_float3(0.f, -6.f, 15.f), _float3(0.f, 0.f, 0.f));
	pActionCamera->Set_EndDesc(_float3(0.f, -6.f, 16.f), _float3(0.f, 0.f, 0.f), 15, 1.5f);
	pGameInstance->Add_Camera(TEXT("Dragon_CameraAction3"), pActionCamera);*/

	NULLCHECK(pActionCamera = dynamic_cast<CCamera_Action*>(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(CAMERA), ID_UINT(CAMERA, CAMERA_ACTION), &CameraDesc)));
	pActionCamera->Reserve_NumActionDesc(4);
	pActionCamera->Set_TargetObejct(pGameInstance->Get_Object(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_FROSTDRAGON)));
	pActionCamera->Set_StartDesc(_float3(0.f, -6.f, 16.f), _float3(0.f, 2.f, 0.f));
	pActionCamera->Set_ActionCamDesc(_float3(0.f, -4.f, 16.f), _float3(0.f, 2.f, 0.f), 15, 2.f, 1);
	pActionCamera->Set_ActionCamDesc(_float3(0.f, -4.f, 18.f), _float3(0.f, 2.f, 0.f), 15, 2.f, 2);
	pActionCamera->Set_EndDesc(_float3(0.f, -5.f, 21.f), _float3(0.f, 2.f, 0.f), 11, 2.f);
	pGameInstance->Add_Camera(TEXT("Dragon_CameraAction"), pActionCamera);

	NULLCHECK(pActionCamera = dynamic_cast<CCamera_Action*>(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(CAMERA), ID_UINT(CAMERA, CAMERA_ACTION), &CameraDesc)));
	pActionCamera->Reserve_NumActionDesc(4);
	pActionCamera->Set_TargetObejct(pGameInstance->Get_Object(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_ROBELLA)));
	pActionCamera->Set_StartDesc(_float3(0.f, 6.f, 13.f), _float3(0.f, 4.f, 0.f));
	pActionCamera->Set_ActionCamDesc(_float3(0.f, 6.f, 13.f), _float3(0.f, 4.f, 0.f), 15, 2.f, 1);
	pActionCamera->Set_ActionCamDesc(_float3(0.f, 6.f, 7.f), _float3(0.f, 4.f, 0.f), 15, 2.f, 2);
	pActionCamera->Set_EndDesc(_float3(0.f, 8.f, 13.f), _float3(0.f, 4.f, 0.f), 11, 2.f);
	pGameInstance->Add_Camera(TEXT("Robella_CameraAction"), pActionCamera);

	NULLCHECK(pActionCamera = dynamic_cast<CCamera_Action*>(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(CAMERA), ID_UINT(CAMERA, CAMERA_ACTION), &CameraDesc)));
	pActionCamera->Reserve_NumActionDesc(4);
	pActionCamera->Set_TargetObejct(pGameInstance->Get_Object(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_WEAVER)));
	pActionCamera->Set_StartDesc(_float3(0.f, 3.f, 6.f), _float3(0.f, 4.f, 0.f));
	pActionCamera->Set_ActionCamDesc(_float3(0.f, 3.f, 6.f), _float3(0.f, 4.f, 0.f), 15, 1.f, 1);
	pActionCamera->Set_ActionCamDesc(_float3(0.f, 4.f, 7.f), _float3(0.f, 4.f, 0.f), 15, 1.3f, 2);
	pActionCamera->Set_EndDesc(_float3(0.f, 6.f, 8.f), _float3(0.f, 4.f, 0.f), 11, 2.f);
	pGameInstance->Add_Camera(TEXT("Weaver_CameraAction"), pActionCamera);

	NULLCHECK(pActionCamera = dynamic_cast<CCamera_Action*>(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(CAMERA), ID_UINT(CAMERA, CAMERA_ACTION), &CameraDesc)));
	pActionCamera->Reserve_NumActionDesc(3);
	pActionCamera->Set_TargetObejct(pGameInstance->Get_Object(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_LEVIATHAN)), "Bone_eye_02_end");
	pActionCamera->Set_StartDesc(_float3(-80.f, 40.f, -100.f), _float3(0.f, 0.f, 0.f));
	pActionCamera->Set_ActionCamDesc(_float3(-80.f, 40.f, -100.f), _float3(0.f, 0.f, 0.f), 15, 3.f, 1);
	pActionCamera->Set_EndDesc(_float3(180.f, 80.f, 140.f), _float3(0.f, 0.f, 0.f), 15, 3.f);
	pGameInstance->Add_Camera(TEXT("Leviathan_CameraAction"), pActionCamera);
#endif

	// 마지막 카메라 컷씬
	// 로벨라1-2  - 기계도시1-2 - 용1-2 - 태양-태양(시간 지속)
	NULLCHECK(pActionCamera = dynamic_cast<CCamera_Action*>(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(iID), ID_UINT(CAMERA, CAMERA_ACTION), &CameraDesc)));
	pActionCamera->Reserve_NumActionDesc(42);
	pActionCamera->Set_StartDesc(_float3(38.9077, 126.889, 77.1809), _float3(39.5585, 126.505, 77.8362));
	pActionCamera->Set_ActionCamDesc(_float3(118.891, 104.367, 8.12183), _float3(119.255, 104.083, 9.00878), 0, 5.287, 1);
	pActionCamera->Set_ActionCamDesc(_float3(208.213, 72.3202, 27.6823), _float3(208.354, 71.7107, 28.4624), 0, 3.72, 2);
	pActionCamera->Set_ActionCamDesc(_float3(216.427, 37.5732, 70.7003), _float3(216.217, 37.1069, 71.5596), 0, 2.818, 3);
	pActionCamera->Set_ActionCamDesc(_float3(211.679, 30.3775, 100.851), _float3(211.688, 30.2334, 101.841), 0, 1.585, 4);
	pActionCamera->Set_ActionCamDesc(_float3(215.115, 23.8677, 134.436), _float3(216.003, 23.8147, 134.893), 0, 1.601, 5);
	pActionCamera->Set_ActionCamDesc(_float3(238.777, 29.2134, 144.21), _float3(239.631, 29.667, 144.466), 0, 1.334, 6);
	pActionCamera->Set_ActionCamDesc(_float3(262.519, 30.9933, 166.949), _float3(262.935, 30.7719, 167.831), 0, 1.801, 7);
	pActionCamera->Set_ActionCamDesc(_float3(280.831, 22.677, 212.992), _float3(281.096, 22.5458, 213.947), 0, 2.518, 8);
	pActionCamera->Set_ActionCamDesc(_float3(299.255, 21.7285, 258.939), _float3(298.529, 21.3331, 258.375), 0, 2.152, 9);
	pActionCamera->Set_ActionCamDesc(_float3(330.046, 46.8533, 292.442), _float3(329.414, 46.4699, 291.769), 0, 2.651, 10);
	pActionCamera->Set_ActionCamDesc(_float3(367.989, 36.1444, 270.951), _float3(368.779, 35.8472, 270.415), 0, 2.385, 11);
	pActionCamera->Set_ActionCamDesc(_float3(390.853, 31.3378, 251.028), _float3(391.69, 31.6087, 250.551), 0, 1.568, 12);
	pActionCamera->Set_ActionCamDesc(_float3(417.368, 43.1333, 238.794), _float3(418.217, 43.5634, 238.487), 0, 1.584, 13);
	pActionCamera->Set_ActionCamDesc(_float3(445.227, 50.1714, 218.032), _float3(445.981, 50.1184, 217.378), 0, 1.801, 14);
	pActionCamera->Set_ActionCamDesc(_float3(469.731, 50.2719, 189.932), _float3(470.485, 50.351, 189.28), 0, 1.869, 15);
	pActionCamera->Set_ActionCamDesc(_float3(499.375, 52.0941, 176.732), _float3(500.368, 52.147, 176.621), 0, 1.651, 16);
	pActionCamera->Set_ActionCamDesc(_float3(535.909, 53.235, 176.681), _float3(536.308, 53.2486, 175.765), 0, 1.668, 17);
	pActionCamera->Set_ActionCamDesc(_float3(591.881, 76.4021, 203.029), _float3(591.08, 76.1431, 202.49), 0, 3.352, 18);
	pActionCamera->Set_ActionCamDesc(_float3(595.147, 82.4595, 251.04), _float3(594.418, 82.3679, 250.362), 0, 1.718, 19);
	pActionCamera->Set_ActionCamDesc(_float3(551.36, 82.7232, 273.517), _float3(550.454, 82.7493, 273.096), 0, 2.102, 20);
	pActionCamera->Set_ActionCamDesc(_float3(459.781, 127.037, 353.3), _float3(459.583, 126.778, 352.354), 0, /*5.303*/3.f, 21);
	pActionCamera->Set_ActionCamDesc(_float3(422.71, 141.567, 397.242), _float3(422.759, 141.09, 396.365), 0, 2.118, 22);
	pActionCamera->Set_ActionCamDesc(_float3(302.95, 85.5796, 381.326), _float3(302.04, 85.2213, 381.118), 0, /*6.421*/3.f, 23);
	pActionCamera->Set_ActionCamDesc(_float3(227.353, 40.8979, 369.787), _float3(226.473, 40.444, 369.645), 0, /*4.453*/3.f, 24);
	pActionCamera->Set_ActionCamDesc(_float3(192.066, 36.0499, 363.691), _float3(191.093, 36.1156, 363.468), 0, 1.835, 25);
	pActionCamera->Set_ActionCamDesc(_float3(155.906, 35.3963, 340.412), _float3(155.129, 35.3312, 341.039), 0, 1.867, 26);
	pActionCamera->Set_ActionCamDesc(_float3(118.474, 37.7488, 344.865), _float3(119.283, 37.4151, 345.349), 0, 1.451, 27);
	pActionCamera->Set_ActionCamDesc(_float3(81.7653, 61.4632, 343.85), _float3(82.5195, 60.8544, 344.096), 0, 2.102, 28);
	pActionCamera->Set_ActionCamDesc(_float3(107.714, 72.548, 299.68), _float3(107.969, 72.0481, 300.507), 0, 3.068, 29);
	pActionCamera->Set_ActionCamDesc(_float3(139.991, 59.5381, 319.006), _float3(139.905, 59.1555, 319.926), 0, 1.418, 30);
	pActionCamera->Set_ActionCamDesc(_float3(143.258, 50.2881, 339.22), _float3(142.983, 49.8225, 340.061), 0, 1.084, 31);
	pActionCamera->Set_ActionCamDesc(_float3(137.493, 39.1556, 363.384), _float3(137.441, 38.9096, 364.352), 0, 1.368, 32);
	pActionCamera->Set_ActionCamDesc(_float3(137.933, 41.2644, 380.496), _float3(137.883, 41.6595, 381.413), 0, 0.883, 33);
	pActionCamera->Set_ActionCamDesc(_float3(140.788, 61.7311, 408.575), _float3(140.872, 62.2979, 409.394), 0, 1.752, 34);
	pActionCamera->Set_ActionCamDesc(_float3(144.499, 78.897, 432.04), _float3(144.54, 79.4852, 432.848), 0, 1.467, 35);
	pActionCamera->Set_ActionCamDesc(_float3(146.705, 90.234, 450.136), _float3(147.016, 90.4549, 451.06), 0, 1.085, 36);
	pActionCamera->Set_ActionCamDesc(_float3(136.717, 152.753, 456.6), _float3(137.151, 152.197, 455.891), 0, /*8.702*/4.f, 37);
	pActionCamera->Set_ActionCamDesc(_float3(43.5023, 180.722, 408.812), _float3(44.2856, 180.256, 408.4), 0, 4.316, 38);
	pActionCamera->Set_ActionCamDesc(_float3(183.602, 166.277, 410.343), _float3(184.213, 165.869, 409.665), 0, /*7.665*/4.f, 39);
	pActionCamera->Set_ActionCamDesc(_float3(238.798, 180.755, 423.545), _float3(239.693, 181.197, 423.604), 0, 2.666, 40);
	pActionCamera->Set_EndDesc(_float3(269.358, 195.616, 424.85), _float3(270.261, 196.046, 424.85), 11, 60.f);
	pGameInstance->Add_Camera(TEXT("Final_Cut_Scene"), pActionCamera);


	NULLCHECK(pActionCamera = dynamic_cast<CCamera_Action*>(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(CAMERA), ID_UINT(CAMERA, CAMERA_ACTION), &CameraDesc)));
	pActionCamera->Reserve_NumActionDesc(3);
	pActionCamera->Set_TargetObejct(pGameInstance->Get_Object(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_LEVIATHAN)), "Bone_eye_02_end");
	pActionCamera->Set_StartDesc(_float3(-80.f, 120.f, -100.f), _float3(0.f, 0.f, 0.f));
	pActionCamera->Set_ActionCamDesc(_float3(-80.f, 120.f, -100.f), _float3(0.f, 0.f, 0.f), 15, 1.f, 1);
	pActionCamera->Set_EndDesc(_float3(180.f, 140.f, 140.f), _float3(0.f, 0.f, 0.f), 15, 2.f);
	pGameInstance->Add_Camera(TEXT("Leviathan_Damaged"), pActionCamera);


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_MiniGame(MINIGAME_TYPE eType)
{
	USEGAMEINSTANCE;
	if (MINIGAME_GLIDE == eType)
	{
		json jMini_Swing;


		pGameInstance->Load_Json(pGameInstance->json_Complete_Path(towstring("Map/Minigame_Swing").c_str()).c_str(), &jMini_Swing);

		json jSourLoad = jMini_Swing["MapObj"];

		for (auto& jLoadData : jSourLoad)
		{
			GAMEOBJECT_DESC tInfo;
			tInfo.iObjectID = jLoadData["iObjectID"];

			tInfo.WorldMatrix.r[0] = XMVectorSet(jLoadData["vRight.x"], jLoadData["vRight.y"], jLoadData["vRight.z"], 0.f);
			tInfo.WorldMatrix.r[1] = XMVectorSet(jLoadData["vUp.x"], jLoadData["vUp.y"], jLoadData["vUp.z"], 0.f);
			tInfo.WorldMatrix.r[2] = XMVectorSet(jLoadData["vLook.x"], jLoadData["vLook.y"], jLoadData["vLook.z"], 0.f);
			tInfo.WorldMatrix.r[3] = XMVectorSet(jLoadData["vPosition.x"], jLoadData["vPosition.y"], jLoadData["vPosition.z"], 1.f);

			ID_UINT iID;
			iID.iID = tInfo.iObjectID;

			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, iID, iID);
			pGameInstance->Get_Objects(LEVEL_GAMEPLAY, iID)->back()->Get_Transform()->Set_WorldMatrix(tInfo.WorldMatrix);
		}
	}
	else if (MINIGAME_BIKE == eType)
	{
		json jMini_Coin;

		pGameInstance->Load_Json(pGameInstance->json_Complete_Path(towstring("Map/Minigame_Coin").c_str()).c_str(), &jMini_Coin);

		json jSourLoad1 = jMini_Coin["MapObj"];

		for (auto& jLoadData : jSourLoad1)
		{
			GAMEOBJECT_DESC tInfo;
			tInfo.iObjectID = jLoadData["iObjectID"];

			tInfo.WorldMatrix.r[0] = XMVectorSet(jLoadData["vRight.x"], jLoadData["vRight.y"], jLoadData["vRight.z"], 0.f);
			tInfo.WorldMatrix.r[1] = XMVectorSet(jLoadData["vUp.x"], jLoadData["vUp.y"], jLoadData["vUp.z"], 0.f);
			tInfo.WorldMatrix.r[2] = XMVectorSet(jLoadData["vLook.x"], jLoadData["vLook.y"], jLoadData["vLook.z"], 0.f);
			tInfo.WorldMatrix.r[3] = XMVectorSet(jLoadData["vPosition.x"], jLoadData["vPosition.y"], jLoadData["vPosition.z"], 1.f);

			ID_UINT iID;
			iID.iID = tInfo.iObjectID;

			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, iID, iID);
			pGameInstance->Get_Objects(LEVEL_GAMEPLAY, iID)->back()->Get_Transform()->Set_WorldMatrix(tInfo.WorldMatrix);
		}
	}
	return S_OK;
}

void CLevel_GamePlay::Load_UI(string strFileName)
{
	USEGAMEINSTANCE;
	json Data;

	wstring strFolder = TEXT("UI/");
	wstring strTemp = towstring(strFileName);
	wstring strFullName = strFolder + strTemp;

	const wchar_t* szName = strFullName.c_str();
	pGameInstance->Load_Json(pGameInstance->json_Complete_Path(szName).c_str(), &Data);

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

		//if (ID_UINT(UI, UI_MAIN_ICON).iID == IDPrototype.iID ||
		//	/*ID_UINT(UI, UI_PLAYER_HP_BAR).iID == IDPrototype.iID ||*/
		//	/*ID_UINT(UI, UI_PLAYER_HP_BACK).iID == IDPrototype.iID ||*/
		//	ID_UINT(UI, UI_MAIN_BUTTON).iID == IDPrototype.iID ||
		//	/*ID_UINT(UI, UI_STAMINA_BAR).iID == IDPrototype.iID ||*/
		//	/*ID_UINT(UI, UI_STAMINA_BACK).iID == IDPrototype.iID ||*/
		//	ID_UINT(UI, UI_SHORTKEY).iID == IDPrototype.iID
		//	/*ID_UINT(UI, UI_CURFOOD_ICON).iID == IDPrototype.iID ||*/
		//	/*ID_UINT(UI, UI_CURFOOD_NUM).iID == IDPrototype.iID*/)
		//{
		//	continue;
		//}

		//if (
		//	//ID_UINT(UI, UI_PLAYER_HP_BAR).iID == IDPrototype.iID ||
		//	//ID_UINT(UI, UI_PLAYER_HP_BACK).iID == IDPrototype.iID ||
		//	//ID_UINT(UI, UI_STAMINA_BAR).iID == IDPrototype.iID ||
		//	//ID_UINT(UI, UI_STAMINA_BACK).iID == IDPrototype.iID ||
		//	ID_UINT(UI, UI_CURFOOD_ICON).iID == IDPrototype.iID ||
		//	ID_UINT(UI, UI_CURFOOD_NUM).iID == IDPrototype.iID)
		//{
		//	continue;
		//}


		CGameObject* pGameObject = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, IDPrototype, IDPrototype, &UIDesc);
		CUI* pUI = (CUI*)pGameObject;
		pUI->Set_FontText(strTemp);
	}
}

void CLevel_GamePlay::Load_Navigation()
{


}

void CLevel_GamePlay::Add_MapObjData(_uint iModelID, GAMEOBJECT_DESC tInfo)
{
	ID_UINT ModelID;
	ModelID.iID = iModelID;
	GAMEOBJDESCS* pLayer = Find_MapList(ModelID.iID);

	if (nullptr == pLayer)
	{
		GAMEOBJDESCS	OBjDesc;
		OBjDesc.push_back(tInfo);
		m_MapObjLists.emplace(ModelID.iID, OBjDesc);
	}
	else
		pLayer->push_back(tInfo);
}

list<CLevel_GamePlay::GAMEOBJECT_DESC>* CLevel_GamePlay::Find_MapList(_uint iModelID)
{
	auto	iter = find_if(m_MapObjLists.begin(), m_MapObjLists.end(), CID_Finder(iModelID));

	if (iter == m_MapObjLists.end())
		return nullptr;

	return &iter->second;
}

void CLevel_GamePlay::Render_Light_Depth(CLight* pLight)
{
	//_vector vLightDir = XMVector3Normalize(XMLoadFloat4(&pLight->Get_LightDesc()->vPosition));

	//if (0.f < XMVectorGetX(XMVector3Dot(vLightDir, XMVectorSet(0.f, 1.f, 0.f, 0.f))))
	pLight->Render_Depth(m_pDepthShader, 2);

}

void CLevel_GamePlay::Move_LightRevolution(CLight* pLight, _fvector vAxis, _fvector vCurPos, _float fSpeed)
{
	_vector	vRotatedPos;
	vRotatedPos = XMVector3Rotate(vCurPos, XMQuaternionRotationAxis(vAxis, fSpeed));
	pLight->Set_Position(vRotatedPos);
}

void CLevel_GamePlay::Update_LightState()
{
#if defined(_PLAYER)
	_float4 vSunPosition = m_pSun[0]->Get_LightDesc()->vPosition;
	IMPL_ONCE(m_bSun, EFFECT_EXCUTE("Sun", XMMatrixTranslation(vSunPosition.x, vSunPosition.y, vSunPosition.z), 1.f));

	CGameObject* TargetObject = ((CLight_Target*)m_pPlayerLight)->Get_TargetObject();
	_bool RendrState = TargetObject->Get_RenderState();
	_vector vSunPos = XMLoadFloat4(&m_pSun[0]->Get_LightDesc()->vPosition);

	if (RendrState)
	{
		_vector vSunDirfromTarget = XMVector3Normalize(vSunPos - TargetObject->Get_Pos());
		((CLight_Target*)m_pPlayerLight)->Set_Offset(vSunDirfromTarget * 20.f);
		((CLight_Target*)m_pPlayerLight)->Render_Depth(m_pDepthShader, 2);
	}
#endif
}

void CLevel_GamePlay::CreateCrystal(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	m_fTimeAcc_Crytal += fTimeDelta;
	// Crystal Create
	if (m_bCrystal_Robella && 0 <= m_fTimeAcc_Crytal)
	{
		// 로벨라쪽
		CGameObject* pCrystal_Robella = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_NORMAL, LEVIATHAN_CRYSTAL), ID_UINT(ENEMY, ENEMY_NORMAL, LEVIATHAN_CRYSTAL), &UNITDESC(ELEMENTAL_FIRE, 10000.f, 0.f, 1.f));
		NULLCHECK(pCrystal_Robella);
		pCrystal_Robella->Set_Pos(XMVectorSet(138.44f, 22.38f, 118.12f, 1.f));
		((CNavigation*)pCrystal_Robella->Get_Component(ID_UINT(NAVIGATION)))->Find_CurrentIndex(((CTransform*)pCrystal_Robella->Get_Transform()));
		((CNavigation*)pCrystal_Robella->Get_Component(ID_UINT(NAVIGATION)))->SetUp_OnTerrain(((CTransform*)pCrystal_Robella->Get_Transform()));

		// UI
		pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(UI, UI_CRYSTAL_HP_BAR), ID_UINT(UI, UI_CRYSTAL_HP_BAR),
			&CUI::UIDESC(738.f, 70.f, 100.f, 10.f, 0.f, 1, 900, { 0.f,0.f }, pCrystal_Robella));
		pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(UI, UI_CRYSTAL_HP_BAR), ID_UINT(UI, UI_CRYSTAL_HP_BAR),
			&CUI::UIDESC(738.f, 70.f, 100.f, 10.f, 0.f, 2, 800, { 0.f,0.f }, pCrystal_Robella));


		CCamera::CAMERADESC CameraDesc;
		CameraDesc.vEye = _float4(0.f, 0.f, 0.f, 1.f);
		CameraDesc.vAt = _float4(0.f, 0.f, 1.f, 1.f);
		CameraDesc.fFovy = XMConvertToRadians(60.0f);
		CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
		CameraDesc.fNear = g_fNear;
		CameraDesc.fFar = g_fFar;
		CCamera_Action* pActionCamera = nullptr;
		NULLCHECK(pActionCamera = dynamic_cast<CCamera_Action*>(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(CAMERA), ID_UINT(CAMERA, CAMERA_ACTION), &CameraDesc)));
		pActionCamera->Reserve_NumActionDesc(3);
		pActionCamera->Set_TargetObejct(pGameInstance->Get_Objects(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_NORMAL, LEVIATHAN_CRYSTAL))->back());
		pActionCamera->Set_StartDesc(_float3(0.f, 6.f, 12.f), _float3(0.f, 3.f, 0.f));
		pActionCamera->Set_ActionCamDesc(_float3(0.f, 6.f, 5.f), _float3(0.f, 3.f, 0.f), 15, 2.5f, 1);
		pActionCamera->Set_EndDesc(_float3(0.f, 8.f, 11.f), _float3(0.f, 3.f, 0.f), 11, 2.f);
		pGameInstance->Add_Camera(TEXT("Crystal_Robella_CameraAction"), pActionCamera);
		pGameInstance->Play_ActionCamera(TEXT("Crystal_Robella_CameraAction"), 0.f);

		m_bCrystal_Robella = false;
	}
	else if (m_bCrystal_Dragon && 4.5f <= m_fTimeAcc_Crytal)
	{
		// 용쪽
		CGameObject* pCrystal_Dragon = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_NORMAL, LEVIATHAN_CRYSTAL), ID_UINT(ENEMY, ENEMY_NORMAL, LEVIATHAN_CRYSTAL), &UNITDESC(ELEMENTAL_LIGHTNING, 10000.f, 0.f, 1.f));
		NULLCHECK(pCrystal_Dragon);
		pCrystal_Dragon->Set_Pos(XMVectorSet(100.f, 160.f, 435.27, 1.f));
		((CNavigation*)pCrystal_Dragon->Get_Component(ID_UINT(NAVIGATION)))->Find_CurrentIndex(((CTransform*)pCrystal_Dragon->Get_Transform()));
		((CNavigation*)pCrystal_Dragon->Get_Component(ID_UINT(NAVIGATION)))->SetUp_OnTerrain(((CTransform*)pCrystal_Dragon->Get_Transform()));

		// UI
		pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(UI, UI_CRYSTAL_HP_BAR), ID_UINT(UI, UI_CRYSTAL_HP_BAR),
			&CUI::UIDESC(640.f, 70.f, 100.f, 10.f, 0.f, 1, 900, { 0.f,0.f }, pCrystal_Dragon));
		pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(UI, UI_CRYSTAL_HP_BAR), ID_UINT(UI, UI_CRYSTAL_HP_BAR),
			&CUI::UIDESC(640.f, 70.f, 100.f, 10.f, 0.f, 2, 800, { 0.f,0.f }, pCrystal_Dragon));

		CCamera::CAMERADESC CameraDesc;
		CameraDesc.vEye = _float4(0.f, 0.f, 0.f, 1.f);
		CameraDesc.vAt = _float4(0.f, 0.f, 1.f, 1.f);
		CameraDesc.fFovy = XMConvertToRadians(60.0f);
		CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
		CameraDesc.fNear = g_fNear;
		CameraDesc.fFar = g_fFar;
		CCamera_Action* pActionCamera = nullptr;
		NULLCHECK(pActionCamera = dynamic_cast<CCamera_Action*>(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(CAMERA), ID_UINT(CAMERA, CAMERA_ACTION), &CameraDesc)));
		pActionCamera->Reserve_NumActionDesc(3);
		pActionCamera->Set_TargetObejct(pGameInstance->Get_Objects(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_NORMAL, LEVIATHAN_CRYSTAL))->back());
		pActionCamera->Set_StartDesc(_float3(0.f, 6.f, 12.f), _float3(0.f, 3.f, 0.f));
		pActionCamera->Set_ActionCamDesc(_float3(0.f, 6.f, 5.f), _float3(0.f, 3.f, 0.f), 15, 2.5f, 1);
		pActionCamera->Set_EndDesc(_float3(0.f, 8.f, 11.f), _float3(0.f, 3.f, 0.f), 11, 2.f);
		pGameInstance->Add_Camera(TEXT("Crystal_Dragon_CameraAction"), pActionCamera);
		pGameInstance->Play_ActionCamera(TEXT("Crystal_Dragon_CameraAction"), 0.f);

		m_bCrystal_Dragon = false;
	}
	else if (m_bCrystal_Weaver && 9.f <= m_fTimeAcc_Crytal)
	{
		// 웨버쪽
		CGameObject* pCrystal_Weaver = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_NORMAL, LEVIATHAN_CRYSTAL), ID_UINT(ENEMY, ENEMY_NORMAL, LEVIATHAN_CRYSTAL), &UNITDESC(ELEMENTAL_ICE, 10000.f, 0.f, 1.f));
		NULLCHECK(pCrystal_Weaver);
		pCrystal_Weaver->Set_Pos(XMVectorSet(436.26f, 153.46f, 308.f, 1.f)); // 
		((CNavigation*)pCrystal_Weaver->Get_Component(ID_UINT(NAVIGATION)))->Set_CurCellDesc(25591);
		((CNavigation*)pCrystal_Weaver->Get_Component(ID_UINT(NAVIGATION)))->SetUp_OnTerrain(((CTransform*)pCrystal_Weaver->Get_Transform()));

		// UI
		pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(UI, UI_CRYSTAL_HP_BAR), ID_UINT(UI, UI_CRYSTAL_HP_BAR),
			&CUI::UIDESC(542.f, 70.f, 100.f, 10.f, 0.f, 1, 900, { 0.f,0.f }, pCrystal_Weaver));
		pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(UI, UI_CRYSTAL_HP_BAR), ID_UINT(UI, UI_CRYSTAL_HP_BAR),
			&CUI::UIDESC(542.f, 70.f, 100.f, 10.f, 0.f, 2, 800, { 0.f,0.f }, pCrystal_Weaver));

		CCamera::CAMERADESC CameraDesc;
		CameraDesc.vEye = _float4(0.f, 0.f, 0.f, 1.f);
		CameraDesc.vAt = _float4(0.f, 0.f, 1.f, 1.f);
		CameraDesc.fFovy = XMConvertToRadians(60.0f);
		CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
		CameraDesc.fNear = g_fNear;
		CameraDesc.fFar = g_fFar;
		CCamera_Action* pActionCamera = nullptr;
		NULLCHECK(pActionCamera = dynamic_cast<CCamera_Action*>(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(CAMERA), ID_UINT(CAMERA, CAMERA_ACTION), &CameraDesc)));
		pActionCamera->Reserve_NumActionDesc(3);
		pActionCamera->Set_TargetObejct(pGameInstance->Get_Objects(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_NORMAL, LEVIATHAN_CRYSTAL))->back());
		pActionCamera->Set_StartDesc(_float3(0.f, 6.f, 12.f), _float3(0.f, 3.f, 0.f));
		pActionCamera->Set_ActionCamDesc(_float3(0.f, 6.f, 5.f), _float3(0.f, 3.f, 0.f), 15, 2.5f, 1);
		pActionCamera->Set_EndDesc(_float3(0.f, 8.f, 11.f), _float3(0.f, 3.f, 0.f), 11, 2.f);
		pGameInstance->Add_Camera(TEXT("Crystal_Weaver_CameraAction"), pActionCamera);
		pGameInstance->Play_ActionCamera(TEXT("Crystal_Weaver_CameraAction"), 0.f);

		m_bCrystal_Weaver = false;
	}
	else if (13.5 <= m_fTimeAcc_Crytal)
	{
		// 마지막 카메라 연출 끝나고 플레이어로 돌아오기 위해.
		pGameInstance->Set_NextCamera(m_pPlayerTargetCamera, 0.f);
		m_bStartCreateCrystal = false;

		static_cast<CEnemy_Boss_Leviathan*>(pGameInstance->Get_Object(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BOSS, ENEMY_BOSS_LEVIATHAN)))->Set_Apear(true);
	}
}

CLevel_GamePlay* CLevel_GamePlay::Create()
{
	CLevel_GamePlay* pInstance = new CLevel_GamePlay();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();
	Safe_Release(m_pDepthShader);
}

