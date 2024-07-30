#include "stdafx.h"
#include "..\Public\State_Enemy_Gangster_Cap_Sittingdown.h"
#include "GameInstance.h"
#include "Player.h"
#include "Enemy.h"
CState_Enemy_Gangster_Cap_Sittingdown::CState_Enemy_Gangster_Cap_Sittingdown(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_Gangster_Cap_Sittingdown::CState_Enemy_Gangster_Cap_Sittingdown(const CState_Enemy_Gangster_Cap_Sittingdown& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_Gangster_Cap_Sittingdown::Initialize_Prototype()
{

	m_ReserveStates.push_back(STATE_ENEMY_GANGSTER_CAP_IDLE);
	m_ReserveStates.push_back(STATE_ENEMY_GANGSTER_CAP_WALK);

	m_ReserveStates.push_back(STATE_ENEMY_GANGSTER_CAP_RUN_RETURN);

	m_ReserveProbabilitys[STATE_ENEMY_GANGSTER_CAP_IDLE] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_GANGSTER_CAP_WALK] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_GANGSTER_CAP_RUN] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_GANGSTER_CAP_RUN_RETURN] = 1;

	return S_OK;
}

HRESULT CState_Enemy_Gangster_Cap_Sittingdown::Initialize(void * pArg)
{

	return S_OK;
}

void CState_Enemy_Gangster_Cap_Sittingdown::Enter(void* pArg)
{

}

void CState_Enemy_Gangster_Cap_Sittingdown::Tick(_float fTimeDelta)
{
	


	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_Gangster_Cap_Sittingdown::Exit(void* pArg)
{

}

_bool CState_Enemy_Gangster_Cap_Sittingdown::Check_Condition(CState_Handler* pHandler)
{

	return false;
}


CState_Enemy_Gangster_Cap_Sittingdown* CState_Enemy_Gangster_Cap_Sittingdown::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_Gangster_Cap_Sittingdown* pInstance = new CState_Enemy_Gangster_Cap_Sittingdown(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_Gangster_Cap_Sittingdown");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_Gangster_Cap_Sittingdown::Clone(void* pArg)
{
	CState_Enemy_Gangster_Cap_Sittingdown* pInstance = new CState_Enemy_Gangster_Cap_Sittingdown(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_Gangster_Cap_Sittingdown");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_Gangster_Cap_Sittingdown::Free()
{
	__super::Free();
}
