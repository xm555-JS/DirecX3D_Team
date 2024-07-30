#include "stdafx.h"
#include "..\Public\State_Enemy_Habaka_Admiral_Idle_Battle.h"
#include "GameInstance.h"
#include "Enemy.h"



CState_Enemy_Habaka_Admiral_Idle_Battle::CState_Enemy_Habaka_Admiral_Idle_Battle(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_Habaka_Admiral_Idle_Battle::CState_Enemy_Habaka_Admiral_Idle_Battle(const CState_Enemy_Habaka_Admiral_Idle_Battle& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_Habaka_Admiral_Idle_Battle::Initialize_Prototype()//주변에 플레이어가 없을때 갈수있는상태
{

	m_ForceStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DEAD_1);
	m_ForceStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_TURN_L);
	m_ForceStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_TURN_R);
	m_ForceStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_WALK_B);

	m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_RUN);
	m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_RUN_RETURN);				//chase end

	//m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_B);
	//m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_L);
	//m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_R);
	//m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_F);

	m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_IDLE_NORMAL);	

	m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_WALK_L);
	m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_WALK_R);

	m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_ATTACK_KICK_A);
	m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_ATTACK_FRONTJUMP);

	m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_RUN] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_RUN_RETURN] = 1;

	//m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_B] = 1;
	//m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_L] = 1;
	//m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_R] = 1;
	//m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_F] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_IDLE_NORMAL] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_WALK_L] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_WALK_R] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_ATTACK_KICK_A] = 2;
	m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_ATTACK_FRONTJUMP] = 2;

	return S_OK;
}

HRESULT CState_Enemy_Habaka_Admiral_Idle_Battle::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Enemy_Habaka_Admiral_Idle_Battle::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fPlayTime = 2.f;
	m_fAnimSpeed = 1.f;
}

void CState_Enemy_Habaka_Admiral_Idle_Battle::Tick(_float fTimeDelta)
{
	if (nullptr == ((CEnemy*)m_pOwner)->Get_Target())
		m_bEndCurState = true;

	if (m_fPlayTime < m_pState_Handler->Get_TimeAcc())
		m_bEndCurState = true;

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_Habaka_Admiral_Idle_Battle::Exit(void* pArg)
{
}

_bool CState_Enemy_Habaka_Admiral_Idle_Battle::Check_Condition(CState_Handler* pHandler)
{
	CEnemy* pOwner = (CEnemy*)pHandler->Get_Owner();
	
	if (nullptr != pOwner->Get_Target())
		return true;
	
	return false;
}


CState_Enemy_Habaka_Admiral_Idle_Battle* CState_Enemy_Habaka_Admiral_Idle_Battle::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_Habaka_Admiral_Idle_Battle* pInstance = new CState_Enemy_Habaka_Admiral_Idle_Battle(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_Habaka_Admiral_Idle_Battle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_Habaka_Admiral_Idle_Battle::Clone(void* pArg)
{
	CState_Enemy_Habaka_Admiral_Idle_Battle* pInstance = new CState_Enemy_Habaka_Admiral_Idle_Battle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_Habaka_Admiral_Idle_Battle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_Habaka_Admiral_Idle_Battle::Free()
{
	__super::Free();
}
