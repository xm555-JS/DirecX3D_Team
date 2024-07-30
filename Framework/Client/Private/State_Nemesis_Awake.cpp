#include "..\Public\State_Nemesis_Awake.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Nemesis_Awake::CState_Nemesis_Awake(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Nemesis_Awake::CState_Nemesis_Awake(const CState_Nemesis_Awake& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Nemesis_Awake::Initialize_Prototype()
{
	//m_ReserveStates.push_back(STATE_BOSS_NEMESIS_IDLE);

	//m_ReserveProbabilitys[STATE_BOSS_NEMESIS_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Nemesis_Awake::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Nemesis_Awake::Enter(void* pArg)
{
	__super::Enter(pArg);
}

void CState_Nemesis_Awake::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	// 해당 클래스의 애니메이션이 끝나면 Idle_state로 넘어감
	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Nemesis_Awake::Exit(void* pArg)
{
}

_bool CState_Nemesis_Awake::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Nemesis_Awake* CState_Nemesis_Awake::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Nemesis_Awake* pInstance = new CState_Nemesis_Awake(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Nemesis_Awake");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Nemesis_Awake::Clone(void* pArg)
{
	CState_Nemesis_Awake* pInstance = new CState_Nemesis_Awake(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Nemesis_Awake");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Nemesis_Awake::Free()
{
	__super::Free();
}
