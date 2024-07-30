#include "stdafx.h"
#include "..\Public\State_Dragon_Fall_Loop.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

CState_Dragon_Fall_Loop::CState_Dragon_Fall_Loop(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Dragon_Fall_Loop::CState_Dragon_Fall_Loop(const CState_Dragon_Fall_Loop& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Dragon_Fall_Loop::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_DRAGON_APEAR);

	m_ReserveProbabilitys[STATE_BOSS_DRAGON_APEAR] = 1;

	return S_OK;
}

HRESULT CState_Dragon_Fall_Loop::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Dragon_Fall_Loop::Enter(void* pArg)
{
	__super::Enter(pArg);
}

void CState_Dragon_Fall_Loop::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;


	CState_AI::Tick(fTimeDelta);
}

void CState_Dragon_Fall_Loop::Exit(void* pArg)
{
	m_pTargetModel->Set_ModelEasingRatio(1.f);
}

_bool CState_Dragon_Fall_Loop::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Dragon_Fall_Loop* CState_Dragon_Fall_Loop::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Dragon_Fall_Loop* pInstance = new CState_Dragon_Fall_Loop(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Dragon_Fall_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Dragon_Fall_Loop::Clone(void* pArg)
{
	CState_Dragon_Fall_Loop* pInstance = new CState_Dragon_Fall_Loop(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Dragon_Fall_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Dragon_Fall_Loop::Free()
{
	__super::Free();
}
