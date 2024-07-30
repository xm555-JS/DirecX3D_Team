#include "stdafx.h"
#include "..\Public\State_Angel_Reset.h"

#include "GameInstance.h"

#include "GameObject.h"
#include "Model.h"

CState_Angel_Reset::CState_Angel_Reset(CState_Manager* pManager, STATE_TYPE eState)
	:CState_AI(pManager, eState)
{
}

CState_Angel_Reset::CState_Angel_Reset(const CState_Angel_Reset& rhs)
	: CState_AI(rhs)
{
}

HRESULT CState_Angel_Reset::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_ANGEL_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_ANGEL_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Angel_Reset::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Angel_Reset::Enter(void* pArg)
{
}

void CState_Angel_Reset::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	CState_AI::Tick(fTimeDelta);
}

void CState_Angel_Reset::Exit(void* pArg)
{
}

_bool CState_Angel_Reset::Check_Condition(CState_Handler* pHandler)
{
	return true;
}

CState_Angel_Reset* CState_Angel_Reset::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Angel_Reset* pInstance = new CState_Angel_Reset(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Angel_Reset");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Angel_Reset::Clone(void* pArg)
{
	CState_Angel_Reset* pInstance = new CState_Angel_Reset(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Angel_Reset");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Angel_Reset::Free()
{
	__super::Free();
}
