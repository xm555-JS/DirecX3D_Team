#include "stdafx.h"
#include "..\Public\State_Robella_2Phase_Down_Up.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Robella_2Phase_Down_Up::CState_Robella_2Phase_Down_Up(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Robella_2Phase_Down_Up::CState_Robella_2Phase_Down_Up(const CState_Robella_2Phase_Down_Up& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Robella_2Phase_Down_Up::Initialize_Prototype()
{
	// 다음으로 넘어갈 수 있는 state      
	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_IDLE2);

	m_ReserveProbabilitys[STATE_BOSS_ROBELLA_IDLE2] = 1;

	return S_OK;
}

HRESULT CState_Robella_2Phase_Down_Up::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Robella_2Phase_Down_Up::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fAccTime = 0.f;
}

void CState_Robella_2Phase_Down_Up::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_fAccTime += fTimeDelta;

	if (m_fAccTime >= 3.f)
	{
		m_bEndCurState = true;
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Robella_2Phase_Down_Up::Exit(void* pArg)
{
}

_bool CState_Robella_2Phase_Down_Up::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Robella_2Phase_Down_Up* CState_Robella_2Phase_Down_Up::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Robella_2Phase_Down_Up* pInstance = new CState_Robella_2Phase_Down_Up(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Robella_2Phase_Down_Up");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Robella_2Phase_Down_Up::Clone(void* pArg)
{
	CState_Robella_2Phase_Down_Up* pInstance = new CState_Robella_2Phase_Down_Up(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Robella_2Phase_Down_Up");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Robella_2Phase_Down_Up::Free()
{
	__super::Free();
}