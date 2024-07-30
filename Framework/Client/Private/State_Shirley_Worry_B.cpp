#include "stdafx.h"
#include "..\Public\State_Shirley_Worry_B.h"
#include "GameInstance.h"
#include "Npc_Shirley.h"

CState_Shirley_Worry_B::CState_Shirley_Worry_B(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Npc(pManager, eState)
{
}

CState_Shirley_Worry_B::CState_Shirley_Worry_B(const CState_Shirley_Worry_B& rhs)
	: CState_Npc(rhs)
{
}

HRESULT CState_Shirley_Worry_B::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_NCP_SHIRLEY_IDLE);

	m_ReserveProbabilitys[STATE_NCP_SHIRLEY_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Shirley_Worry_B::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Shirley_Worry_B::Enter(void* pArg)
{
	__super::Enter(pArg);
}

void CState_Shirley_Worry_B::Tick(_float fTimeDelta)
{
	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	CState_AI::Tick(fTimeDelta);
}

void CState_Shirley_Worry_B::Exit(void* pArg)
{
}

_bool CState_Shirley_Worry_B::Check_Condition(CState_Handler* pHandler)
{
	if (false == ((CNpc_Shirley*)pHandler->Get_Owner())->Get_Script())
		return false;

	if (CNpc_Shirley::KILL_GANG_SWARM_SCRIPT_5 == ((CNpc_Shirley*)pHandler->Get_Owner())->Get_ScriptPhase()
		|| CNpc_Shirley::KILL_MECHANICALCITY_SWARM_1 == ((CNpc_Shirley*)pHandler->Get_Owner())->Get_ScriptPhase()
		|| CNpc_Shirley::KILL_MUTATION_SCRIPT_5 == ((CNpc_Shirley*)pHandler->Get_Owner())->Get_ScriptPhase())
	{
		return true;
	}

	return false;
}


CState_Shirley_Worry_B* CState_Shirley_Worry_B::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Shirley_Worry_B* pInstance = new CState_Shirley_Worry_B(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Shirley_Worry_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Shirley_Worry_B::Clone(void* pArg)
{
	CState_Shirley_Worry_B* pInstance = new CState_Shirley_Worry_B(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Shirley_Worry_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Shirley_Worry_B::Free()
{
}
