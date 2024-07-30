#include "stdafx.h"
#include "..\Public\State_Shirley_Worry_A.h"
#include "GameInstance.h"
#include "Npc_Shirley.h"

CState_Shirley_Worry_A::CState_Shirley_Worry_A(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Npc(pManager, eState)
{
}

CState_Shirley_Worry_A::CState_Shirley_Worry_A(const CState_Shirley_Worry_A& rhs)
	: CState_Npc(rhs)
{
}

HRESULT CState_Shirley_Worry_A::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_NCP_SHIRLEY_IDLE);

	m_ReserveProbabilitys[STATE_NCP_SHIRLEY_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Shirley_Worry_A::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Shirley_Worry_A::Enter(void* pArg)
{
	__super::Enter(pArg);
}

void CState_Shirley_Worry_A::Tick(_float fTimeDelta)
{
	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	CState_AI::Tick(fTimeDelta);
}

void CState_Shirley_Worry_A::Exit(void* pArg)
{
}

_bool CState_Shirley_Worry_A::Check_Condition(CState_Handler* pHandler)
{
	if (false == ((CNpc_Shirley*)pHandler->Get_Owner())->Get_Script())
		return false;

	if (CNpc_Shirley::CUBE_ORDER_SCRIPT_4 == ((CNpc_Shirley*)pHandler->Get_Owner())->Get_ScriptPhase()
		|| CNpc_Shirley::COLLECT_LETTUCE_SCRIPT_2 == ((CNpc_Shirley*)pHandler->Get_Owner())->Get_ScriptPhase()
		|| CNpc_Shirley::KILL_GANG_SWARM_SCRIPT_3 == ((CNpc_Shirley*)pHandler->Get_Owner())->Get_ScriptPhase()
		|| CNpc_Shirley::KILL_ROBELLA_SCRIPT_CLEAR == ((CNpc_Shirley*)pHandler->Get_Owner())->Get_ScriptPhase()
		|| CNpc_Shirley::MAKE_COOKING_SCRIPT_3 == ((CNpc_Shirley*)pHandler->Get_Owner())->Get_ScriptPhase()
		|| CNpc_Shirley::KILL_FROST_DRAGON_SCRIPT_CLEAR == ((CNpc_Shirley*)pHandler->Get_Owner())->Get_ScriptPhase()
		|| CNpc_Shirley::MAKE_COOKING_SCRIPT1_CLEAR == ((CNpc_Shirley*)pHandler->Get_Owner())->Get_ScriptPhase()
		|| CNpc_Shirley::KILL_MUTATION_SCRIPT_7 == ((CNpc_Shirley*)pHandler->Get_Owner())->Get_ScriptPhase()
		|| CNpc_Shirley::KILL_WEAVER_SCRIPT_CLEAR == ((CNpc_Shirley*)pHandler->Get_Owner())->Get_ScriptPhase())
	{
		return true;
	}

	return false;
}


CState_Shirley_Worry_A* CState_Shirley_Worry_A::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Shirley_Worry_A* pInstance = new CState_Shirley_Worry_A(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Shirley_Worry_A");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Shirley_Worry_A::Clone(void* pArg)
{
	CState_Shirley_Worry_A* pInstance = new CState_Shirley_Worry_A(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Shirley_Worry_A");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Shirley_Worry_A::Free()
{
}
