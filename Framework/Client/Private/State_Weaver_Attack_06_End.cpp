#include "stdafx.h"
#include "..\Public\State_Weaver_Attack_06_End.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Weaver_Attack_06_End::CState_Weaver_Attack_06_End(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Weaver_Attack_06_End::CState_Weaver_Attack_06_End(const CState_Weaver_Attack_06_End& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Weaver_Attack_06_End::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_WEAVER_ATTACK_06_BREAK);
	m_ForceStates.push_back(STATE_BOSS_WEAVER_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_WEAVER_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_WEAVER_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Weaver_Attack_06_End::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Weaver_Attack_06_End::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAccTime = 0.f;
}

void CState_Weaver_Attack_06_End::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;


	CState_AI::Tick(fTimeDelta);
}

void CState_Weaver_Attack_06_End::Exit(void* pArg)
{
}

_bool CState_Weaver_Attack_06_End::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Weaver_Attack_06_End* CState_Weaver_Attack_06_End::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Weaver_Attack_06_End* pInstance = new CState_Weaver_Attack_06_End(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Weaver_Attack_06_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Weaver_Attack_06_End::Clone(void* pArg)
{
	CState_Weaver_Attack_06_End* pInstance = new CState_Weaver_Attack_06_End(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Weaver_Attack_06_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Weaver_Attack_06_End::Free()
{
	__super::Free();
}
