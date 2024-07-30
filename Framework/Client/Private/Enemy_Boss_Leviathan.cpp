#include "stdafx.h"
#include "State_Handler.h"

#include "GameInstance.h"
#include "Collider.h"
#include "Sound_Manager.h"
#include "..\Public\Enemy_Boss_Leviathan.h"

#include "Navigation.h"

CEnemy_Boss_Leviathan::CEnemy_Boss_Leviathan()
	: CEnemy_Boss()
{
}

CEnemy_Boss_Leviathan::CEnemy_Boss_Leviathan(const CEnemy_Boss& rhs)
	: CEnemy_Boss(rhs)
{
}

_uint CEnemy_Boss_Leviathan::Get_NextIslandIdx()
{
	m_iCurIslandIdx++;
	if (2 < m_iCurIslandIdx)
		m_iCurIslandIdx = 0;

	return m_iCurIslandIdx;
}

HRESULT CEnemy_Boss_Leviathan::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemy_Boss_Leviathan::Initialize(void* pArg)
{
	m_fFrustumRange = 5.f;

	if (FAILED(__super::Initialize(pArg)))
		__debugbreak();

	// 실제 위치
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(138.44f, 22.39f, 118.12f, 1.f));
	// 테스트 위치
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(320.f, 11.f, 250.f, 1.f));
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(430.f, -150.f, 520.f, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(400.f, -150.f, 550.f, 1.f));


	//m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));

	if (FAILED(SetUp_Components()))
		__debugbreak();

	FAILEDCHECK(SetUp_State_Handler());
	//FAILEDCHECK(Create_UI_Status("RobellaStatus"));

	m_pInfo = new UNITDESC();
	m_pCurInfo = new UNITDESC();

	*m_pInfo = UNITDESC(ELEMENTAL_DEFAULT, 20000.f, 10.f, 2.f);
	//memcpy(m_pInfo, &UNITDESC(ELEMENTAL_DEFAULT, 20000.f, 10.f, 2.f), sizeof(UNITDESC));
	memcpy(m_pCurInfo, m_pInfo, sizeof(UNITDESC));

	//m_vTargetIslandPos[0] = { 330.f, 92.f, -46.f, 1.f};		//옹로
	//m_vTargetIslandPos[1] = { 414.f, 90.f, 537.f, 1.f };	//로웨
	//m_vTargetIslandPos[2] = { 290.f, 118.f, 225.f, 1.f };	//웨용

	//m_vTargetIslandPos[0] = { -122.f, 130.f, 137.f, 1.f };	//옹로
	//m_vTargetIslandPos[1] = { 345.f, 130.f, -128.f, 1.f };	//로웨
	//m_vTargetIslandPos[2] = { 435.f, 130.f, 583.f, 1.f };	//웨용

	m_vTargetIslandPos[0] = { 18.f, 130.f, 210.f, 1.f };	//옹로
	m_vTargetIslandPos[1] = { 335.f, 130.f, -8.f, 1.f };	//로웨
	m_vTargetIslandPos[2] = { 380.f, 130.f, 494.f, 1.f };	//웨용


	GAMEINSTANCE->Add_CallbackList(this, EVENT_LEVIATHAN_APEAR);
	return S_OK;
}

void CEnemy_Boss_Leviathan::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//USEGAMEINSTANCE;
	//if (KEY_PRESSING('K'))
	//	m_pTransformCom->Go_Front(fTimeDelta, 20.f);

	m_pState_Handler->Tick(fTimeDelta);
}

void CEnemy_Boss_Leviathan::LateTick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	//if (!m_pTransformCom->Is_InAir())
	//	m_pNaviCom->SetUp_OnTerrain(m_pTransformCom);
	//if (IsInFrustum())
	//{
	pGameInstance->Add_RenderGroup(RENDER_NONALPHABLEND, this);
	//}

	//for (auto& iter : *m_pCol_Volume->Get_StayColliders())
	//{
	//	Push_EachOther(iter, m_pCol_Volume, m_pTransformCom);
	//}

	pGameInstance->Add_ColliderGroup(ID_UINT(COL_ENEMY_ATTACK), m_pCol_Attack);
	pGameInstance->Add_ColliderGroup(ID_UINT(COL_ENEMY_CHASERANGE), m_pCol_AttackRange);
	pGameInstance->Add_ColliderGroup(ID_UINT(COL_ENEMY_HITRANGE), m_pCol_Body);
	pGameInstance->Add_ColliderGroup(ID_UINT(COL_ENEMY_BOSS_VOLUME), m_pCol_Volume);

}

