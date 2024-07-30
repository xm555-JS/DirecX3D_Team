#include "stdafx.h"
#include "..\Public\State_Robella_1Phase_Down_Loop.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Robella_1Phase_Down_Loop::CState_Robella_1Phase_Down_Loop(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Robella_1Phase_Down_Loop::CState_Robella_1Phase_Down_Loop(const CState_Robella_1Phase_Down_Loop& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Robella_1Phase_Down_Loop::Initialize_Prototype()
{
	// 다음으로 넘어갈 수 있는 state      
	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_1PHASE_DOWN_UP);
	//m_ReserveStates.push_back(STATE_BOSS_ROBELLA_2PHASE);

	m_ReserveProbabilitys[STATE_BOSS_ROBELLA_1PHASE_DOWN_UP] = 1;

	return S_OK;
}

HRESULT CState_Robella_1Phase_Down_Loop::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Robella_1Phase_Down_Loop::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fAccTime = 0.f;
}

void CState_Robella_1Phase_Down_Loop::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_fAccTime += fTimeDelta;

	if (m_fAccTime >= 3.f)
	{
		m_bEndCurState = true;
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Robella_1Phase_Down_Loop::Exit(void* pArg)
{
}

_bool CState_Robella_1Phase_Down_Loop::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Robella_1Phase_Down_Loop* CState_Robella_1Phase_Down_Loop::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Robella_1Phase_Down_Loop* pInstance = new CState_Robella_1Phase_Down_Loop(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Robella_1Phase_Down_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Robella_1Phase_Down_Loop::Clone(void* pArg)
{
	CState_Robella_1Phase_Down_Loop* pInstance = new CState_Robella_1Phase_Down_Loop(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Robella_1Phase_Down_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Robella_1Phase_Down_Loop::Free()
{
	__super::Free();
}