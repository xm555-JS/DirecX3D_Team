#include "stdafx.h"
#include "..\Public\State_Enemy_Gangster_Cap_Damage_R.h"
#include "GameInstance.h"
#include "Enemy.h"
CState_Enemy_Gangster_Cap_Damage_R::CState_Enemy_Gangster_Cap_Damage_R(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_Gangster_Cap_Damage_R::CState_Enemy_Gangster_Cap_Damage_R(const CState_Enemy_Gangster_Cap_Damage_R& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_Gangster_Cap_Damage_R::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_ENEMY_GANGSTER_CAP_DEAD_1);

	m_ReserveStates.push_back(STATE_ENEMY_GANGSTER_CAP_IDLE_BATTLE);
	m_ReserveProbabilitys[STATE_ENEMY_GANGSTER_CAP_IDLE_BATTLE] = 1;

	//m_ReserveStates.push_back(STATE_ENEMY_GANGSTER_CAP_DAMAGE_B);
	//m_ReserveProbabilitys[STATE_ENEMY_GANGSTER_CAP_DAMAGE_B] = 1;
	//m_ReserveStates.push_back(STATE_ENEMY_GANGSTER_CAP_DAMAGE_L);
	//m_ReserveProbabilitys[STATE_ENEMY_GANGSTER_CAP_DAMAGE_L] = 1;
	//m_ReserveStates.push_back(STATE_ENEMY_GANGSTER_CAP_DAMAGE_R);
	//m_ReserveProbabilitys[STATE_ENEMY_GANGSTER_CAP_DAMAGE_R] = 1;

	return S_OK;
}

HRESULT CState_Enemy_Gangster_Cap_Damage_R::Initialize(void * pArg)
{

	return S_OK;
}

void CState_Enemy_Gangster_Cap_Damage_R::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 1.f;
	Set_CurrentHp();
	m_pTargetModel->Set_ModelEasingRatio(0.f);
	m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_pState_Handler->Get_CurStateType())]->Set_KeyFrame(5);
}

void CState_Enemy_Gangster_Cap_Damage_R::Tick(_float fTimeDelta)
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

void CState_Enemy_Gangster_Cap_Damage_R::Exit(void* pArg)
{
	m_bPushed = false;
	m_pTargetModel->Set_ModelEasingRatio(0.1f);
}

_bool CState_Enemy_Gangster_Cap_Damage_R::Check_Condition(CState_Handler* pHandler)
{

	return Is_Damaged(pHandler);
}


CState_Enemy_Gangster_Cap_Damage_R* CState_Enemy_Gangster_Cap_Damage_R::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_Gangster_Cap_Damage_R* pInstance = new CState_Enemy_Gangster_Cap_Damage_R(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_Gangster_Cap_Damage_R");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_Gangster_Cap_Damage_R::Clone(void* pArg)
{
	CState_Enemy_Gangster_Cap_Damage_R* pInstance = new CState_Enemy_Gangster_Cap_Damage_R(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_Gangster_Cap_Damage_R");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_Gangster_Cap_Damage_R::Free()
{
	__super::Free();
}
