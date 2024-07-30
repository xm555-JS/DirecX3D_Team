#include "stdafx.h"
#include "..\Public\State_Rudolf_Shield.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Rudolf_Shield::CState_Rudolf_Shield(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Rudolf_Shield::CState_Rudolf_Shield(const CState_Rudolf_Shield& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Rudolf_Shield::Initialize_Prototype()
{
	// 다음으로 넘어갈 수 있는 state
	//m_ReserveStates.push_back(STATE_BOSS_NEMESIS_IDLE);

	// 다음 state로 넘어갈 확률
	//m_ReserveProbabilitys[STATE_BOSS_NEMESIS_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Rudolf_Shield::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Rudolf_Shield::Enter(void* pArg)
{
	__super::Enter(pArg);
}

void CState_Rudolf_Shield::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	CState_AI::Tick(fTimeDelta);
}

void CState_Rudolf_Shield::Exit(void* pArg)
{
}

_bool CState_Rudolf_Shield::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Rudolf_Shield* CState_Rudolf_Shield::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Rudolf_Shield* pInstance = new CState_Rudolf_Shield(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Rudolf_Shield");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Rudolf_Shield::Clone(void* pArg)
{
	CState_Rudolf_Shield* pInstance = new CState_Rudolf_Shield(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Rudolf_Shield");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Rudolf_Shield::Free()
{
	__super::Free();
}
