#include "..\Public\State_Nemesis_Idle.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Nemesis_Idle::CState_Nemesis_Idle(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Nemesis_Idle::CState_Nemesis_Idle(const CState_Nemesis_Idle& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Nemesis_Idle::Initialize_Prototype()
{
	//// 다음으로 넘어갈 수 있는 state      
 //	m_ReserveStates.push_back(STATE_BOSS_NEMESIS_WALK_F);
	//m_ReserveStates.push_back(STATE_BOSS_NEMESIS_RUN);

	//m_ReserveStates.push_back(STATE_BOSS_NEMESIS_ATTACK_01);
	//m_ReserveStates.push_back(STATE_BOSS_NEMESIS_ATTACK_02);

	////m_ReserveStates.push_back(STATE_BOSS_NEMESIS_SHIELD);

	//// 다음 state로 넘어갈 확률
	//m_ReserveProbabilitys[STATE_BOSS_NEMESIS_WALK_F] = 1;
	//m_ReserveProbabilitys[STATE_BOSS_NEMESIS_RUN] = 0.5f;

	//m_ReserveProbabilitys[STATE_BOSS_NEMESIS_ATTACK_01] = 0.5f;
	//m_ReserveProbabilitys[STATE_BOSS_NEMESIS_ATTACK_02] = 0.5f;

	//m_ReserveProbabilitys[STATE_BOSS_NEMESIS_SHIELD] = 1;

	return S_OK;
}

HRESULT CState_Nemesis_Idle::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Nemesis_Idle::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fAccTime = 0.f;
}

void CState_Nemesis_Idle::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_fAccTime += fTimeDelta;

	if (m_fAccTime >= 3.f)
	{
		m_bEndCurState = true;
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Nemesis_Idle::Exit(void* pArg)
{
}

_bool CState_Nemesis_Idle::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Nemesis_Idle* CState_Nemesis_Idle::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Nemesis_Idle* pInstance = new CState_Nemesis_Idle(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Nemesis_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Nemesis_Idle::Clone(void* pArg)
{
	CState_Nemesis_Idle* pInstance = new CState_Nemesis_Idle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Nemesis_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Nemesis_Idle::Free()
{
	__super::Free();
}