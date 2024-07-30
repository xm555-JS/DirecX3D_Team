#include "stdafx.h"
#include "..\Public\Habaka_Admiral.h"
#include "State_Handler.h"
#include "Navigation.h"
#include "Quest_Manager.h"

#include "GameInstance.h"
#include "Imgui_Manager.h"

/*하바카 제독*/
CHabaka_Admiral::CHabaka_Admiral()
	: CEnemy_Normal()
{
}

CHabaka_Admiral::CHabaka_Admiral(const CHabaka_Admiral& rhs)
	: CEnemy_Normal(rhs)
{
}

HRESULT CHabaka_Admiral::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHabaka_Admiral::Initialize(void* pArg)
{
	m_fFrustumRange = 4.f;

	FAILEDCHECK(__super::Initialize(pArg));

	if (FAILED(SetUp_Components()))
		__debugbreak();

	USEGAMEINSTANCE;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_vStartPos));

	FAILEDCHECK(SetUp_State_Handler());
	FAILEDCHECK(Create_UI_Status(8, 6.2f, true));

	return S_OK;
}

void CHabaka_Admiral::Tick(_float fTimeDelta)
{
#if defined(_TEST_JJ)
	DeadTestKey();
#endif
	if (true == m_bDead)
		return;

	__super::Tick(fTimeDelta);

	m_pState_Handler->Tick(fTimeDelta);

	m_pCol_Sphere[COL_ENEMY_ATTACK]->Update(m_pTransformCom->Get_WorldMatrix());
	m_pCol_Sphere[COL_ENEMY_VOLUME]->Update(m_pTransformCom->Get_WorldMatrix());
	m_pCol_Sphere[COL_ENEMY_HITRANGE]->Update(m_pTransformCom->Get_WorldMatrix());
	m_pCol_Sphere[COL_ENEMY_ATTACKRANGE]->Update(m_pTransformCom->Get_WorldMatrix());
	m_pTransformCom->Tick(fTimeDelta);

	USEGAMEINSTANCE;

	for (auto& iter : *m_pCol_Sphere[COL_ENEMY_VOLUME]->Get_StayColliders())
	{
		Push_EachOther(iter, m_pCol_Sphere[COL_ENEMY_VOLUME], m_pTransformCom, m_pNaviCom);
	}

	//타겟 설정
	for (auto& iter : *m_pCol_Sphere[COL_ENEMY_ATTACKRANGE]->Get_StayColliders())
	{
		m_pTarget = iter->Get_Owner();
	}
	for (auto& iter : *m_pCol_Sphere[COL_ENEMY_ATTACKRANGE]->Get_ExitColliders())
	{
		m_pTarget = nullptr;
	}

	//퀘스트있을때 PlaceMark 생성
	if (QUEST_KILL_MUTATION == CQuest_Manager::Get_Instance()->Get_CurQuset())
	{
		IMPL_ONCE(m_bCreate_PlaceMark, Create_UI_Place_Mark(4.f));
	}
	if (m_pCurInfo->fHP <= 0)
	{
		Pooling_UI_PlaceMark();
	}
}

void CHabaka_Admiral::LateTick(_float fTimeDelta)
{
#ifndef _MAPTOOL
	if (!m_pTransformCom->Is_InAir())
		m_pNaviCom->SetUp_OnTerrain(m_pTransformCom);
#endif

	USEGAMEINSTANCE;
	pGameInstance->Add_ColliderGroup(ID_UINT(COL_ENEMY_ATTACK), m_pCol_Sphere[COL_ENEMY_ATTACK]);
	pGameInstance->Add_ColliderGroup(ID_UINT(COL_ENEMY_VOLUME), m_pCol_Sphere[COL_ENEMY_VOLUME]);
	pGameInstance->Add_ColliderGroup(ID_UINT(COL_ENEMY_HITRANGE), m_pCol_Sphere[COL_ENEMY_HITRANGE]);
	pGameInstance->Add_ColliderGroup(ID_UINT(COL_ENEMY_ATTACKRANGE), m_pCol_Sphere[COL_ENEMY_ATTACKRANGE]);
	if (IsInFrustum())
		GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);

}

