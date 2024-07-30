#include "stdafx.h"
#include "..\Public\State_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk.h"
#include "GameInstance.h"
#include "Enemy.h"
CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk::CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{

}

CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk::CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk(const CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DEAD_END);

	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_B);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_L);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_R);

	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_IDLE_NORMAL);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_IDLE_BATTLE);

	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_B] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_L] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_DAMAGE_R] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_IDLE_NORMAL] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOUBLEEDGEDSWORDADEPT_IDLE_BATTLE] = 1;

	m_fPatrolLength = 5.f; //플레이어와의 거리가 m_fPatrolLength보다 작을때만 실행가능

	return S_OK;
}

HRESULT CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk::Initialize(void* pArg)
{

	return S_OK;
}

void CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fPlayTime = 1.5f;
	m_fAnimSpeed = 1.f;

	m_fSpeed = 1.f;

	m_iTurnDir = Random(1, 2);
}

void CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk::Tick(_float fTimeDelta)
{
	if (Is_Damaged())
		m_bEndCurState = true;

	if (nullptr != ((CEnemy*)m_pOwner)->Get_Target())			//내 객체가 타겟을 가지고 있느냐
		m_bEndCurState = true;

	if (m_fPlayTime < m_pState_Handler->Get_TimeAcc())
		m_bEndCurState = true;


	CNavigation* pNavigation = (CNavigation*)m_pState_Handler->Get_Owner()->Get_Component(NAVIGATION);

	switch (m_iTurnDir)
	{
	case 1:
		if (1.f > m_pState_Handler->Get_TimeAcc())
		{
			m_pTargetModel->Get_Owner()->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta, 1.f);
		}

		break;
	case 2:
		if (1.f > m_pState_Handler->Get_TimeAcc())
		{
			m_pTargetModel->Get_Owner()->Get_Transform()->Turn(XMVectorSet(0.f, -1.f, 0.f, 0.f), fTimeDelta, 1.f);
		}
		break;
	}

	m_pTargetModel->Get_Owner()->Get_Transform()->Go_Straight(fTimeDelta, m_fSpeed, pNavigation);


	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk::Exit(void* pArg)
{

}

_bool CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk::Check_Condition(CState_Handler* pHandler)
{
	if (Is_Damaged(pHandler))
		return false;

	if (Get_StartPosLength(pHandler) < m_fPatrolLength) //플레이어와의 거리가 m_fPatrolLength보다 작을때만 실행가능
	{
		CEnemy* pOwner = (CEnemy*)pHandler->Get_Owner();

		if (nullptr == pOwner->Get_Target())
			return true;
	}

	return false;

}


CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk* CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk* pInstance = new CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk::Clone(void* pArg)
{
	CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk* pInstance = new CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_MechanicalCity_DoubleEdgedSwordAdept_Walk::Free()
{
	__super::Free();
}
