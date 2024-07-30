#include "stdafx.h"
#include "..\Public\State_Cooker_Cooking.h"
#include "GameInstance.h"

CState_Cooker_Cooking::CState_Cooker_Cooking(CState_Manager* pManager, STATE_TYPE eState)
	:CState_AI(pManager, eState)
{
}

CState_Cooker_Cooking::CState_Cooker_Cooking(const CState_Cooker_Cooking& rhs)
	: CState_AI(rhs)
{
}

HRESULT CState_Cooker_Cooking::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_NCP_COOKER_COOKING_COMPLETE);

	return S_OK;
}

HRESULT CState_Cooker_Cooking::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Cooker_Cooking::Enter(void* pArg)
{
}

void CState_Cooker_Cooking::Tick(_float fTimeDelta)
{
	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	CState_AI::Tick(fTimeDelta);
}

void CState_Cooker_Cooking::Exit(void* pArg)
{
}

_bool CState_Cooker_Cooking::Check_Condition(CState_Handler* pHandler)
{
	return true;
}


CState_Cooker_Cooking* CState_Cooker_Cooking::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Cooker_Cooking* pInstance = new CState_Cooker_Cooking(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Cooker_Cooking");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Cooker_Cooking::Clone(void* pArg)
{
	CState_Cooker_Cooking* pInstance = new CState_Cooker_Cooking(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Cooker_Cooking");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Cooker_Cooking::Free()
{
}
