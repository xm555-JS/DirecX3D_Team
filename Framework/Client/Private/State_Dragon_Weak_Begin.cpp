#include "stdafx.h"
#include "..\Public\State_Dragon_Weak_Begin.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

CState_Dragon_Weak_Begin::CState_Dragon_Weak_Begin(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Dragon_Weak_Begin::CState_Dragon_Weak_Begin(const CState_Dragon_Weak_Begin& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Dragon_Weak_Begin::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_DRAGON_FALL_LAND);

	m_ReserveProbabilitys[STATE_BOSS_DRAGON_FALL_LAND] = 1;

	return S_OK;
}

HRESULT CState_Dragon_Weak_Begin::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Dragon_Weak_Begin::Enter(void* pArg)
{
	__super::Enter(pArg);
}

void CState_Dragon_Weak_Begin::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	CState_AI::Tick(fTimeDelta);
}

void CState_Dragon_Weak_Begin::Exit(void* pArg)
{
}

_bool CState_Dragon_Weak_Begin::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Dragon_Weak_Begin* CState_Dragon_Weak_Begin::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Dragon_Weak_Begin* pInstance = new CState_Dragon_Weak_Begin(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Dragon_Weak_Begin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Dragon_Weak_Begin::Clone(void* pArg)
{
	CState_Dragon_Weak_Begin* pInstance = new CState_Dragon_Weak_Begin(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Dragon_Weak_Begin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Dragon_Weak_Begin::Free()
{
	__super::Free();
}
