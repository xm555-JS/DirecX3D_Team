#include "stdafx.h"
#include "..\Public\State_Cooker_Cooking_Complete.h"
#include "GameInstance.h"

CState_Cooker_Cooking_Complete::CState_Cooker_Cooking_Complete(CState_Manager* pManager, STATE_TYPE eState)
	:CState_AI(pManager, eState)
{
}

CState_Cooker_Cooking_Complete::CState_Cooker_Cooking_Complete(const CState_Cooker_Cooking_Complete& rhs)
	: CState_AI(rhs)
{
}

HRESULT CState_Cooker_Cooking_Complete::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_NCP_COOKER_IDLE);

	return S_OK;
}

HRESULT CState_Cooker_Cooking_Complete::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Cooker_Cooking_Complete::Enter(void* pArg)
{
}

void CState_Cooker_Cooking_Complete::Tick(_float fTimeDelta)
{
	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	CState_AI::Tick(fTimeDelta);
}

void CState_Cooker_Cooking_Complete::Exit(void* pArg)
{
}

_bool CState_Cooker_Cooking_Complete::Check_Condition(CState_Handler* pHandler)
{
	return true;
}


CState_Cooker_Cooking_Complete* CState_Cooker_Cooking_Complete::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Cooker_Cooking_Complete* pInstance = new CState_Cooker_Cooking_Complete(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Cooker_Cooking_Complete");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Cooker_Cooking_Complete::Clone(void* pArg)
{
	CState_Cooker_Cooking_Complete* pInstance = new CState_Cooker_Cooking_Complete(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Cooker_Cooking_Complete");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Cooker_Cooking_Complete::Free()
{
}