HRESULT CHabaka_Admiral::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		__debugbreak();

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		__debugbreak();

	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE));

		m_pModelCom->Render(i, m_pShaderCom, 0, "g_Bones");
	}

	_vector vAlphaZeroDiffuse = XMVectorSet(1.f, 1.f, 1.f, 1.f);
	_vector vAlphaZeroGlow = XMVectorSet(1.f, 1.f, 1.f, 1.f);
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_AlphaZeroDiffuse", &vAlphaZeroDiffuse, sizeof(_float4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_AlphaZeroGlow", &vAlphaZeroGlow, sizeof(_float4)));

#ifdef _DEBUG
	m_pCol_Sphere[COL_ENEMY_ATTACK]->Render();
	m_pCol_Sphere[COL_ENEMY_VOLUME]->Render();
	m_pCol_Sphere[COL_ENEMY_HITRANGE]->Render();
	m_pCol_Sphere[COL_ENEMY_ATTACKRANGE]->Render();
#endif // _DEBUG

	_bool bDissolve = false;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bIsDissolve", &bDissolve, sizeof(_bool)));

	_bool bUseRimLight = false;
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_bRimLight", &bUseRimLight, sizeof(_bool)));
	return S_OK;
}


HRESULT CHabaka_Admiral::SetUp_Components()
{
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXANIM), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY, HABAKA_ADMIRAL), ID_UINT(MODEL_DYNAMIC), (CComponent**)&m_pModelCom));
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(STATE_HANDLER), ID_UINT(STATE_HANDLER), (CComponent**)&m_pState_Handler));
	m_pModelCom->Set_Owner(this);
	m_pState_Handler->Set_Owner(this);
	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_NOISE), ID_UINT(TEXTURE), (CComponent**)&m_pDissolveTextureCom));
	m_iDissolveTextureNoise = 87;
	m_iDissolveTextureColor = 32;

	CCollider::COLLIDERDESC			ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(5.f, 5.f, 5.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 3.f, 2.5f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK), (CComponent**)&m_pCol_Sphere[COL_ENEMY_ATTACK], &ColliderDesc));
	m_pCol_Sphere[COL_ENEMY_ATTACK]->Set_Owner(this);


	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(5.f, 5.f, 5.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 3.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_VOLUME), (CComponent**)&m_pCol_Sphere[COL_ENEMY_VOLUME], &ColliderDesc));
	m_pCol_Sphere[COL_ENEMY_VOLUME]->Set_Owner(this);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(5.f, 5.f, 5.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 3.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_HITRANGE), (CComponent**)&m_pCol_Sphere[COL_ENEMY_HITRANGE], &ColliderDesc));
	m_pCol_Sphere[COL_ENEMY_HITRANGE]->Set_Owner(this);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(20.f, 20.f, 20.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 3.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACKRANGE), (CComponent**)&m_pCol_Sphere[COL_ENEMY_ATTACKRANGE], &ColliderDesc));
	m_pCol_Sphere[COL_ENEMY_ATTACKRANGE]->Set_Owner(this);

	/* For.Com_Navigation */
	CNavigation::NAVIDESC		NaviDesc;
	NaviDesc.m_iCurrentIndex = 0;
	NaviDesc.pOwner = this;
	if (FAILED(__super::Add_Component(m_iCurLevel, ID_UINT(NAVIGATION), ID_UINT(NAVIGATION), (CComponent**)&m_pNaviCom, &NaviDesc)))
		return E_FAIL;
	m_pNaviCom->Find_CurrentIndex(m_pTransformCom);
	return S_OK;
}

