#include "stdafx.h"
#include "..\Public\State_Enemy_HunterDog_Idle_Normal.h"
#include "GameInstance.h"
#include "GameObject.h"

CState_Enemy_HunterDog_Idle_Normal::CState_Enemy_HunterDog_Idle_Normal(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_HunterDog_Idle_Normal::CState_Enemy_HunterDog_Idle_Normal(const CState_Enemy_HunterDog_Idle_Normal& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_HunterDog_Idle_Normal::Initialize_Prototype()//적발견 x시 갈수 있는 상태 
{
#ifndef _MAPTOOL
	m_ReserveStates.push_back(STATE_ENEMY_HUNTERDOG_IDLE_BATTLE);

	m_ReserveStates.push_back(STATE_ENEMY_HUNTERDOG_WALK);

	m_ReserveStates.push_back(STATE_ENEMY_HUNTERDOG_RUN_RETURN);

	m_ReserveStates.push_back(STATE_ENEMY_HUNTERDOG_IDLE_ACT_A);


	m_ReserveProbabilitys[STATE_ENEMY_HUNTERDOG_IDLE_BATTLE] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_HUNTERDOG_WALK] = 3;

	m_ReserveProbabilitys[STATE_ENEMY_HUNTERDOG_RUN_RETURN] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_HUNTERDOG_IDLE_ACT_A] = 1;
#endif
	return S_OK;
}

HRESULT CState_Enemy_HunterDog_Idle_Normal::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Enemy_HunterDog_Idle_Normal::Enter(void* pArg)
{

}

void CState_Enemy_HunterDog_Idle_Normal::Tick(_float fTimeDelta)
{


	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_HunterDog_Idle_Normal::Exit(void* pArg)
{

}

_bool CState_Enemy_HunterDog_Idle_Normal::Check_Condition(CState_Handler* pHandler)
{


	return true;
}


CState_Enemy_HunterDog_Idle_Normal* CState_Enemy_HunterDog_Idle_Normal::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_HunterDog_Idle_Normal* pInstance = new CState_Enemy_HunterDog_Idle_Normal(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_HunterDog_Idle_Normal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_HunterDog_Idle_Normal::Clone(void* pArg)
{
	CState_Enemy_HunterDog_Idle_Normal* pInstance = new CState_Enemy_HunterDog_Idle_Normal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_HunterDog_Idle_Normal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_HunterDog_Idle_Normal::Free()
{
	__super::Free();
}
