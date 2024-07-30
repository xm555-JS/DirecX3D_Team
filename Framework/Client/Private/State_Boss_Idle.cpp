#include "stdafx.h"
#include "..\Public\State_Boss_Idle.h"
#include "GameInstance.h"

CState_Boss_Idle::CState_Boss_Idle(CState_Manager* pManager, STATE_TYPE eState)
	:CState_AI(pManager, eState)
{
}

CState_Boss_Idle::CState_Boss_Idle(const CState_Boss_Idle& rhs)
	: CState_AI(rhs)
{
}

HRESULT CState_Boss_Idle::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_WALK);
	m_ReserveStates.push_back(STATE_BOSS_ATTACK);
	// 울트라 공격
	// 쉴드

	return S_OK;
}

HRESULT CState_Boss_Idle::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Boss_Idle::Enter(void* pArg)
{
	m_bEndCurState = true;
}

void CState_Boss_Idle::Tick(_float fTimeDelta)
{
	CState_AI::Tick(fTimeDelta);
}

void CState_Boss_Idle::Exit(void* pArg)
{
}

_bool CState_Boss_Idle::Check_Condition(CState_Handler* pHandler)
{
	return true;
}


CState_Boss_Idle* CState_Boss_Idle::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Boss_Idle* pInstance = new CState_Boss_Idle(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Boss_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Boss_Idle::Clone(void* pArg)
{
	CState_Boss_Idle* pInstance = new CState_Boss_Idle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Boss_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Boss_Idle::Free()
{
}
