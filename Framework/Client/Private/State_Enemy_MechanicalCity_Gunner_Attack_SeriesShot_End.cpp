#include "stdafx.h"
#include "..\Public\State_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_End.h"
#include "GameInstance.h"
#include "Enemy.h"
CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_End::CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_End(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_End::CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_End(const CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_End& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_End::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_DEAD_END);


	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_B);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_F);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_L);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_R);

	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_IDLE_BATTLE);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_IDLE_NORMAL);


	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_B] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_F] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_L] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_R] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_IDLE_BATTLE] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_IDLE_NORMAL] = 1;


	m_fAttackLength_Min = 2.5f;
	m_fAttackLength_Max = 10.f;

	return S_OK;
}

HRESULT CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_End::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_End::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fAnimSpeed = 1.f;
}

void CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_End::Tick(_float fTimeDelta)
{
	if (Is_Damaged())
		m_bEndCurState = true;

	if (Is_AnimFinished())
		m_bEndCurState = true;

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_End::Exit(void* pArg)
{
}

_bool CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_End::Check_Condition(CState_Handler* pHandler)
{
	if (Is_Damaged(pHandler))
		return false;

	return true;
}


CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_End* CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_End::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_End* pInstance = new CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_End(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_End::Clone(void* pArg)
{
	CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_End* pInstance = new CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_End(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_End::Free()
{
	__super::Free();
}
