#include "stdafx.h"
#include "..\Public\State_Weaver_Attack_11_Loop.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Weaver_Attack_11_Loop::CState_Weaver_Attack_11_Loop(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Weaver_Attack_11_Loop::CState_Weaver_Attack_11_Loop(const CState_Weaver_Attack_11_Loop& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Weaver_Attack_11_Loop::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_WEAVER_WEAK_BEGIN);
	m_ForceStates.push_back(STATE_BOSS_WEAVER_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_11_END);

	m_ReserveProbabilitys[STATE_BOSS_WEAVER_ATTACK_11_END] = 1;

	return S_OK;
}

HRESULT CState_Weaver_Attack_11_Loop::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Weaver_Attack_11_Loop::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAccTime = 0.f;
}

void CState_Weaver_Attack_11_Loop::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_fAccTime += fTimeDelta;

	if (m_fAccTime > 5.f)
		m_bEndCurState = true;

	CState_AI::Tick(fTimeDelta);
}

void CState_Weaver_Attack_11_Loop::Exit(void* pArg)
{
}

_bool CState_Weaver_Attack_11_Loop::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Weaver_Attack_11_Loop* CState_Weaver_Attack_11_Loop::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Weaver_Attack_11_Loop* pInstance = new CState_Weaver_Attack_11_Loop(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Weaver_Attack_11_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Weaver_Attack_11_Loop::Clone(void* pArg)
{
	CState_Weaver_Attack_11_Loop* pInstance = new CState_Weaver_Attack_11_Loop(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Weaver_Attack_11_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Weaver_Attack_11_Loop::Free()
{
	__super::Free();
}