HRESULT CHabaka_Admiral::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		__debugbreak();

	__super::SetUp_ShaderResource();

	USEGAMEINSTANCE;

	_vector vAlphaZeroDiffuse = XMVectorSet(0.8f, 0.5f, 0.8f, 0.7f);
	_vector vAlphaZeroGlow = XMVectorSet(0.6f, 0.1f, 0.6f, 0.f);
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_AlphaZeroDiffuse", &vAlphaZeroDiffuse, sizeof(_float4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_AlphaZeroGlow", &vAlphaZeroGlow, sizeof(_float4)));

	return S_OK;
}

HRESULT CHabaka_Admiral::SetUp_State_Handler()
{
	m_pState_Handler->Set_ModelComponent(m_pModelCom);


	m_pState_Handler->Set_Animation(STATE_ENEMY_HABAKA_ADMIRAL_IDLE_NORMAL, 21);
	m_pState_Handler->Set_Animation(STATE_ENEMY_HABAKA_ADMIRAL_IDLE_BATTLE, 21);
	m_pState_Handler->Set_Animation(STATE_ENEMY_HABAKA_ADMIRAL_FIND, 41);

	m_pState_Handler->Set_Animation(STATE_ENEMY_HABAKA_ADMIRAL_WALK, 28);
	m_pState_Handler->Set_Animation(STATE_ENEMY_HABAKA_ADMIRAL_RUN, 19);
	m_pState_Handler->Set_Animation(STATE_ENEMY_HABAKA_ADMIRAL_RUN_RETURN, 19);


	m_pState_Handler->Set_Animation(STATE_ENEMY_HABAKA_ADMIRAL_ATTACK_FRONTJUMP, 1);
	m_pState_Handler->Set_Animation(STATE_ENEMY_HABAKA_ADMIRAL_ATTACK_PUNCH_A, 2);
	m_pState_Handler->Set_Animation(STATE_ENEMY_HABAKA_ADMIRAL_ATTACK_PUNCH_B, 7);
	m_pState_Handler->Set_Animation(STATE_ENEMY_HABAKA_ADMIRAL_ATTACK_KICK_A, 4);
	m_pState_Handler->Set_Animation(STATE_ENEMY_HABAKA_ADMIRAL_ATTACK_KICK_B, 9);
	m_pState_Handler->Set_Animation(STATE_ENEMY_HABAKA_ADMIRAL_ATTACK_SKILL_A, 15);


	m_pState_Handler->Set_Animation(STATE_ENEMY_HABAKA_ADMIRAL_TURN_L, 23);
	m_pState_Handler->Set_Animation(STATE_ENEMY_HABAKA_ADMIRAL_TURN_R, 25);

	m_pState_Handler->Set_Animation(STATE_ENEMY_HABAKA_ADMIRAL_WALK_B, 27);
	m_pState_Handler->Set_Animation(STATE_ENEMY_HABAKA_ADMIRAL_WALK_F, 28);
	m_pState_Handler->Set_Animation(STATE_ENEMY_HABAKA_ADMIRAL_WALK_L, 30);
	m_pState_Handler->Set_Animation(STATE_ENEMY_HABAKA_ADMIRAL_WALK_R, 31);

	m_pState_Handler->Set_Animation(STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_B, 17);
	m_pState_Handler->Set_Animation(STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_L, 17);
	m_pState_Handler->Set_Animation(STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_R, 17);
	m_pState_Handler->Set_Animation(STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_F, 17);

	m_pState_Handler->Set_Animation(STATE_ENEMY_HABAKA_ADMIRAL_DEAD_1, 38);
	m_pState_Handler->Set_Animation(STATE_ENEMY_HABAKA_ADMIRAL_DEAD_END, 38);


	m_pState_Handler->SetUp_StartState(STATE_ENEMY_HABAKA_ADMIRAL_IDLE_NORMAL);

	return S_OK;
}

CHabaka_Admiral* CHabaka_Admiral::Create()
{
	CHabaka_Admiral* pInstance = new CHabaka_Admiral();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHabaka_Admiral");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHabaka_Admiral::Clone(void* pArg)
{
	CHabaka_Admiral* pInstance = new CHabaka_Admiral(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CHabaka_Admiral");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHabaka_Admiral::Free()
{
	__super::Free();
}
