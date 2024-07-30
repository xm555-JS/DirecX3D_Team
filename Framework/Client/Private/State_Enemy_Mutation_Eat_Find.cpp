#include "stdafx.h"
#include "..\Public\State_Enemy_Mutation_Eat_Find.h"
#include "GameInstance.h"
CState_Enemy_Mutation_Eat_Find::CState_Enemy_Mutation_Eat_Find(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_Mutation_Eat_Find::CState_Enemy_Mutation_Eat_Find(const CState_Enemy_Mutation_Eat_Find& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_Mutation_Eat_Find::Initialize_Prototype()
{

	m_ReserveStates.push_back(STATE_ENEMY_MUTATION_IDLE_BATTLE);
	m_ReserveStates.push_back(STATE_ENEMY_MUTATION_IDLE_NORMAL);
	m_ReserveProbabilitys[STATE_ENEMY_MUTATION_IDLE_BATTLE] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MUTATION_IDLE_NORMAL] = 1;

	return S_OK;
}

HRESULT CState_Enemy_Mutation_Eat_Find::Initialize(void* pArg)
{

	return S_OK;
}

void CState_Enemy_Mutation_Eat_Find::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 1.f;
}

void CState_Enemy_Mutation_Eat_Find::Tick(_float fTimeDelta)
{

	if (Is_AnimFinished())
		m_bEndCurState = true;

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_Mutation_Eat_Find::Exit(void* pArg)
{

}

_bool CState_Enemy_Mutation_Eat_Find::Check_Condition(CState_Handler* pHandler)
{
	return true;
}


CState_Enemy_Mutation_Eat_Find* CState_Enemy_Mutation_Eat_Find::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_Mutation_Eat_Find* pInstance = new CState_Enemy_Mutation_Eat_Find(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_Mutation_Eat_Find");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_Mutation_Eat_Find::Clone(void* pArg)
{
	CState_Enemy_Mutation_Eat_Find* pInstance = new CState_Enemy_Mutation_Eat_Find(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_Mutation_Eat_Find");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_Mutation_Eat_Find::Free()
{
	__super::Free();
}
