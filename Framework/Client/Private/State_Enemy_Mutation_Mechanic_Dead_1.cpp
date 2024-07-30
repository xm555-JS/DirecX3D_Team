#include "stdafx.h"
#include "..\Public\State_Enemy_Mutation_Mechanic_Dead_1.h"
#include "GameInstance.h"
#include "Player.h"
#include "Enemy.h"
CState_Enemy_Mutation_Mechanic_Dead_1::CState_Enemy_Mutation_Mechanic_Dead_1(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_Mutation_Mechanic_Dead_1::CState_Enemy_Mutation_Mechanic_Dead_1(const CState_Enemy_Mutation_Mechanic_Dead_1& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_Mutation_Mechanic_Dead_1::Initialize_Prototype()
{

	m_ReserveStates.push_back(STATE_ENEMY_MUTATION_MECHANIC_DEAD_END);
	m_ReserveProbabilitys[STATE_ENEMY_MUTATION_MECHANIC_DEAD_END] = 1;

	return S_OK;
}

HRESULT CState_Enemy_Mutation_Mechanic_Dead_1::Initialize(void* pArg)
{

	return S_OK;
}

void CState_Enemy_Mutation_Mechanic_Dead_1::Enter(void* pArg)
{


}

void CState_Enemy_Mutation_Mechanic_Dead_1::Tick(_float fTimeDelta)
{

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_Mutation_Mechanic_Dead_1::Exit(void* pArg)
{

}

_bool CState_Enemy_Mutation_Mechanic_Dead_1::Check_Condition(CState_Handler* pHandler)
{


	return Is_OwnerDead(pHandler);
}


CState_Enemy_Mutation_Mechanic_Dead_1* CState_Enemy_Mutation_Mechanic_Dead_1::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_Mutation_Mechanic_Dead_1* pInstance = new CState_Enemy_Mutation_Mechanic_Dead_1(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_Mutation_Mechanic_Dead_1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_Mutation_Mechanic_Dead_1::Clone(void* pArg)
{
	CState_Enemy_Mutation_Mechanic_Dead_1* pInstance = new CState_Enemy_Mutation_Mechanic_Dead_1(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_Mutation_Mechanic_Dead_1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_Mutation_Mechanic_Dead_1::Free()
{
	__super::Free();
}
