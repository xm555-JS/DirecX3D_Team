#include "stdafx.h"
#include "..\Public\State_Enemy_Gangster_Cap_Walk_B.h"
#include "GameInstance.h"
#include "Enemy.h"
CState_Enemy_Gangster_Cap_Walk_B::CState_Enemy_Gangster_Cap_Walk_B(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_Gangster_Cap_Walk_B::CState_Enemy_Gangster_Cap_Walk_B(const CState_Enemy_Gangster_Cap_Walk_B& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_Gangster_Cap_Walk_B::Initialize_Prototype()
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

	m_fBackStartDistance = 2.5f;

	return S_OK;
}

HRESULT CState_Enemy_Gangster_Cap_Walk_B::Initialize(void * pArg)
{
	m_fBackEndDistance = 4.5f;

	return S_OK;
}

void CState_Enemy_Gangster_Cap_Walk_B::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fPlayTime = 4.f;

	m_fAnimSpeed = 1.f;

	m_fSpeed = 3.f;
}

void CState_Enemy_Gangster_Cap_Walk_B::Tick(_float fTimeDelta)
{
	if(m_fPlayTime<m_pState_Handler->Get_TimeAcc())
		m_bEndCurState = true;

	if (Get_PlayerPosLength() > m_fBackEndDistance)
	{
		m_bEndCurState = true;
	}
	else
	{
		_vector vPlayerPos = m_pState_Handler->Get_Player()->Get_Pos();

		m_pTargetModel->Get_Owner()->Get_Transform()->YFixedLookAt(vPlayerPos);
		CNavigation* pNavigation = (CNavigation*)m_pState_Handler->Get_Owner()->Get_Component(NAVIGATION);
		m_pTargetModel->Get_Owner()->Get_Transform()->Go_Back(fTimeDelta, m_fSpeed, pNavigation);

	}


	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_Gangster_Cap_Walk_B::Exit(void* pArg)
{

}

_bool CState_Enemy_Gangster_Cap_Walk_B::Check_Condition(CState_Handler* pHandler)
{
	_vector vPlayerPos = pHandler->Get_Player()->Get_Pos();
	_vector vOwnerPos = pHandler->Get_Owner()->Get_Pos();

	_float fLength = XMVectorGetX((XMVector3Length(vPlayerPos - vOwnerPos)));

	if (m_fBackStartDistance > fLength)
		return true;

	return false;
}


CState_Enemy_Gangster_Cap_Walk_B* CState_Enemy_Gangster_Cap_Walk_B::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_Gangster_Cap_Walk_B* pInstance = new CState_Enemy_Gangster_Cap_Walk_B(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_Gangster_Cap_Walk_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_Gangster_Cap_Walk_B::Clone(void* pArg)
{
	CState_Enemy_Gangster_Cap_Walk_B* pInstance = new CState_Enemy_Gangster_Cap_Walk_B(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_Gangster_Cap_Walk_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_Gangster_Cap_Walk_B::Free()
{
	__super::Free();
}
