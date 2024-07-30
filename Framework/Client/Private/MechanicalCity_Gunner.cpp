#include "stdafx.h"
#include "..\Public\MechanicalCity_Gunner.h"
#include "State_Handler.h"
#include "Navigation.h"
#include "GameInstance.h"


/*기계도시 총잡이*/
CMechanicalCity_Gunner::CMechanicalCity_Gunner()
	: CEnemy_Normal()
{
}

CMechanicalCity_Gunner::CMechanicalCity_Gunner(const CMechanicalCity_Gunner& rhs)
	: CEnemy_Normal(rhs)
{
}

HRESULT CMechanicalCity_Gunner::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMechanicalCity_Gunner::Initialize(void* pArg)
{
	m_fFrustumRange = 2.5f;

#if defined(_TEST_JJ)
	DeadTestKey();
#endif

	FAILEDCHECK(__super::Initialize(pArg));
	if (FAILED(SetUp_Components()))
		__debugbreak();

	USEGAMEINSTANCE;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_vStartPos));

	FAILEDCHECK(SetUp_State_Handler());
	FAILEDCHECK(Create_UI_Status(6, 1.8f));

	return S_OK;
}

void CMechanicalCity_Gunner::Tick(_float fTimeDelta)
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
}

void CMechanicalCity_Gunner::LateTick(_float fTimeDelta)
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

HRESULT CMechanicalCity_Gunner::Render()
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


HRESULT CMechanicalCity_Gunner::SetUp_Components()
{
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXANIM), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY, MECHANICALCITY_GUNNER), ID_UINT(MODEL_DYNAMIC), (CComponent**)&m_pModelCom));
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(STATE_HANDLER), ID_UINT(STATE_HANDLER), (CComponent**)&m_pState_Handler));
	m_pModelCom->Set_Owner(this);
	m_pState_Handler->Set_Owner(this);
	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_NOISE), ID_UINT(TEXTURE), (CComponent**)&m_pDissolveTextureCom));
	m_iDissolveTextureNoise = 19;
	m_iDissolveTextureColor = 1;


	CCollider::COLLIDERDESC			ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));//throw 공격시                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        0,2,5.5,5
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 1.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK), (CComponent**)&m_pCol_Sphere[COL_ENEMY_ATTACK], &ColliderDesc));
	m_pCol_Sphere[COL_ENEMY_ATTACK]->Set_Owner(this);


	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 1.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_VOLUME), (CComponent**)&m_pCol_Sphere[COL_ENEMY_VOLUME], &ColliderDesc));
	m_pCol_Sphere[COL_ENEMY_VOLUME]->Set_Owner(this);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 1.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_HITRANGE), (CComponent**)&m_pCol_Sphere[COL_ENEMY_HITRANGE], &ColliderDesc));
	m_pCol_Sphere[COL_ENEMY_HITRANGE]->Set_Owner(this);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(30.f, 30.f, 30.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 2.f, 0.f);
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

HRESULT CMechanicalCity_Gunner::SetUp_State_Handler()
{
	m_pState_Handler->Set_ModelComponent(m_pModelCom);


	m_pState_Handler->Set_Animation(STATE_ENEMY_MECHANICALCITY_GUNNER_IDLE_NORMAL, 42);
	m_pState_Handler->Set_Animation(STATE_ENEMY_MECHANICALCITY_GUNNER_IDLE_BATTLE, 9);
	m_pState_Handler->Set_Animation(STATE_ENEMY_MECHANICALCITY_GUNNER_FIND, 41);

	m_pState_Handler->Set_Animation(STATE_ENEMY_MECHANICALCITY_GUNNER_WALK, 48);
	m_pState_Handler->Set_Animation(STATE_ENEMY_MECHANICALCITY_GUNNER_RUN, 6);
	m_pState_Handler->Set_Animation(STATE_ENEMY_MECHANICALCITY_GUNNER_RUN_RETURN, 6);


	m_pState_Handler->Set_Animation(STATE_ENEMY_MECHANICALCITY_GUNNER_ATTACK_SERIESSHOT_BEGIN, 0);
	m_pState_Handler->Set_Animation(STATE_ENEMY_MECHANICALCITY_GUNNER_ATTACK_SERIESSHOT_ING, 1);
	m_pState_Handler->Set_Animation(STATE_ENEMY_MECHANICALCITY_GUNNER_ATTACK_SERIESSHOT_END, 2);
	m_pState_Handler->Set_Animation(STATE_ENEMY_MECHANICALCITY_GUNNER_ATTACK_THROWBOMB, 3);


	m_pState_Handler->Set_Animation(STATE_ENEMY_MECHANICALCITY_GUNNER_TURN_L, 11);
	m_pState_Handler->Set_Animation(STATE_ENEMY_MECHANICALCITY_GUNNER_TURN_R, 12);

	m_pState_Handler->Set_Animation(STATE_ENEMY_MECHANICALCITY_GUNNER_WALK_B, 13);
	m_pState_Handler->Set_Animation(STATE_ENEMY_MECHANICALCITY_GUNNER_WALK_F, 15);
	m_pState_Handler->Set_Animation(STATE_ENEMY_MECHANICALCITY_GUNNER_WALK_L, 17);
	m_pState_Handler->Set_Animation(STATE_ENEMY_MECHANICALCITY_GUNNER_WALK_R, 18);

	m_pState_Handler->Set_Animation(STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_B, 24);
	m_pState_Handler->Set_Animation(STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_F, 25);
	m_pState_Handler->Set_Animation(STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_L, 26);
	m_pState_Handler->Set_Animation(STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_R, 27);

	m_pState_Handler->Set_Animation(STATE_ENEMY_MECHANICALCITY_GUNNER_DEAD_1, 40);
	m_pState_Handler->Set_Animation(STATE_ENEMY_MECHANICALCITY_GUNNER_DEAD_END, 40);


	m_pState_Handler->SetUp_StartState(STATE_ENEMY_MECHANICALCITY_GUNNER_IDLE_NORMAL);

	return S_OK;
}

CMechanicalCity_Gunner* CMechanicalCity_Gunner::Create()
{
	CMechanicalCity_Gunner* pInstance = new CMechanicalCity_Gunner();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMechanicalCity_Gunner");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMechanicalCity_Gunner::Clone(void* pArg)
{
	CMechanicalCity_Gunner* pInstance = new CMechanicalCity_Gunner(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMechanicalCity_Gunner");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMechanicalCity_Gunner::Free()
{
	__super::Free();
}