HRESULT CEnemy_Boss_Leviathan::Render()
{
	__super::Render();

#ifdef _DEBUG
	m_pCol_Attack->Render();
	m_pCol_AttackRange->Render();
	m_pCol_Body->Render();
	m_pCol_Volume->Render();
#endif // DEBUG

	_float4 vColor = { 1.f, 1.f, 1.f, 1.f };
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vSourColor", &vColor, sizeof(_float4)));
	_vector vAlphaZeroDiffuse = XMVectorSet(1.f, 1.f, 1.f, 1.f);
	_vector vAlphaZeroGlow = XMVectorSet(1.f, 1.f, 1.f, 1.f);
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_AlphaZeroDiffuse", &vAlphaZeroDiffuse, sizeof(_float4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_AlphaZeroGlow", &vAlphaZeroGlow, sizeof(_float4)));

	_bool bUseRimLight = false;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bRimLight", &bUseRimLight, sizeof(_bool)));
	return S_OK;
}

void CEnemy_Boss_Leviathan::On_Dead()
{
	USEGAMEINSTANCE;
	pGameInstance->Add_Event(EVENT_LEVIATHAN_DEAD, nullptr, nullptr);
	pGameInstance->Play_ActionCamera(TEXT("Final_Cut_Scene"), 0.f);
}

void CEnemy_Boss_Leviathan::On_EventMessege(EVENTDESC tEvent)
{
	if (EVENT_LEVIATHAN_APEAR == tEvent.eEventType)
	{
		Create_UI_Status("LeviathanStatus");
	}
}

HRESULT CEnemy_Boss_Leviathan::SetUp_Components()
{
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXANIM), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_LEVIATHAN_40), ID_UINT(MODEL_DYNAMIC), (CComponent**)&m_pModelCom));
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(STATE_HANDLER), ID_UINT(STATE_HANDLER), (CComponent**)&m_pState_Handler));
	m_pModelCom->Set_Owner(this);
	m_pState_Handler->Set_Owner(this);

	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_NOISE), ID_UINT(TEXTURE), (CComponent**)&m_pDissolveTextureCom));
	m_iDissolveTextureNoise = 252;
	m_iDissolveTextureColor = 32;

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 1.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK), (CComponent**)&m_pCol_Attack, &ColliderDesc));
	m_pCol_Attack->Set_Owner(this);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(40.f, 40.f, 40.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 1.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACKRANGE), (CComponent**)&m_pCol_AttackRange, &ColliderDesc));
	m_pCol_AttackRange->Set_Owner(this);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(8.f, 8.f, 8.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 1.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_HITRANGE), (CComponent**)&m_pCol_Body, &ColliderDesc));
	m_pCol_Body->Set_Owner(this);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(5.f, 5.f, 5.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 1.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_BOSS_VOLUME), (CComponent**)&m_pCol_Volume, &ColliderDesc));
	m_pCol_Volume->Set_Owner(this);

	/* For.Com_Navigation */
	CNavigation::NAVIDESC		NaviDesc;
	NaviDesc.m_iCurrentIndex = 0;
	NaviDesc.pOwner = this;
	if (FAILED(__super::Add_Component(m_iCurLevel, ID_UINT(NAVIGATION), ID_UINT(NAVIGATION), (CComponent**)&m_pNaviCom, &NaviDesc)))
		return E_FAIL;
	m_pNaviCom->Find_CurrentIndex(m_pTransformCom);

	return S_OK;
}

HRESULT CEnemy_Boss_Leviathan::SetUp_ShaderResource()
{
	FAILEDCHECK(__super::SetUp_ShaderResource());

	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_vSourColor", &m_vColor, sizeof(_float4)));
	_vector vAlphaZeroDiffuse = XMVectorSet(1.f, 0.f, 0.f, 0.95f);
	_vector vAlphaZeroGlow = XMVectorSet(1.f, 0.41f, 0.16f, 0.47f);
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_AlphaZeroDiffuse", &vAlphaZeroDiffuse, sizeof(_float4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_AlphaZeroGlow", &vAlphaZeroGlow, sizeof(_float4)));


	_bool	bUseToon = false;
	//_bool	bUseOutline = false;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bToon", &bUseToon, sizeof(_bool)));
	//FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bOutline", &bUseOutline, sizeof(_bool)));

	return S_OK;
}

