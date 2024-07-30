#include "stdafx.h"
#include "..\Public\State_Cooker_Idle.h"
#include "GameInstance.h"

CState_Cooker_Idle::CState_Cooker_Idle(CState_Manager* pManager, STATE_TYPE eState)
	:CState_AI(pManager, eState)
{
}

CState_Cooker_Idle::CState_Cooker_Idle(const CState_Cooker_Idle& rhs)
	: CState_AI(rhs)
{
}

HRESULT CState_Cooker_Idle::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_NCP_COOKER_COOKING);

	return S_OK;
}

HRESULT CState_Cooker_Idle::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Cooker_Idle::Enter(void* pArg)
{
	m_bEndCurState = true;
}

void CState_Cooker_Idle::Tick(_float fTimeDelta)
{
	CState_AI::Tick(fTimeDelta);
}

void CState_Cooker_Idle::Exit(void* pArg)
{
}

_bool CState_Cooker_Idle::Check_Condition(CState_Handler* pHandler)
{
	return true;
}


CState_Cooker_Idle* CState_Cooker_Idle::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Cooker_Idle* pInstance = new CState_Cooker_Idle(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Cooker_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Cooker_Idle::Clone(void* pArg)
{
	CState_Cooker_Idle* pInstance = new CState_Cooker_Idle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Cooker_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Cooker_Idle::Free()
{
}
