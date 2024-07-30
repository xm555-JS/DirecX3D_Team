#include "stdafx.h"
#include "..\Public\Gangster_Cap.h"
#include "State_Handler.h"
#include "Navigation.h"

#include "GameInstance.h"

#include "Collider.h"
#include "Weapon.h"
#include "Quest_Manager.h"

/*갱스터 캡*/
CGangster_Cap::CGangster_Cap()
	: CEnemy_Normal()
{
}

CGangster_Cap::CGangster_Cap(const CGangster_Cap & rhs)
	: CEnemy_Normal(rhs)
{
}

HRESULT CGangster_Cap::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGangster_Cap::Initialize(void * pArg)
{

	m_fFrustumRange = 3.f;

	FAILEDCHECK(__super::Initialize(pArg));

	if (FAILED(SetUp_Components()))
		__debugbreak();

	USEGAMEINSTANCE;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_vStartPos));
	
	FAILEDCHECK(SetUp_State_Handler());
	FAILEDCHECK(Create_UI_Status(3, 3.75f, true));
	//Create_UI_Place_Mark(2.f);

	FAILEDCHECK(SetUp_Weapon());
	return S_OK;
}

void CGangster_Cap::Tick(_float fTimeDelta)
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
	if (QUEST_KILL_GANG_SWARM == CQuest_Manager::Get_Instance()->Get_CurQuset())
	{
		IMPL_ONCE(m_bCreate_PlaceMark, Create_UI_Place_Mark(4.f));
	}
	if (m_pCurInfo->fHP <= 0)
	{
		Pooling_UI_PlaceMark();
	}
}

void CGangster_Cap::LateTick(_float fTimeDelta)
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
	{
		GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);
	}
}

HRESULT CGangster_Cap::Render()
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

void CGangster_Cap::On_Dead()
{
	USEGAMEINSTANCE;
	pGameInstance->Add_Event(EVENT_MONSTER_DEAD, this, nullptr);

	if (m_pWeapon)
		pGameInstance->Add_DeadObject(m_pWeapon);
}

HRESULT CGangster_Cap::SetUp_Components()
{
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXANIM), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY, GANGSTER_CAP), ID_UINT(MODEL_DYNAMIC), (CComponent**)&m_pModelCom));
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(STATE_HANDLER), ID_UINT(STATE_HANDLER), (CComponent**)&m_pState_Handler));
	m_pModelCom->Set_Owner(this);
	m_pState_Handler->Set_Owner(this);
	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(TEXTURE, TEXTURE_EFFECT, TEXTURE_EFFECT_NOISE), ID_UINT(TEXTURE), (CComponent**)&m_pDissolveTextureCom));
	m_iDissolveTextureNoise = 28;
	m_iDissolveTextureColor = 46;

	CCollider::COLLIDERDESC			ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(3.f, 3.f, 3.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 2.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK), (CComponent**)&m_pCol_Sphere[COL_ENEMY_ATTACK], &ColliderDesc));
	m_pCol_Sphere[COL_ENEMY_ATTACK]->Set_Owner(this);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(3.f, 3.f, 3.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 2.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_VOLUME), (CComponent**)&m_pCol_Sphere[COL_ENEMY_VOLUME], &ColliderDesc));
	m_pCol_Sphere[COL_ENEMY_VOLUME]->Set_Owner(this);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(3.f, 3.f, 3.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 2.f, 0.f);
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


HRESULT CGangster_Cap::SetUp_State_Handler()
{
	m_pState_Handler->Set_ModelComponent(m_pModelCom);

	m_pState_Handler->Set_Animation(STATE_ENEMY_GANGSTER_CAP_DANCE, 12);
	m_pState_Handler->Set_Animation(STATE_ENEMY_GANGSTER_CAP_SLEEP_END, 18);
	m_pState_Handler->Set_Animation(STATE_ENEMY_GANGSTER_CAP_SLEEP_1, 17);
	m_pState_Handler->Set_Animation(STATE_ENEMY_GANGSTER_CAP_STANDUP, 20);

	m_pState_Handler->Set_Animation(STATE_ENEMY_GANGSTER_CAP_IDLE_NORMAL, 1);
	m_pState_Handler->Set_Animation(STATE_ENEMY_GANGSTER_CAP_IDLE_BATTLE, 1);
	m_pState_Handler->Set_Animation(STATE_ENEMY_GANGSTER_CAP_WALK, 70);
	m_pState_Handler->Set_Animation(STATE_ENEMY_GANGSTER_CAP_FIND, 68);

	m_pState_Handler->Set_Animation(STATE_ENEMY_GANGSTER_CAP_RUN, 41);
	m_pState_Handler->Set_Animation(STATE_ENEMY_GANGSTER_CAP_RUN_RETURN, 41);

	m_pState_Handler->Set_Animation(STATE_ENEMY_GANGSTER_CAP_ATTACK_SWING, 35);
	m_pState_Handler->Set_Animation(STATE_ENEMY_GANGSTER_CAP_ATTACK_DASH, 36);
	m_pState_Handler->Set_Animation(STATE_ENEMY_GANGSTER_CAP_ATTACK_TURNTURN, 37);

	m_pState_Handler->Set_Animation(STATE_ENEMY_GANGSTER_CAP_TURN_L, 43);//43
	m_pState_Handler->Set_Animation(STATE_ENEMY_GANGSTER_CAP_TURN_R, 44);//44

	m_pState_Handler->Set_Animation(STATE_ENEMY_GANGSTER_CAP_WALK_B, 45);
	m_pState_Handler->Set_Animation(STATE_ENEMY_GANGSTER_CAP_WALK_F, 46);
	m_pState_Handler->Set_Animation(STATE_ENEMY_GANGSTER_CAP_WALK_L, 49);
	m_pState_Handler->Set_Animation(STATE_ENEMY_GANGSTER_CAP_WALK_R, 50);

	m_pState_Handler->Set_Animation(STATE_ENEMY_GANGSTER_CAP_DAMAGE_B, 53);
	m_pState_Handler->Set_Animation(STATE_ENEMY_GANGSTER_CAP_DAMAGE_L, 63);
	m_pState_Handler->Set_Animation(STATE_ENEMY_GANGSTER_CAP_DAMAGE_R, 64);

	m_pState_Handler->Set_Animation(STATE_ENEMY_GANGSTER_CAP_DEAD_1, 65);
	m_pState_Handler->Set_Animation(STATE_ENEMY_GANGSTER_CAP_DEAD_END, 66);

	m_pState_Handler->SetUp_StartState(STATE_ENEMY_GANGSTER_CAP_IDLE_NORMAL);

	return S_OK;
}

HRESULT CGangster_Cap::SetUp_Weapon()
{
	USEGAMEINSTANCE;
	m_pWeapon = (CWeapon*)pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(WEAPON, WEAPON_ENEMY_HAMMER), ID_UINT(WEAPON, WEAPON_ENEMY_HAMMER));
	m_pWeapon->Setup_WeaponState(this, string("weapon_R"));
	pGameInstance->Add_TickGroup(TG_WEAPON, m_pWeapon);
	pGameInstance->Add_LateTickGroup(LTG_WEAPON, m_pWeapon);
	return S_OK;
}

CGangster_Cap * CGangster_Cap::Create()
{
	CGangster_Cap*		pInstance = new CGangster_Cap();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGangster_Cap");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CGangster_Cap::Clone(void * pArg)
{
	CGangster_Cap*		pInstance = new CGangster_Cap(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGangster_Cap");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGangster_Cap::Free()
{
	__super::Free();
}
