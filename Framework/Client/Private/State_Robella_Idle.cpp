#include "stdafx.h"
#include "..\Public\State_Robella_Idle.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Robella_Idle::CState_Robella_Idle(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Robella_Idle::CState_Robella_Idle(const CState_Robella_Idle& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Robella_Idle::Initialize_Prototype()
{
	// 다음으로 넘어갈 수 있는 state      
	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_ATTACK_01);
	//m_ReserveStates.push_back(STATE_BOSS_ROBELLA_2PHASE);

	// 다음 state로 넘어갈 확률
	m_ReserveProbabilitys[STATE_BOSS_ROBELLA_ATTACK_01] = 1.f;
	//m_ReserveProbabilitys[STATE_BOSS_ROBELLA_2PHASE] = 1.f;

	return S_OK;
}

HRESULT CState_Robella_Idle::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Robella_Idle::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fAccTime = 0.f;
}

void CState_Robella_Idle::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_fAccTime += fTimeDelta;

	if (m_fAccTime >= 3.f)
	{
		m_bEndCurState = true;
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Robella_Idle::Exit(void* pArg)
{
}

_bool CState_Robella_Idle::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Robella_Idle* CState_Robella_Idle::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Robella_Idle* pInstance = new CState_Robella_Idle(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Robella_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Robella_Idle::Clone(void* pArg)
{
	CState_Robella_Idle* pInstance = new CState_Robella_Idle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Robella_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Robella_Idle::Free()
{
	__super::Free();
}