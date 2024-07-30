#include "stdafx.h"
#include "..\Public\State_Npc.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "State_Manager.h"
#include "Quest_Manager.h"
#include "Npc_Shirley.h"

CState_Npc::CState_Npc(CState_Manager* pManager, STATE_TYPE eState)
	: CState_AI(pManager, eState)
{
}

CState_Npc::CState_Npc(const CState_Npc& rhs)
	: CState_AI(rhs)
{
}

HRESULT CState_Npc::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CState_Npc::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Npc::Enter(void* pArg)
{
	m_pOwner = m_pTargetModel->Get_Owner();
	m_pOwnerTranformCom = m_pOwner->Get_Transform();
	//m_pTarget = ((CNpc*)m_pOwner)->Get_Target();
}

void CState_Npc::Tick(_float fTimeDelta)
{

	STATE_TYPE	eNextState = CState_Npc::Ready_NextState();
	if (STATE_END != eNextState)
	{
		m_pState_Handler->Change_State(eNextState);
	}

	m_pTargetModel->Play_Animation(fTimeDelta * m_fAnimSpeed);
}

void CState_Npc::Exit(void* pArg)
{
}

STATE_TYPE CState_Npc::Ready_NextState()
{
	for (auto& eState : m_ForceStates)
	{
		if (m_pState_Manager->Get_State(eState)->Check_Condition(m_pState_Handler))
			return eState;
	}

	STATE_TYPE			eState = STATE_END;

	if (m_bEndCurState)
	{
		eState = m_ReserveStates.front();
		m_ReserveStates.erase(m_ReserveStates.begin());
		m_ReserveStates.push_back(eState);
	}

	return eState;
}

_bool CState_Npc::Check_Condition(CState_Handler* pHandler)
{
	return true;
}
void CState_Npc::Free()
{
}
