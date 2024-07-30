#include "stdafx.h"
#include "..\Public\State_Weaver_Attack_06_Break.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Weaver_Attack_06_Break::CState_Weaver_Attack_06_Break(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Weaver_Attack_06_Break::CState_Weaver_Attack_06_Break(const CState_Weaver_Attack_06_Break& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Weaver_Attack_06_Break::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_WEAVER_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_WEAVER_WEAK_LOOP);

	m_ReserveProbabilitys[STATE_BOSS_WEAVER_WEAK_LOOP] = 1;

	return S_OK;
}

HRESULT CState_Weaver_Attack_06_Break::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Weaver_Attack_06_Break::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAccTime = 0.f;
}

void CState_Weaver_Attack_06_Break::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;


	CState_AI::Tick(fTimeDelta);
}

void CState_Weaver_Attack_06_Break::Exit(void* pArg)
{
}

_bool CState_Weaver_Attack_06_Break::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	//일정딜 이상일때

	return false;
}

CState_Weaver_Attack_06_Break* CState_Weaver_Attack_06_Break::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Weaver_Attack_06_Break* pInstance = new CState_Weaver_Attack_06_Break(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Weaver_Attack_06_Break");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Weaver_Attack_06_Break::Clone(void* pArg)
{
	CState_Weaver_Attack_06_Break* pInstance = new CState_Weaver_Attack_06_Break(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Weaver_Attack_06_Break");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Weaver_Attack_06_Break::Free()
{
	__super::Free();
}
