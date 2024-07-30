#include "stdafx.h"
#include "..\Public\State_Shirley_Idle.h"
#include "GameInstance.h"
#include "Npc_Shirley.h"

CState_Shirley_Idle::CState_Shirley_Idle(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Npc(pManager, eState)
{
}

CState_Shirley_Idle::CState_Shirley_Idle(const CState_Shirley_Idle& rhs)
	: CState_Npc(rhs)
{
}

HRESULT CState_Shirley_Idle::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_NCP_SHIRLEY_BRILLIANT);
	m_ReserveStates.push_back(STATE_NCP_SHIRLEY_GREETING);
	m_ReserveStates.push_back(STATE_NCP_SHIRLEY_HUNGRY);
	m_ReserveStates.push_back(STATE_NCP_SHIRLEY_SURPRISE);
	m_ReserveStates.push_back(STATE_NCP_SHIRLEY_THANK);
	m_ReserveStates.push_back(STATE_NCP_SHIRLEY_CONVERSATION_A);
	m_ReserveStates.push_back(STATE_NCP_SHIRLEY_CONVERSATION_B);
	m_ReserveStates.push_back(STATE_NCP_SHIRLEY_CONVERSATION_C);
	m_ReserveStates.push_back(STATE_NCP_SHIRLEY_WORRY_A);
	m_ReserveStates.push_back(STATE_NCP_SHIRLEY_WORRY_B);

	m_ReserveProbabilitys[STATE_NCP_SHIRLEY_BRILLIANT] = 1;
	m_ReserveProbabilitys[STATE_NCP_SHIRLEY_GREETING] = 1;
	m_ReserveProbabilitys[STATE_NCP_SHIRLEY_HUNGRY] = 1;
	m_ReserveProbabilitys[STATE_NCP_SHIRLEY_SURPRISE] = 1;
	m_ReserveProbabilitys[STATE_NCP_SHIRLEY_THANK] = 1;
	m_ReserveProbabilitys[STATE_NCP_SHIRLEY_CONVERSATION_A] = 1;
	m_ReserveProbabilitys[STATE_NCP_SHIRLEY_CONVERSATION_B] = 1;
	m_ReserveProbabilitys[STATE_NCP_SHIRLEY_CONVERSATION_C] = 1;
	m_ReserveProbabilitys[STATE_NCP_SHIRLEY_WORRY_A] = 1;
	m_ReserveProbabilitys[STATE_NCP_SHIRLEY_WORRY_B] = 1;

	return S_OK;
}

HRESULT CState_Shirley_Idle::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Shirley_Idle::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_bEndCurState = true;
}

void CState_Shirley_Idle::Tick(_float fTimeDelta)
{
	STATE_TYPE	eNextState = CState_AI::Ready_NextState();
	if (STATE_END != eNextState)
	{
		// 셜리와 대화할 때 같은 애니메이션이 계속 실행됨
		// 그래서 현재 스크립트와 이전 스크립트를 비교해서
		// 한 스크립트에 한 애니메이션만 실행되게 함
		if (true == ((CNpc_Shirley*)m_pState_Handler->Get_Owner())->Get_Script())
		{
			_uint iCurScriptPhase = ((CNpc_Shirley*)m_pState_Handler->Get_Owner())->Get_ScriptPhase();

			if (m_iPreScriptPhase != iCurScriptPhase)
				m_pState_Handler->Change_State(eNextState);

			m_iPreScriptPhase = ((CNpc_Shirley*)m_pState_Handler->Get_Owner())->Get_ScriptPhase();
		}
	}

	m_pTargetModel->Play_Animation(fTimeDelta * m_fAnimSpeed);
}

void CState_Shirley_Idle::Exit(void* pArg)
{
}

_bool CState_Shirley_Idle::Check_Condition(CState_Handler* pHandler)
{
	return true;
}


CState_Shirley_Idle* CState_Shirley_Idle::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Shirley_Idle* pInstance = new CState_Shirley_Idle(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Shirley_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Shirley_Idle::Clone(void* pArg)
{
	CState_Shirley_Idle* pInstance = new CState_Shirley_Idle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Shirley_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Shirley_Idle::Free()
{
}
