#include "stdafx.h"
#include "..\Public\State_Weaver_Reset.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Weaver_Reset::CState_Weaver_Reset(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Weaver_Reset::CState_Weaver_Reset(const CState_Weaver_Reset& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Weaver_Reset::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_WEAVER_WEAK_BEGIN);
	m_ForceStates.push_back(STATE_BOSS_WEAVER_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_DRAGON_CHASE);

	m_ReserveProbabilitys[STATE_BOSS_DRAGON_CHASE] = 1;

	return S_OK;
}

HRESULT CState_Weaver_Reset::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Weaver_Reset::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAccTime = 0.f;
}

void CState_Weaver_Reset::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;


	CState_AI::Tick(fTimeDelta);
}

void CState_Weaver_Reset::Exit(void* pArg)
{
}

_bool CState_Weaver_Reset::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Weaver_Reset* CState_Weaver_Reset::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Weaver_Reset* pInstance = new CState_Weaver_Reset(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Weaver_Reset");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Weaver_Reset::Clone(void* pArg)
{
	CState_Weaver_Reset* pInstance = new CState_Weaver_Reset(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Weaver_Reset");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Weaver_Reset::Free()
{
	__super::Free();
}
