#include "stdafx.h"
#include "..\Public\State_Shirley_Greeting.h"
#include "GameInstance.h"
#include "Npc_Shirley.h"

CState_Shirley_Greeting::CState_Shirley_Greeting(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Npc(pManager, eState)
{
}

CState_Shirley_Greeting::CState_Shirley_Greeting(const CState_Shirley_Greeting& rhs)
	: CState_Npc(rhs)
{
}

HRESULT CState_Shirley_Greeting::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_NCP_SHIRLEY_IDLE);

	m_ReserveProbabilitys[STATE_NCP_SHIRLEY_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Shirley_Greeting::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Shirley_Greeting::Enter(void* pArg)
{
	__super::Enter(pArg);
}

void CState_Shirley_Greeting::Tick(_float fTimeDelta)
{
	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	CState_AI::Tick(fTimeDelta);
}

void CState_Shirley_Greeting::Exit(void* pArg)
{
}

_bool CState_Shirley_Greeting::Check_Condition(CState_Handler* pHandler)
{
	if (false == ((CNpc_Shirley*)pHandler->Get_Owner())->Get_Script())
		return false;

	if (CNpc_Shirley::CUBE_ORDER_SCRIPT_1 == ((CNpc_Shirley*)pHandler->Get_Owner())->Get_ScriptPhase())
	{
		return true;
	}

	return false;
}


CState_Shirley_Greeting* CState_Shirley_Greeting::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Shirley_Greeting* pInstance = new CState_Shirley_Greeting(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Shirley_Greeting");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Shirley_Greeting::Clone(void* pArg)
{
	CState_Shirley_Greeting* pInstance = new CState_Shirley_Greeting(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Shirley_Greeting");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Shirley_Greeting::Free()
{
}
