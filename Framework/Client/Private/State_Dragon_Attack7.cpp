#include "stdafx.h"
#include "..\Public\State_Dragon_Attack7.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"
#include "Enemy_Boss_Dragon.h"

CState_Dragon_Attack7::CState_Dragon_Attack7(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Dragon_Attack7::CState_Dragon_Attack7(const CState_Dragon_Attack7& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Dragon_Attack7::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_DRAGON_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_DRAGON_APEAR);

	m_ReserveProbabilitys[STATE_BOSS_DRAGON_APEAR] = 1;

	return S_OK;
}

HRESULT CState_Dragon_Attack7::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Dragon_Attack7::Enter(void* pArg)
{
	__super::Enter(pArg);
}

void CState_Dragon_Attack7::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	m_fAnimSpeed = 0.5f;

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;


	CState_AI::Tick(fTimeDelta);
}

void CState_Dragon_Attack7::Exit(void* pArg)
{
}

_bool CState_Dragon_Attack7::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Dragon_Attack7* CState_Dragon_Attack7::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Dragon_Attack7* pInstance = new CState_Dragon_Attack7(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Dragon_Attack7");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Dragon_Attack7::Clone(void* pArg)
{
	CState_Dragon_Attack7* pInstance = new CState_Dragon_Attack7(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Dragon_Attack7");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Dragon_Attack7::Free()
{
	__super::Free();
}