HRESULT CEnemy_Boss_Leviathan::SetUp_State_Handler()
{
	m_pState_Handler->Set_ModelComponent(m_pModelCom);

	/*

		STATE_BOSS_LEVIATHAN_IDLE,
		STATE_BOSS_LEVIATHAN_APEAR,
		STATE_BOSS_LEVIATHAN_WEAK,
		STATE_BOSS_LEVIATHAN_JUMP_TURNL,
		STATE_BOSS_LEVIATHAN_JUMP_TURNR,

		STATE_BOSS_LEVIATHAN_ATTACK_DOWN_FR,
		STATE_BOSS_LEVIATHAN_ATTACK_DOWN_BR,
		STATE_BOSS_LEVIATHAN_ATTACK_DOWN_F,
		STATE_BOSS_LEVIATHAN_ATTACK_UP_FL,
		STATE_BOSS_LEVIATHAN_ATTACK_UP_BL,
		STATE_BOSS_LEVIATHAN_ATTACK_UP_F,
		STATE_BOSS_LEVIATHAN_END,*/

	m_pState_Handler->Set_Animation(STATE_BOSS_LEVIATHAN_APEAR, 10);

	m_pState_Handler->Set_Animation(STATE_BOSS_LEVIATHAN_IDLE, 12);				// D -D
	m_pState_Handler->Set_Animation(STATE_BOSS_LEVIATHAN_WALK, 13);
	m_pState_Handler->Set_Animation(STATE_BOSS_LEVIATHAN_WEAK, 14);
	m_pState_Handler->Set_Animation(STATE_BOSS_LEVIATHAN_JUMP_TURNL, 10);
	m_pState_Handler->Set_Animation(STATE_BOSS_LEVIATHAN_JUMP_TURNR, 11);

	m_pState_Handler->Set_Animation(STATE_BOSS_LEVIATHAN_ATTACK_DOWN_FR, 0);
	m_pState_Handler->Set_Animation(STATE_BOSS_LEVIATHAN_ATTACK_DOWN_BL_A, 2);
	m_pState_Handler->Set_Animation(STATE_BOSS_LEVIATHAN_ATTACK_DOWN_BL_B, 3);
	m_pState_Handler->Set_Animation(STATE_BOSS_LEVIATHAN_ATTACK_DOWN_BL_C, 4);
	m_pState_Handler->Set_Animation(STATE_BOSS_LEVIATHAN_ATTACK_DOWN_F_A, 6);
	m_pState_Handler->Set_Animation(STATE_BOSS_LEVIATHAN_ATTACK_DOWN_F_B, 7);
	m_pState_Handler->Set_Animation(STATE_BOSS_LEVIATHAN_ATTACK_DOWN_F_C, 8);
	m_pState_Handler->Set_Animation(STATE_BOSS_LEVIATHAN_ATTACK_UP_FL, 1);
	m_pState_Handler->Set_Animation(STATE_BOSS_LEVIATHAN_ATTACK_UP_BR, 5);
	m_pState_Handler->Set_Animation(STATE_BOSS_LEVIATHAN_ATTACK_UP_F, 9);

	m_pState_Handler->Set_Animation(STATE_BOSS_LEVIATHAN_ROAR_0, 9);
	m_pState_Handler->Set_Animation(STATE_BOSS_LEVIATHAN_LOOK_CENTER_ROAR, 9);
	m_pState_Handler->Set_Animation(STATE_BOSS_LEVIATHAN_LOOK_OUT_ROAR, 9);
	m_pState_Handler->Set_Animation(STATE_BOSS_LEVIATHAN_TURN, 10);
	m_pState_Handler->Set_Animation(STATE_BOSS_LEVIATHAN_JUMP_TURN, 10);

	m_pState_Handler->Set_Animation(STATE_BOSS_LEVIATHAN_DEAD, 14);

	//m_pState_Handler->Set_Animation(STATE_BOSS_LEVIATHAN_TURN_ROAR_1, 9);
	//m_pState_Handler->Set_Animation(STATE_BOSS_LEVIATHAN_LOOK_CENTER_ROAR, 9);

	m_pState_Handler->SetUp_StartState(STATE_BOSS_LEVIATHAN_IDLE);

	//m_pModelCom->Set_AnimationRotateMatrix(XMMatrixRotationY(XMConvertToRadians(180.0f)), 2);
	//m_pModelCom->Set_AnimationRotateMatrix(XMMatrixRotationY(XMConvertToRadians(180.0f)), 5);
	//m_pModelCom->Set_AnimationRotateMatrix(XMMatrixRotationY(XMConvertToRadians(180.0f)), 9);

	//m_pModelCom->Set_AnimationRotateMatrix(XMMatrixRotationY(XMConvertToRadians(-90.0f)), 3);
	//m_pModelCom->Set_AnimationRotateMatrix(XMMatrixRotationY(XMConvertToRadians(-90.0f)), 4);



	return S_OK;
}


CEnemy_Boss_Leviathan* CEnemy_Boss_Leviathan::Create()
{
	CEnemy_Boss_Leviathan* pInstance = new CEnemy_Boss_Leviathan();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEnemy_Boss_Leviathan");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnemy_Boss_Leviathan::Clone(void* pArg)
{
	CEnemy_Boss_Leviathan* pInstance = new CEnemy_Boss_Leviathan(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEnemy_Boss_Leviathan");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnemy_Boss_Leviathan::Free()
{
	GAMEINSTANCE->Remove_CallbackList(this, EVENT_LEVIATHAN_APEAR);
	__super::Free();
}
