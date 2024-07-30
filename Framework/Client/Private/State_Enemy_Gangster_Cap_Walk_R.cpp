#include "stdafx.h"
#include "..\Public\State_Enemy_Gangster_Cap_Walk_R.h"
#include "GameInstance.h"
#include "Enemy.h"
CState_Enemy_Gangster_Cap_Walk_R::CState_Enemy_Gangster_Cap_Walk_R(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_Gangster_Cap_Walk_R::CState_Enemy_Gangster_Cap_Walk_R(const CState_Enemy_Gangster_Cap_Walk_R& rhs)
	: CState_Enemy_Normal(rhs)
{
	m_fRunStartDistance = rhs.m_fRunStartDistance;

}

HRESULT CState_Enemy_Gangster_Cap_Walk_R::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_ENEMY_GANGSTER_CAP_DEAD_1);

	//m_ReserveStates.push_back(STATE_ENEMY_GANGSTER_CAP_DAMAGE_B);
	//m_ReserveStates.push_back(STATE_ENEMY_GANGSTER_CAP_DAMAGE_L);
	//m_ReserveStates.push_back(STATE_ENEMY_GANGSTER_CAP_DAMAGE_R);

	m_ReserveStates.push_back(STATE_ENEMY_GANGSTER_CAP_IDLE_BATTLE);


	//m_ReserveProbabilitys[STATE_ENEMY_GANGSTER_CAP_DAMAGE_B] = 1;
	//m_ReserveProbabilitys[STATE_ENEMY_GANGSTER_CAP_DAMAGE_L] = 1;
	//m_ReserveProbabilitys[STATE_ENEMY_GANGSTER_CAP_DAMAGE_R] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_GANGSTER_CAP_IDLE_BATTLE] = 1;

	m_fRunStartDistance = 5.f;
	m_fBackStartDistance = 2.5f;

	return S_OK;
}

HRESULT CState_Enemy_Gangster_Cap_Walk_R::Initialize(void* pArg)
{

	return S_OK;
}

void CState_Enemy_Gangster_Cap_Walk_R::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fPlayTime = 3.f;
	m_fAnimSpeed = 1.f;

	m_fSpeed = 3.f;

}

void CState_Enemy_Gangster_Cap_Walk_R::Tick(_float fTimeDelta)
{
	_vector vPlayerPos = m_pState_Handler->Get_Player()->Get_Pos();

	if (Get_PlayerPosLength() > m_fRunStartDistance)
		m_bEndCurState = true;

	if (m_fPlayTime < m_pState_Handler->Get_TimeAcc())
	{
		m_bEndCurState = true;
	}

	m_pTargetModel->Get_Owner()->Get_Transform()->YFixedLookAt(m_pState_Handler->Get_Player()->Get_Pos());

	CNavigation* pNavigation = (CNavigation*)m_pState_Handler->Get_Owner()->Get_Component(NAVIGATION);
	m_pTargetModel->Get_Owner()->Get_Transform()->Go_Right(fTimeDelta, m_fSpeed, pNavigation);

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_Gangster_Cap_Walk_R::Exit(void* pArg)
{

}

_bool CState_Enemy_Gangster_Cap_Walk_R::Check_Condition(CState_Handler* pHandler)
{
	_vector vPlayerPos = pHandler->Get_Player()->Get_Pos();
	_vector vOwnerPos = pHandler->Get_Owner()->Get_Pos();

	_float fLength = XMVectorGetX((XMVector3Length(vPlayerPos - vOwnerPos)));

	if (m_fRunStartDistance > fLength && m_fBackStartDistance < fLength)
		return true;

	return false;
}


CState_Enemy_Gangster_Cap_Walk_R* CState_Enemy_Gangster_Cap_Walk_R::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_Gangster_Cap_Walk_R* pInstance = new CState_Enemy_Gangster_Cap_Walk_R(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_Gangster_Cap_Walk_R");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_Gangster_Cap_Walk_R::Clone(void* pArg)
{
	CState_Enemy_Gangster_Cap_Walk_R* pInstance = new CState_Enemy_Gangster_Cap_Walk_R(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_Gangster_Cap_Walk_R");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_Gangster_Cap_Walk_R::Free()
{
	__super::Free();
}
