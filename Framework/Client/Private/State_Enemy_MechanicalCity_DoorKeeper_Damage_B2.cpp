#include "stdafx.h"
#include "..\Public\State_Enemy_MechanicalCity_DoorKeeper_Damage_B2.h"
#include "GameInstance.h"
#include "Enemy.h"
CState_Enemy_MechanicalCity_DoorKeeper_Damage_B2::CState_Enemy_MechanicalCity_DoorKeeper_Damage_B2(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_MechanicalCity_DoorKeeper_Damage_B2::CState_Enemy_MechanicalCity_DoorKeeper_Damage_B2(const CState_Enemy_MechanicalCity_DoorKeeper_Damage_B2& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_MechanicalCity_DoorKeeper_Damage_B2::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_ENEMY_MECHANICALCITY_DOORKEEPER_DEAD_END);

	//m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOORKEEPER_DAMAGE_B);

	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOORKEEPER_IDLE_BATTLE);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_DOORKEEPER_IDLE_NORMAL);


	//m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_DAMAGE_B] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_IDLE_BATTLE] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_DOORKEEPER_IDLE_NORMAL] = 1;

	return S_OK;
}

HRESULT CState_Enemy_MechanicalCity_DoorKeeper_Damage_B2::Initialize(void* pArg)
{

	return S_OK;
}

void CState_Enemy_MechanicalCity_DoorKeeper_Damage_B2::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fPlayTime = 0.01f;
	m_fAnimSpeed = 1.f;

	m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_pState_Handler->Get_CurStateType())]->Set_KeyFrame(5);
}

void CState_Enemy_MechanicalCity_DoorKeeper_Damage_B2::Tick(_float fTimeDelta)
{
	if (m_fPlayTime < m_pState_Handler->Get_TimeAcc())
	{
		if (Is_Damaged())
			m_bEndCurState = true;
	}

	CState_AI::Tick(fTimeDelta);

}

void CState_Enemy_MechanicalCity_DoorKeeper_Damage_B2::Exit(void* pArg)
{

}

_bool CState_Enemy_MechanicalCity_DoorKeeper_Damage_B2::Check_Condition(CState_Handler* pHandler)
{
	return Is_Damaged(pHandler);
}


CState_Enemy_MechanicalCity_DoorKeeper_Damage_B2* CState_Enemy_MechanicalCity_DoorKeeper_Damage_B2::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_MechanicalCity_DoorKeeper_Damage_B2* pInstance = new CState_Enemy_MechanicalCity_DoorKeeper_Damage_B2(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_MechanicalCity_DoorKeeper_Damage_B2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_MechanicalCity_DoorKeeper_Damage_B2::Clone(void* pArg)
{
	CState_Enemy_MechanicalCity_DoorKeeper_Damage_B2* pInstance = new CState_Enemy_MechanicalCity_DoorKeeper_Damage_B2(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_MechanicalCity_DoorKeeper_Damage_B2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_MechanicalCity_DoorKeeper_Damage_B2::Free()
{
	__super::Free();
}
