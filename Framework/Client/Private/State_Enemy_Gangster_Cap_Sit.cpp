#include "stdafx.h"
#include "..\Public\State_Enemy_Gangster_Cap_Sit.h"
#include "GameInstance.h"
#include "Player.h"
#include "Enemy.h"
CState_Enemy_Gangster_Cap_Sit::CState_Enemy_Gangster_Cap_Sit(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_Gangster_Cap_Sit::CState_Enemy_Gangster_Cap_Sit(const CState_Enemy_Gangster_Cap_Sit& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_Gangster_Cap_Sit::Initialize_Prototype()
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

HRESULT CState_Enemy_Gangster_Cap_Sit::Initialize(void * pArg)
{

	return S_OK;
}

void CState_Enemy_Gangster_Cap_Sit::Enter(void* pArg)
{
	m_fAnimSpeed = 0.5f;
}

void CState_Enemy_Gangster_Cap_Sit::Tick(_float fTimeDelta)
{
	


	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_Gangster_Cap_Sit::Exit(void* pArg)
{

}

_bool CState_Enemy_Gangster_Cap_Sit::Check_Condition(CState_Handler* pHandler)
{

	return false;
}


CState_Enemy_Gangster_Cap_Sit* CState_Enemy_Gangster_Cap_Sit::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_Gangster_Cap_Sit* pInstance = new CState_Enemy_Gangster_Cap_Sit(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_Gangster_Cap_Sit");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_Gangster_Cap_Sit::Clone(void* pArg)
{
	CState_Enemy_Gangster_Cap_Sit* pInstance = new CState_Enemy_Gangster_Cap_Sit(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_Gangster_Cap_Sit");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_Gangster_Cap_Sit::Free()
{
	__super::Free();
}
