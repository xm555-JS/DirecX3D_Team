#include "..\Public\State_Nemesis_Run.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Nemesis_Run::CState_Nemesis_Run(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Nemesis_Run::CState_Nemesis_Run(const CState_Nemesis_Run& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Nemesis_Run::Initialize_Prototype()
{
	//// 다음으로 넘어갈 수 있는 state
	//m_ReserveStates.push_back(STATE_BOSS_NEMESIS_IDLE);

	//// 다음 state로 넘어갈 확률
	//m_ReserveProbabilitys[STATE_BOSS_NEMESIS_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Nemesis_Run::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Nemesis_Run::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fStartLength = 20.f;
	m_fStopLength = 3.f;
}

void CState_Nemesis_Run::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	_bool bFollow = Follow_Player(m_pState_Handler, m_pTargetModel, m_fSpeed * 1.5f, fTimeDelta, m_fStopLength, m_fStartLength);

	// 거리가 가까워지면 다른 상태로 넘어감
	if (bFollow == false)
	{
		m_bEndCurState = true;
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Nemesis_Run::Exit(void* pArg)
{
}

_bool CState_Nemesis_Run::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	// 만약 플레이어와 거리가 일정 이상하면 이 상태를 실행하도록
	if (Get_CurrentLength(pHandler) >= m_fStartLength)
	{
		return true;
	}

	return false;
}

CState_Nemesis_Run* CState_Nemesis_Run::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Nemesis_Run* pInstance = new CState_Nemesis_Run(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Nemesis_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Nemesis_Run::Clone(void* pArg)
{
	CState_Nemesis_Run* pInstance = new CState_Nemesis_Run(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Nemesis_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Nemesis_Run::Free()
{
	__super::Free();
}
