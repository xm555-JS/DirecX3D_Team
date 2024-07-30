#include "stdafx.h"
#include "..\Public\State_Angel_Run_R.h"

#include "GameInstance.h"

#include "GameObject.h"
#include "Model.h"

#include "Unit.h"

CState_Angel_Run_R::CState_Angel_Run_R(CState_Manager* pManager, STATE_TYPE eState)
	:CState_AI(pManager, eState)
{
}

CState_Angel_Run_R::CState_Angel_Run_R(const CState_Angel_Run_R& rhs)
	: CState_AI(rhs)
{
}

HRESULT CState_Angel_Run_R::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_ANGEL_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_ANGEL_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Angel_Run_R::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Angel_Run_R::Enter(void* pArg)
{
}

void CState_Angel_Run_R::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	CState_AI::Tick(fTimeDelta);
}

void CState_Angel_Run_R::Exit(void* pArg)
{
}

_bool CState_Angel_Run_R::Check_Condition(CState_Handler* pHandler)
{
	return true;
}

CState_Angel_Run_R* CState_Angel_Run_R::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Angel_Run_R* pInstance = new CState_Angel_Run_R(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Angel_Run_R");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Angel_Run_R::Clone(void* pArg)
{
	CState_Angel_Run_R* pInstance = new CState_Angel_Run_R(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Angel_Run_R");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Angel_Run_R::Free()
{
	__super::Free();
}
