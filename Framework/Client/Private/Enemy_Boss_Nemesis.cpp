#include "stdafx.h"
#include "State_Handler.h"

#include "GameInstance.h"
#include "Collider.h"
#include "Sound_Manager.h"
#include "..\Public\Enemy_Boss_Nemesis.h"

#include "Navigation.h"

CEnemy_Boss_Nemesis::CEnemy_Boss_Nemesis()
	: CEnemy_Boss()
{
}

CEnemy_Boss_Nemesis::CEnemy_Boss_Nemesis(const CEnemy_Boss& rhs)
	: CEnemy_Boss(rhs)
{
}

HRESULT CEnemy_Boss_Nemesis::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemy_Boss_Nemesis::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		__debugbreak();

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(-5.f, 0.f, 5.f, 1.f));

	if (FAILED(SetUp_Components()))
		__debugbreak();

	FAILEDCHECK(SetUp_State_Handler());

	m_pInfo = new UNITDESC();
	m_pCurInfo = new UNITDESC();

	memcpy(m_pInfo, &UNITDESC(ELEMENTAL_DEFAULT, 100.f, 10.f, 2.f), sizeof(UNITDESC));
	memcpy(m_pCurInfo, m_pInfo, sizeof(UNITDESC));

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_ENEMY, this);
	pGameInstance->Add_LateTickGroup(TG_ENEMY, this);

	return S_OK;
}

void CEnemy_Boss_Nemesis::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;

	m_pState_Handler->Tick(fTimeDelta);
}

void CEnemy_Boss_Nemesis::LateTick(_float fTimeDelta)
{
	if (!m_pTransformCom->Is_InAir())
		m_pNaviCom->SetUp_OnTerrain(m_pTransformCom);

	GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);
}

HRESULT CEnemy_Boss_Nemesis::SetUp_Components()
{
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXANIM), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_DYNAMIC, MODEL_ENEMY_BOSS, MODEL_ENEMY_BOSS_NEMESIS), ID_UINT(MODEL_DYNAMIC), (CComponent**)&m_pModelCom));
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(STATE_HANDLER), ID_UINT(STATE_HANDLER), (CComponent**)&m_pState_Handler));
	m_pModelCom->Set_Owner(this);
	m_pState_Handler->Set_Owner(this);

	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	/* For.Com_Navigation */
	CNavigation::NAVIDESC		NaviDesc;
	NaviDesc.m_iCurrentIndex = 0;
	NaviDesc.pOwner = this;
	if (FAILED(__super::Add_Component(m_iCurLevel, ID_UINT(NAVIGATION), ID_UINT(NAVIGATION), (CComponent**)&m_pNaviCom, &NaviDesc)))
		return E_FAIL;
	m_pNaviCom->Find_CurrentIndex(m_pTransformCom);

	return S_OK;
}

HRESULT CEnemy_Boss_Nemesis::SetUp_ShaderResource()
{
	FAILEDCHECK(__super::SetUp_ShaderResource());

	return S_OK;
}

HRESULT CEnemy_Boss_Nemesis::SetUp_State_Handler()
{
	m_pState_Handler->Set_ModelComponent(m_pModelCom);

	//m_pState_Handler->Set_Animation(STATE_BOSS_NEMESIS_PATROL_STAND, 35);
	//m_pState_Handler->Set_Animation(STATE_BOSS_NEMESIS_PATROL_AWAKE, 11);

	//m_pState_Handler->Set_Animation(STATE_BOSS_NEMESIS_IDLE, 14);
	//m_pState_Handler->Set_Animation(STATE_BOSS_NEMESIS_WALK_F, 19);
	//m_pState_Handler->Set_Animation(STATE_BOSS_NEMESIS_RUN, 12);

	//m_pState_Handler->Set_Animation(STATE_BOSS_NEMESIS_ATTACK_01, 0);
	//m_pState_Handler->Set_Animation(STATE_BOSS_NEMESIS_ATTACK_02, 1);
	//m_pState_Handler->Set_Animation(STATE_BOSS_NEMESIS_ATTACK_03, 2);
	//m_pState_Handler->Set_Animation(STATE_BOSS_NEMESIS_ATTACK_04, 3);
	//m_pState_Handler->Set_Animation(STATE_BOSS_RUDOLF_SHIELD, 46); 

	//m_pState_Handler->SetUp_StartState(STATE_BOSS_NEMESIS_PATROL_STAND);

	return S_OK;
}


CEnemy_Boss_Nemesis* CEnemy_Boss_Nemesis::Create()
{
	CEnemy_Boss_Nemesis* pInstance = new CEnemy_Boss_Nemesis();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEnemy_Boss_Nemesis");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnemy_Boss_Nemesis::Clone(void* pArg)
{
	CEnemy_Boss_Nemesis* pInstance = new CEnemy_Boss_Nemesis(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEnemy_Boss_Nemesis");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnemy_Boss_Nemesis::Free()
{
	__super::Free();
}
