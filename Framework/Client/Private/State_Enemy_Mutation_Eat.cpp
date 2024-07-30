#include "stdafx.h"
#include "..\Public\State_Enemy_Mutation_Eat.h"
#include "GameInstance.h"
#include "Enemy.h"
CState_Enemy_Mutation_Eat::CState_Enemy_Mutation_Eat(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_Mutation_Eat::CState_Enemy_Mutation_Eat(const CState_Enemy_Mutation_Eat& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_Mutation_Eat::Initialize_Prototype()
{

	m_ReserveStates.push_back(STATE_ENEMY_MUTATION_EAT_FIND);
	m_ReserveProbabilitys[STATE_ENEMY_MUTATION_EAT_FIND] = 1;

	return S_OK;
}

HRESULT CState_Enemy_Mutation_Eat::Initialize(void* pArg)
{

	return S_OK;
}

void CState_Enemy_Mutation_Eat::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 1.f;
}

void CState_Enemy_Mutation_Eat::Tick(_float fTimeDelta)
{

	CEnemy* pOwner = (CEnemy*)m_pState_Handler->Get_Owner();

	if (nullptr != pOwner->Get_Target())
	{
		if (Is_AnimFinished())
			m_bEndCurState = true;
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_Mutation_Eat::Exit(void* pArg)
{

}

_bool CState_Enemy_Mutation_Eat::Check_Condition(CState_Handler* pHandler)
{
	return true;
}


CState_Enemy_Mutation_Eat* CState_Enemy_Mutation_Eat::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_Mutation_Eat* pInstance = new CState_Enemy_Mutation_Eat(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_Mutation_Eat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_Mutation_Eat::Clone(void* pArg)
{
	CState_Enemy_Mutation_Eat* pInstance = new CState_Enemy_Mutation_Eat(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_Mutation_Eat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_Mutation_Eat::Free()
{
	__super::Free();
}
