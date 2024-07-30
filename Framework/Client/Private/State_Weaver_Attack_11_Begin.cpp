#include "stdafx.h"
#include "..\Public\State_Weaver_Attack_11_Begin.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Weaver_Attack_11_Begin::CState_Weaver_Attack_11_Begin(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Weaver_Attack_11_Begin::CState_Weaver_Attack_11_Begin(const CState_Weaver_Attack_11_Begin& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Weaver_Attack_11_Begin::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_WEAVER_WEAK_BEGIN);
	m_ForceStates.push_back(STATE_BOSS_WEAVER_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_11_LOOP);

	m_ReserveProbabilitys[STATE_BOSS_WEAVER_ATTACK_11_LOOP] = 1;

	return S_OK;
}

HRESULT CState_Weaver_Attack_11_Begin::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Weaver_Attack_11_Begin::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAccTime = 0.f;
}

void CState_Weaver_Attack_11_Begin::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;


	CState_AI::Tick(fTimeDelta);
}

void CState_Weaver_Attack_11_Begin::Exit(void* pArg)
{
}

_bool CState_Weaver_Attack_11_Begin::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Weaver_Attack_11_Begin* CState_Weaver_Attack_11_Begin::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Weaver_Attack_11_Begin* pInstance = new CState_Weaver_Attack_11_Begin(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Weaver_Attack_11_Begin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Weaver_Attack_11_Begin::Clone(void* pArg)
{
	CState_Weaver_Attack_11_Begin* pInstance = new CState_Weaver_Attack_11_Begin(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Weaver_Attack_11_Begin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Weaver_Attack_11_Begin::Free()
{
	__super::Free();
}
