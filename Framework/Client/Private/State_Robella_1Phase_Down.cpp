#include "stdafx.h"
#include "..\Public\State_Robella_1Phase_Down.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Robella_1Phase_Down::CState_Robella_1Phase_Down(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Robella_1Phase_Down::CState_Robella_1Phase_Down(const CState_Robella_1Phase_Down& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Robella_1Phase_Down::Initialize_Prototype()
{
	// 다음으로 넘어갈 수 있는 state      
	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_1PHASE_DOWN_LOOP);
	//m_ReserveStates.push_back(STATE_BOSS_ROBELLA_2PHASE);

	m_ReserveProbabilitys[STATE_BOSS_ROBELLA_1PHASE_DOWN_LOOP] = 1;

	return S_OK;
}

HRESULT CState_Robella_1Phase_Down::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Robella_1Phase_Down::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fAccTime = 0.f;
}

void CState_Robella_1Phase_Down::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_fAccTime += fTimeDelta;

	if (m_fAccTime >= 3.f)
	{
		m_bEndCurState = true;
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Robella_1Phase_Down::Exit(void* pArg)
{
}

_bool CState_Robella_1Phase_Down::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return false;
}

CState_Robella_1Phase_Down* CState_Robella_1Phase_Down::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Robella_1Phase_Down* pInstance = new CState_Robella_1Phase_Down(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Robella_1Phase_Down");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Robella_1Phase_Down::Clone(void* pArg)
{
	CState_Robella_1Phase_Down* pInstance = new CState_Robella_1Phase_Down(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Robella_1Phase_Down");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Robella_1Phase_Down::Free()
{
	__super::Free();
}