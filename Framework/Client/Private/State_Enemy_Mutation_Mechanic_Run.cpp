#include "stdafx.h"
#include "..\Public\State_Enemy_Mutation_Mechanic_Run.h"
#include "GameInstance.h"
#include "Player.h"
#include "Enemy.h"
CState_Enemy_Mutation_Mechanic_Run::CState_Enemy_Mutation_Mechanic_Run(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_Mutation_Mechanic_Run::CState_Enemy_Mutation_Mechanic_Run(const CState_Enemy_Mutation_Mechanic_Run& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_Mutation_Mechanic_Run::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_ENEMY_MUTATION_MECHANIC_DEAD_1);

	m_ReserveStates.push_back(STATE_ENEMY_MUTATION_MECHANIC_DAMAGE_B);
	m_ReserveProbabilitys[STATE_ENEMY_MUTATION_MECHANIC_DAMAGE_B] = 1;
	m_ReserveStates.push_back(STATE_ENEMY_MUTATION_MECHANIC_DAMAGE_L);
	m_ReserveProbabilitys[STATE_ENEMY_MUTATION_MECHANIC_DAMAGE_L] = 1;
	m_ReserveStates.push_back(STATE_ENEMY_MUTATION_MECHANIC_DAMAGE_R);
	m_ReserveProbabilitys[STATE_ENEMY_MUTATION_MECHANIC_DAMAGE_R] = 1;
	m_ReserveStates.push_back(STATE_ENEMY_MUTATION_MECHANIC_DAMAGE_F);
	m_ReserveProbabilitys[STATE_ENEMY_MUTATION_MECHANIC_DAMAGE_F] = 1;

	m_ReserveStates.push_back(STATE_ENEMY_MUTATION_MECHANIC_IDLE_NORMAL);
	m_ReserveProbabilitys[STATE_ENEMY_MUTATION_MECHANIC_IDLE_NORMAL] = 1;

	return S_OK;
}

HRESULT CState_Enemy_Mutation_Mechanic_Run::Initialize(void* pArg)
{

	return S_OK;
}

void CState_Enemy_Mutation_Mechanic_Run::Enter(void* pArg)
{

}

void CState_Enemy_Mutation_Mechanic_Run::Tick(_float fTimeDelta)
{

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_Mutation_Mechanic_Run::Exit(void* pArg)
{

}

_bool CState_Enemy_Mutation_Mechanic_Run::Check_Condition(CState_Handler* pHandler)
{
	return false;
}


CState_Enemy_Mutation_Mechanic_Run* CState_Enemy_Mutation_Mechanic_Run::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_Mutation_Mechanic_Run* pInstance = new CState_Enemy_Mutation_Mechanic_Run(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_Mutation_Mechanic_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_Mutation_Mechanic_Run::Clone(void* pArg)
{
	CState_Enemy_Mutation_Mechanic_Run* pInstance = new CState_Enemy_Mutation_Mechanic_Run(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_Mutation_Mechanic_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_Mutation_Mechanic_Run::Free()
{
	__super::Free();
}
