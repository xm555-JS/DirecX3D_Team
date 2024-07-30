#include "stdafx.h"
#include "..\Public\State_Enemy_Mutation_Dead_3.h"
#include "GameInstance.h"
#include "Enemy.h"
CState_Enemy_Mutation_Dead_3::CState_Enemy_Mutation_Dead_3(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_Mutation_Dead_3::CState_Enemy_Mutation_Dead_3(const CState_Enemy_Mutation_Dead_3& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_Mutation_Dead_3::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_ENEMY_MUTATION_DEAD_END);

	m_ReserveProbabilitys[STATE_ENEMY_MUTATION_DEAD_END] = 1;






	return S_OK;
}

HRESULT CState_Enemy_Mutation_Dead_3::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Enemy_Mutation_Dead_3::Enter(void* pArg)
{
	m_fAnimSpeed = 0.5f;
	m_fSpeed = 0.3f;

}

void CState_Enemy_Mutation_Dead_3::Tick(_float fTimeDelta)
{

	if (30<Get_CurrentKeyFrame(m_pState_Handler,m_pTargetModel))
		m_bEndCurState = true;

	CNavigation* pNavigation = (CNavigation*)m_pState_Handler->Get_Owner()->Get_Component(NAVIGATION);
	m_pState_Handler->Get_Owner()->Get_Transform()->Go_Back(fTimeDelta,m_fSpeed, pNavigation);

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_Mutation_Dead_3::Exit(void* pArg)
{

}

_bool CState_Enemy_Mutation_Dead_3::Check_Condition(CState_Handler* pHandler)
{
	if (Is_OwnerDead(pHandler))
		return true;



	return false;
}


CState_Enemy_Mutation_Dead_3* CState_Enemy_Mutation_Dead_3::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_Mutation_Dead_3* pInstance = new CState_Enemy_Mutation_Dead_3(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_Mutation_Dead_3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_Mutation_Dead_3::Clone(void* pArg)
{
	CState_Enemy_Mutation_Dead_3* pInstance = new CState_Enemy_Mutation_Dead_3(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_Mutation_Dead_3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_Mutation_Dead_3::Free()
{
	__super::Free();
}
