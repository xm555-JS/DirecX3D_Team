#include "stdafx.h"
#include "..\Public\State_Enemy_MechanicalCity_DoorKeeper_Damage_B.h"
#include "GameInstance.h"
#include "Enemy.h"
#include "Animation.h"

CState_Enemy_MechanicalCity_DoorKeeper_Damage_B::CState_Enemy_MechanicalCity_DoorKeeper_Damage_B(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_MechanicalCity_DoorKeeper_Damage_B::CState_Enemy_MechanicalCity_DoorKeeper_Damage_B(const CState_Enemy_MechanicalCity_DoorKeeper_Damage_B& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_MechanicalCity_DoorKeeper_Damage_B::Initialize_Prototype()
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

HRESULT CState_Enemy_MechanicalCity_DoorKeeper_Damage_B::Initialize(void* pArg)
{

	return S_OK;
}

void CState_Enemy_MechanicalCity_DoorKeeper_Damage_B::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 0.f;
	m_strRootBoneName = "Root_Bone";
	m_pTargetModel->Set_ModelEasingRatio(0.001f);
	Set_CurrentHp();
	m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_pState_Handler->Get_CurStateType())]->Set_KeyFrame(5);
}

void CState_Enemy_MechanicalCity_DoorKeeper_Damage_B::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (0.f == m_fAnimSpeed)
	{
		m_fStopTime += pGameInstance->Get_TimeDelta(TEXT("Timer_60"));
		if (0.1f <= m_fStopTime)
		{
			m_fStopTime = 0.f;
			m_fAnimSpeed = 1.f;
		}
	}

	if (Is_Damaged())
		m_bEndCurState = true;

	if(30<Get_CurrentKeyFrame())
		m_bEndCurState = true;

	if (!m_bPushed)
		Set_DamagedAction(&m_bPushed);


	__super::Tick(fTimeDelta);

}

void CState_Enemy_MechanicalCity_DoorKeeper_Damage_B::Exit(void* pArg)
{
	if (Is_Damaged())
		m_pTargetModel->Set_PreAnimationIndex(7);

	m_pTargetModel->Set_ModelEasingRatio(0.1f);
	m_bPushed = false;
}

_bool CState_Enemy_MechanicalCity_DoorKeeper_Damage_B::Check_Condition(CState_Handler* pHandler)
{
	return Is_Damaged(pHandler);
}


CState_Enemy_MechanicalCity_DoorKeeper_Damage_B* CState_Enemy_MechanicalCity_DoorKeeper_Damage_B::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_MechanicalCity_DoorKeeper_Damage_B* pInstance = new CState_Enemy_MechanicalCity_DoorKeeper_Damage_B(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_MechanicalCity_DoorKeeper_Damage_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_MechanicalCity_DoorKeeper_Damage_B::Clone(void* pArg)
{
	CState_Enemy_MechanicalCity_DoorKeeper_Damage_B* pInstance = new CState_Enemy_MechanicalCity_DoorKeeper_Damage_B(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_MechanicalCity_DoorKeeper_Damage_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_MechanicalCity_DoorKeeper_Damage_B::Free()
{
	__super::Free();
}
