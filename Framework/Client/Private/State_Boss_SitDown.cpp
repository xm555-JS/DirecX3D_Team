#include "stdafx.h"
#include "..\Public\State_Boss_SitDown.h"
#include "GameInstance.h"

CState_Boss_SitDown::CState_Boss_SitDown(CState_Manager* pManager, STATE_TYPE eState)
	:CState_AI(pManager, eState)
{
}

CState_Boss_SitDown::CState_Boss_SitDown(const CState_Boss_SitDown& rhs)
	: CState_AI(rhs)
{
}

HRESULT CState_Boss_SitDown::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_AWAKE);

	return S_OK;
}

HRESULT CState_Boss_SitDown::Initialize(void* pArg)
{

	return S_OK;
}

void CState_Boss_SitDown::Enter(void* pArg)
{
	m_bEndCurState = true;
}

void CState_Boss_SitDown::Tick(_float fTimeDelta)
{

	CState_AI::Tick(fTimeDelta);
}

void CState_Boss_SitDown::Exit(void* pArg)
{
}

_bool CState_Boss_SitDown::Check_Condition(CState_Handler* pHandler)
{
	return true;
}


CState_Boss_SitDown* CState_Boss_SitDown::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Boss_SitDown* pInstance = new CState_Boss_SitDown(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Boss_SitDown");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Boss_SitDown::Clone(void* pArg)
{
	CState_Boss_SitDown* pInstance = new CState_Boss_SitDown(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Boss_SitDown");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Boss_SitDown::Free()
{
}
