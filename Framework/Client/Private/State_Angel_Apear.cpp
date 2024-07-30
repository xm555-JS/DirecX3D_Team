#include "stdafx.h"
#include "..\Public\State_Angel_Apear.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

CState_Angel_Apear::CState_Angel_Apear(CState_Manager* pManager, STATE_TYPE eState)
	:CState_AI(pManager, eState)
{
}

CState_Angel_Apear::CState_Angel_Apear(const CState_Angel_Apear& rhs)
	: CState_AI(rhs)
{
}

HRESULT CState_Angel_Apear::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_ANGEL_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_ANGEL_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Angel_Apear::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Angel_Apear::Enter(void* pArg)
{
}

void CState_Angel_Apear::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;


	CState_AI::Tick(fTimeDelta);
}

void CState_Angel_Apear::Exit(void* pArg)
{
}

_bool CState_Angel_Apear::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Angel_Apear* CState_Angel_Apear::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Angel_Apear* pInstance = new CState_Angel_Apear(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Angel_Apear");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Angel_Apear::Clone(void* pArg)
{
	CState_Angel_Apear* pInstance = new CState_Angel_Apear(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Angel_Apear");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Angel_Apear::Free()
{
	__super::Free();
}
