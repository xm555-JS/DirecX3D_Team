#include "stdafx.h"
#include "..\Public\State_Enemy_Mutation_Mechanic_Idle_Normal.h"
#include "GameInstance.h"

CState_Enemy_Mutation_Mechanic_Idle_Normal::CState_Enemy_Mutation_Mechanic_Idle_Normal(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_Mutation_Mechanic_Idle_Normal::CState_Enemy_Mutation_Mechanic_Idle_Normal(const CState_Enemy_Mutation_Mechanic_Idle_Normal& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_Mutation_Mechanic_Idle_Normal::Initialize_Prototype()
{
#ifndef _MAPTOOL
	m_ForceStates.push_back(STATE_ENEMY_MUTATION_MECHANIC_DEAD_1);

	m_ReserveStates.push_back(STATE_ENEMY_MUTATION_MECHANIC_RUN);
	m_ReserveProbabilitys[STATE_ENEMY_MUTATION_MECHANIC_RUN] = 1;

	m_ReserveStates.push_back(STATE_ENEMY_MUTATION_MECHANIC_DAMAGE_B);
	m_ReserveProbabilitys[STATE_ENEMY_MUTATION_MECHANIC_DAMAGE_B] = 1;
	m_ReserveStates.push_back(STATE_ENEMY_MUTATION_MECHANIC_DAMAGE_L);
	m_ReserveProbabilitys[STATE_ENEMY_MUTATION_MECHANIC_DAMAGE_L] = 1;
	m_ReserveStates.push_back(STATE_ENEMY_MUTATION_MECHANIC_DAMAGE_R);
	m_ReserveProbabilitys[STATE_ENEMY_MUTATION_MECHANIC_DAMAGE_R] = 1;
	m_ReserveStates.push_back(STATE_ENEMY_MUTATION_MECHANIC_DAMAGE_F);
	m_ReserveProbabilitys[STATE_ENEMY_MUTATION_MECHANIC_DAMAGE_F] = 1;

	m_ReserveStates.push_back(STATE_ENEMY_MUTATION_MECHANIC_IDLE_BATTLE);
	m_ReserveProbabilitys[STATE_ENEMY_MUTATION_MECHANIC_IDLE_BATTLE] = 1;

	m_ReserveStates.push_back(STATE_ENEMY_MUTATION_MECHANIC_WALK);
	m_ReserveProbabilitys[STATE_ENEMY_MUTATION_MECHANIC_WALK] = 1;

	m_ReserveStates.push_back(STATE_ENEMY_MUTATION_MECHANIC_RUN_RETURN);
	m_ReserveProbabilitys[STATE_ENEMY_MUTATION_MECHANIC_RUN_RETURN] = 1;
#endif
	return S_OK;

}

HRESULT CState_Enemy_Mutation_Mechanic_Idle_Normal::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Enemy_Mutation_Mechanic_Idle_Normal::Enter(void* pArg)
{

}

void CState_Enemy_Mutation_Mechanic_Idle_Normal::Tick(_float fTimeDelta)
{


	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_Mutation_Mechanic_Idle_Normal::Exit(void* pArg)
{

}

_bool CState_Enemy_Mutation_Mechanic_Idle_Normal::Check_Condition(CState_Handler* pHandler)
{

	return true;
}


CState_Enemy_Mutation_Mechanic_Idle_Normal* CState_Enemy_Mutation_Mechanic_Idle_Normal::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_Mutation_Mechanic_Idle_Normal* pInstance = new CState_Enemy_Mutation_Mechanic_Idle_Normal(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_Mutation_Mechanic_Idle_Normal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_Mutation_Mechanic_Idle_Normal::Clone(void* pArg)
{
	CState_Enemy_Mutation_Mechanic_Idle_Normal* pInstance = new CState_Enemy_Mutation_Mechanic_Idle_Normal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_Mutation_Mechanic_Idle_Normal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_Mutation_Mechanic_Idle_Normal::Free()
{
	__super::Free();
}
