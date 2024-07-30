#include "stdafx.h"
#include "..\Public\State_Enemy_Habaka_Admiral_Idle_Normal.h"
#include "GameInstance.h"
#include "Enemy.h"

CState_Enemy_Habaka_Admiral_Idle_Normal::CState_Enemy_Habaka_Admiral_Idle_Normal(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_Habaka_Admiral_Idle_Normal::CState_Enemy_Habaka_Admiral_Idle_Normal(const CState_Enemy_Habaka_Admiral_Idle_Normal& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_Habaka_Admiral_Idle_Normal::Initialize_Prototype()
{
#ifndef _MAPTOOL
	//강제
	m_ForceStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DEAD_1);						//주금

	//예약
	m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_RUN);						//chase
	m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_RUN_RETURN);				//chase end

	//m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_B);					//damaged
	//m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_L);					//
	//m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_R);					//
	//m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_F);					//

	m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_IDLE_BATTLE);				//fighting

	m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_WALK);						//patrol
			
	m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_RUN] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_RUN_RETURN] = 1;

	//m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_B] = 1;
	//m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_L] = 1;
	//m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_R] = 1;
	//m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_F] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_IDLE_BATTLE] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_WALK] = 1;
#endif
	return S_OK;

}

HRESULT CState_Enemy_Habaka_Admiral_Idle_Normal::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Enemy_Habaka_Admiral_Idle_Normal::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fPlayTime = 1.5f;
	m_fAnimSpeed = 1.f;
}

void CState_Enemy_Habaka_Admiral_Idle_Normal::Tick(_float fTimeDelta)
{
	if (nullptr != ((CEnemy*)m_pOwner)->Get_Target())			//내 객체가 타겟을 가지고 있느냐
		m_bEndCurState = true;

	if (m_fPlayTime < m_pState_Handler->Get_TimeAcc())
		m_bEndCurState = true;

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_Habaka_Admiral_Idle_Normal::Exit(void* pArg)
{

}

_bool CState_Enemy_Habaka_Admiral_Idle_Normal::Check_Condition(CState_Handler* pHandler)
{
	CEnemy* pOwner = (CEnemy*)pHandler->Get_Owner();

	if (nullptr == pOwner->Get_Target())
		return true;

	return false;
}


CState_Enemy_Habaka_Admiral_Idle_Normal* CState_Enemy_Habaka_Admiral_Idle_Normal::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_Habaka_Admiral_Idle_Normal* pInstance = new CState_Enemy_Habaka_Admiral_Idle_Normal(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_Habaka_Admiral_Idle_Normal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_Habaka_Admiral_Idle_Normal::Clone(void* pArg)
{
	CState_Enemy_Habaka_Admiral_Idle_Normal* pInstance = new CState_Enemy_Habaka_Admiral_Idle_Normal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_Habaka_Admiral_Idle_Normal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_Habaka_Admiral_Idle_Normal::Free()
{
	__super::Free();
}
