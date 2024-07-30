#include "stdafx.h"
#include "..\Public\State_Enemy_MechanicalCity_Gunner_Damage_L.h"
#include "GameInstance.h"
#include "Enemy.h"
CState_Enemy_MechanicalCity_Gunner_Damage_L::CState_Enemy_MechanicalCity_Gunner_Damage_L(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_MechanicalCity_Gunner_Damage_L::CState_Enemy_MechanicalCity_Gunner_Damage_L(const CState_Enemy_MechanicalCity_Gunner_Damage_L& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_MechanicalCity_Gunner_Damage_L::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_DEAD_END);

	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_B);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_F);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_L);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_R);

	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_IDLE_BATTLE);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_IDLE_NORMAL);


	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_B] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_F] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_L] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_R] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_IDLE_BATTLE] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_IDLE_NORMAL] = 1;

	return S_OK;
}

HRESULT CState_Enemy_MechanicalCity_Gunner_Damage_L::Initialize(void* pArg)
{

	return S_OK;
}

void CState_Enemy_MechanicalCity_Gunner_Damage_L::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 0.f;
	Set_CurrentHp();
	m_pTargetModel->Set_ModelEasingRatio(0.f);
	m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_pState_Handler->Get_CurStateType())]->Set_KeyFrame(5);
}

void CState_Enemy_MechanicalCity_Gunner_Damage_L::Tick(_float fTimeDelta)
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

	if (Is_AnimFinished())
		m_bEndCurState = true;

	if (!m_bPushed)
		Set_DamagedAction(&m_bPushed);

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_MechanicalCity_Gunner_Damage_L::Exit(void* pArg)
{
	m_bPushed = false;
	m_pTargetModel->Set_ModelEasingRatio(0.1f);
}

_bool CState_Enemy_MechanicalCity_Gunner_Damage_L::Check_Condition(CState_Handler* pHandler)
{
	return Is_Damaged(pHandler);
}


CState_Enemy_MechanicalCity_Gunner_Damage_L* CState_Enemy_MechanicalCity_Gunner_Damage_L::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_MechanicalCity_Gunner_Damage_L* pInstance = new CState_Enemy_MechanicalCity_Gunner_Damage_L(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_MechanicalCity_Gunner_Damage_L");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_MechanicalCity_Gunner_Damage_L::Clone(void* pArg)
{
	CState_Enemy_MechanicalCity_Gunner_Damage_L* pInstance = new CState_Enemy_MechanicalCity_Gunner_Damage_L(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_MechanicalCity_Gunner_Damage_L");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_MechanicalCity_Gunner_Damage_L::Free()
{
	__super::Free();
}
