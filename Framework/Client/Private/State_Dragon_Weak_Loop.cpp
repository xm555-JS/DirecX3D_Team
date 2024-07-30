#include "stdafx.h"
#include "..\Public\State_Dragon_Weak_Loop.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

CState_Dragon_Weak_Loop::CState_Dragon_Weak_Loop(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Dragon_Weak_Loop::CState_Dragon_Weak_Loop(const CState_Dragon_Weak_Loop& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Dragon_Weak_Loop::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_DRAGON_WEAK_END);

	m_ReserveProbabilitys[STATE_BOSS_DRAGON_WEAK_END] = 1;

	return S_OK;
}

HRESULT CState_Dragon_Weak_Loop::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Dragon_Weak_Loop::Enter(void* pArg)
{
	__super::Enter(pArg);
}

void CState_Dragon_Weak_Loop::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_fAccTime += fTimeDelta;

	if (m_fAccTime > 1.f)
		m_bEndCurState = true;


	CState_AI::Tick(fTimeDelta);
}

void CState_Dragon_Weak_Loop::Exit(void* pArg)
{
	m_fAccTime = 0.f;
}

_bool CState_Dragon_Weak_Loop::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Dragon_Weak_Loop* CState_Dragon_Weak_Loop::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Dragon_Weak_Loop* pInstance = new CState_Dragon_Weak_Loop(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Dragon_Weak_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Dragon_Weak_Loop::Clone(void* pArg)
{
	CState_Dragon_Weak_Loop* pInstance = new CState_Dragon_Weak_Loop(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Dragon_Weak_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Dragon_Weak_Loop::Free()
{
	__super::Free();
}
