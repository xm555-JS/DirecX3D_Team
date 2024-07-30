#include "stdafx.h"
#include "..\Public\State_Angel_Idle.h"

#include "GameInstance.h"

#include "GameObject.h"
#include "Model.h"


CState_Angel_Idle::CState_Angel_Idle(CState_Manager* pManager, STATE_TYPE eState)
	:CState_AI(pManager, eState)
{
}

CState_Angel_Idle::CState_Angel_Idle(const CState_Angel_Idle& rhs)
	:CState_AI(rhs)
{
}

HRESULT CState_Angel_Idle::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_ANGEL_IDLE);

	m_ReserveStates.push_back(STATE_BOSS_ANGEL_BACK);
	m_ReserveStates.push_back(STATE_BOSS_ANGEL_TURNL);
	m_ReserveStates.push_back(STATE_BOSS_ANGEL_TURNR);

	m_ReserveStates.push_back(STATE_BOSS_ANGEL_RUN);
	m_ReserveStates.push_back(STATE_BOSS_ANGEL_RUNL);
	m_ReserveStates.push_back(STATE_BOSS_ANGEL_RUNR);

	//m_ReserveStates.push_back(STATE_BOSS_ANGEL_SWING_ATTACK);
	//m_ReserveStates.push_back(STATE_BOSS_ANGEL_LASER_ATTACK);
	//
	//m_ReserveStates.push_back(STATE_BOSS_ANGEL_LASER_ATTACKA);
	//
	//m_ReserveStates.push_back(STATE_BOSS_ANGEL_SKILL_ATTACKA);
	//
	//m_ReserveStates.push_back(STATE_BOSS_ANGEL_ULT_ATTACK);
	//
	//m_ReserveStates.push_back(STATE_BOSS_ANGEL_SHIELD);

	m_ReserveProbabilitys[STATE_BOSS_ANGEL_IDLE] = 1;

	m_ReserveProbabilitys[STATE_BOSS_ANGEL_BACK] = 1;
	m_ReserveProbabilitys[STATE_BOSS_ANGEL_TURNL] = 1;
	m_ReserveProbabilitys[STATE_BOSS_ANGEL_TURNR] = 1;

	m_ReserveProbabilitys[STATE_BOSS_ANGEL_RUN] = 1;
	m_ReserveProbabilitys[STATE_BOSS_ANGEL_RUNL] = 1;
	m_ReserveProbabilitys[STATE_BOSS_ANGEL_RUNR] = 1;

	//m_ReserveProbabilitys[STATE_BOSS_ANGEL_SWING_ATTACK] = 1;
	//m_ReserveProbabilitys[STATE_BOSS_ANGEL_LASER_ATTACK] = 1;
	//
	//m_ReserveProbabilitys[STATE_BOSS_ANGEL_LASER_ATTACKA] = 1;
	//
	//m_ReserveProbabilitys[STATE_BOSS_ANGEL_SKILL_ATTACKA] = 1;
	//
	//m_ReserveProbabilitys[STATE_BOSS_ANGEL_ULT_ATTACK] = 1;
	//
	//m_ReserveProbabilitys[STATE_BOSS_ANGEL_SHIELD] = 1;

	return S_OK;
}

HRESULT CState_Angel_Idle::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Angel_Idle::Enter(void* pArg)
{
}

void CState_Angel_Idle::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	CState_AI::Tick(fTimeDelta);
}

void CState_Angel_Idle::Exit(void* pArg)
{
}

_bool CState_Angel_Idle::Check_Condition(CState_Handler* pHandler)
{
	return true;
}

CState_Angel_Idle* CState_Angel_Idle::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Angel_Idle* pInstance = new CState_Angel_Idle(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Angel_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Angel_Idle::Clone(void* pArg)
{
	CState_Angel_Idle* pInstance = new CState_Angel_Idle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Angel_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Angel_Idle::Free()
{
	__super::Free();
}
