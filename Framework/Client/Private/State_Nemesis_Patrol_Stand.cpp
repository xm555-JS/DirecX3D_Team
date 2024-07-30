#include "..\Public\State_Nemesis_Patrol_Stand.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Nemesis_Patrol_Stand::CState_Nemesis_Patrol_Stand(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Nemesis_Patrol_Stand::CState_Nemesis_Patrol_Stand(const CState_Nemesis_Patrol_Stand& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Nemesis_Patrol_Stand::Initialize_Prototype()
{
	//m_ReserveStates.push_back(STATE_BOSS_NEMESIS_PATROL_AWAKE);

	//m_ReserveProbabilitys[STATE_BOSS_NEMESIS_PATROL_AWAKE] = 1;

	return S_OK;
}

HRESULT CState_Nemesis_Patrol_Stand::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Nemesis_Patrol_Stand::Enter(void* pArg)
{
	__super::Enter(pArg);

	// 거리 10은 임시 거리임 나중에 게임플레이 레벨에 올려놓고 확인 해봐야함
	m_fAwake_Distance = 10.f;
}

void CState_Nemesis_Patrol_Stand::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	
	// 플레이어랑 가까워진다면 Awake_State로 변경
	if (Get_CurrentLength(m_pState_Handler) < m_fAwake_Distance)
	{
		m_bEndCurState = true; 
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Nemesis_Patrol_Stand::Exit(void* pArg)
{
}

_bool CState_Nemesis_Patrol_Stand::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Nemesis_Patrol_Stand* CState_Nemesis_Patrol_Stand::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Nemesis_Patrol_Stand* pInstance = new CState_Nemesis_Patrol_Stand(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Nemesis_Patrol_Stand");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Nemesis_Patrol_Stand::Clone(void* pArg)
{
	CState_Nemesis_Patrol_Stand* pInstance = new CState_Nemesis_Patrol_Stand(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Nemesis_Patrol_Stand");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Nemesis_Patrol_Stand::Free()
{
	__super::Free();
}
