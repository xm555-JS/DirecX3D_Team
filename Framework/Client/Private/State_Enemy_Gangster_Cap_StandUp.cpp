#include "stdafx.h"
#include "..\Public\State_Enemy_Gangster_Cap_StandUp.h"
#include "GameInstance.h"
#include "Player.h"
#include "Enemy.h"
CState_Enemy_Gangster_Cap_StandUp::CState_Enemy_Gangster_Cap_StandUp(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_Gangster_Cap_StandUp::CState_Enemy_Gangster_Cap_StandUp(const CState_Enemy_Gangster_Cap_StandUp& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_Gangster_Cap_StandUp::Initialize_Prototype()
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

HRESULT CState_Enemy_Gangster_Cap_StandUp::Initialize(void * pArg)
{

	return S_OK;
}

void CState_Enemy_Gangster_Cap_StandUp::Enter(void* pArg)
{

}

void CState_Enemy_Gangster_Cap_StandUp::Tick(_float fTimeDelta)
{
	


	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_Gangster_Cap_StandUp::Exit(void* pArg)
{

}

_bool CState_Enemy_Gangster_Cap_StandUp::Check_Condition(CState_Handler* pHandler)
{

	return false;
}


CState_Enemy_Gangster_Cap_StandUp* CState_Enemy_Gangster_Cap_StandUp::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_Gangster_Cap_StandUp* pInstance = new CState_Enemy_Gangster_Cap_StandUp(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_Gangster_Cap_StandUp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_Gangster_Cap_StandUp::Clone(void* pArg)
{
	CState_Enemy_Gangster_Cap_StandUp* pInstance = new CState_Enemy_Gangster_Cap_StandUp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_Gangster_Cap_StandUp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_Gangster_Cap_StandUp::Free()
{
	__super::Free();
}
