#include "stdafx.h"
#include "..\Public\State_Enemy_Mutation_Dead_2.h"
#include "GameInstance.h"
#include "Enemy.h"
CState_Enemy_Mutation_Dead_2::CState_Enemy_Mutation_Dead_2(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_Mutation_Dead_2::CState_Enemy_Mutation_Dead_2(const CState_Enemy_Mutation_Dead_2& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_Mutation_Dead_2::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_ENEMY_MUTATION_DEAD_3);

	m_ReserveProbabilitys[STATE_ENEMY_MUTATION_DEAD_3] = 1;





	return S_OK;
}

HRESULT CState_Enemy_Mutation_Dead_2::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Enemy_Mutation_Dead_2::Enter(void* pArg)
{
	m_fAnimSpeed = 0.5f;

}

void CState_Enemy_Mutation_Dead_2::Tick(_float fTimeDelta)
{

	if (Is_AnimFinished(m_pState_Handler,m_pTargetModel))
		m_bEndCurState = true;

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_Mutation_Dead_2::Exit(void* pArg)
{

}

_bool CState_Enemy_Mutation_Dead_2::Check_Condition(CState_Handler* pHandler)
{
	if (Is_OwnerDead(pHandler))
		return true;


	return false;
}


CState_Enemy_Mutation_Dead_2* CState_Enemy_Mutation_Dead_2::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_Mutation_Dead_2* pInstance = new CState_Enemy_Mutation_Dead_2(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_Mutation_Dead_2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_Mutation_Dead_2::Clone(void* pArg)
{
	CState_Enemy_Mutation_Dead_2* pInstance = new CState_Enemy_Mutation_Dead_2(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_Mutation_Dead_2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_Mutation_Dead_2::Free()
{
	__super::Free();
}
