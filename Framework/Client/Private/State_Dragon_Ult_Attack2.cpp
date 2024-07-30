#include "stdafx.h"
#include "..\Public\State_Dragon_Ult_Attack2.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"
#include "HIerarchyNode.h"

CState_Dragon_Ult_Attack2::CState_Dragon_Ult_Attack2(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Dragon_Ult_Attack2::CState_Dragon_Ult_Attack2(const CState_Dragon_Ult_Attack2& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Dragon_Ult_Attack2::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_DRAGON_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_DRAGON_FALL_LAND);

	m_ReserveProbabilitys[STATE_BOSS_DRAGON_FALL_LAND] = 1;

	return S_OK;
}

HRESULT CState_Dragon_Ult_Attack2::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Dragon_Ult_Attack2::Enter(void* pArg)
{
	__super::Enter(pArg);
}

void CState_Dragon_Ult_Attack2::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;


	CState_AI::Tick(fTimeDelta);
}

void CState_Dragon_Ult_Attack2::Exit(void* pArg)
{
	m_pTargetModel->Set_ModelEasingRatio(0.2f);
}

_bool CState_Dragon_Ult_Attack2::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Dragon_Ult_Attack2* CState_Dragon_Ult_Attack2::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Dragon_Ult_Attack2* pInstance = new CState_Dragon_Ult_Attack2(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Dragon_Ult_Attack2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Dragon_Ult_Attack2::Clone(void* pArg)
{
	CState_Dragon_Ult_Attack2* pInstance = new CState_Dragon_Ult_Attack2(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Dragon_Ult_Attack2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Dragon_Ult_Attack2::Free()
{
	__super::Free();
}
