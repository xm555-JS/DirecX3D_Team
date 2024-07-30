#include "stdafx.h"
#include "..\Public\State_Enemy_Mutation_Skill_.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"

CState_Enemy_Mutation_Skill_::CState_Enemy_Mutation_Skill_(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_Mutation_Skill_::CState_Enemy_Mutation_Skill_(const CState_Enemy_Mutation_Skill_& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_Mutation_Skill_::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_ENEMY_MUTATION_IDLE);
	m_ReserveStates.push_back(STATE_ENEMY_MUTATION_WALK);
	m_ReserveStates.push_back(STATE_ENEMY_MUTATION_RUN);

	return S_OK;
}

HRESULT CState_Enemy_Mutation_Skill_::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Enemy_Mutation_Skill_::Enter(void* pArg)
{
	m_fAnimSpeed = 0.5f;
}

void CState_Enemy_Mutation_Skill_::Tick(_float fTimeDelta)
{
	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}


	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_Mutation_Skill_::Exit(void* pArg)
{
}

_bool CState_Enemy_Mutation_Skill_::Check_Condition(CState_Handler* pHandler)
{

	return false;
}


CState_Enemy_Mutation_Skill_* CState_Enemy_Mutation_Skill_::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_Mutation_Skill_* pInstance = new CState_Enemy_Mutation_Skill_(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_Mutation_Skill_");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_Mutation_Skill_::Clone(void* pArg)
{
	CState_Enemy_Mutation_Skill_* pInstance = new CState_Enemy_Mutation_Skill_(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_Mutation_Skill_");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_Mutation_Skill_::Free()
{
	__super::Free();
}
