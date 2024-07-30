#include "stdafx.h"
#include "..\Public\State_Enemy_HyenaGang_Minions_Walk_F.h"
#include "GameInstance.h"
#include "Player.h"
#include "Enemy.h"
CState_Enemy_HyenaGang_Minions_Walk_F::CState_Enemy_HyenaGang_Minions_Walk_F(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_HyenaGang_Minions_Walk_F::CState_Enemy_HyenaGang_Minions_Walk_F(const CState_Enemy_HyenaGang_Minions_Walk_F& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_HyenaGang_Minions_Walk_F::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_ENEMY_HYENAGANG_MINIONS_DEAD_1);

	m_ReserveStates.push_back(STATE_ENEMY_HYENAGANG_MINIONS_DAMAGE_B);
	m_ReserveProbabilitys[STATE_ENEMY_HYENAGANG_MINIONS_DAMAGE_B] = 1;
	m_ReserveStates.push_back(STATE_ENEMY_HYENAGANG_MINIONS_DAMAGE_L);
	m_ReserveProbabilitys[STATE_ENEMY_HYENAGANG_MINIONS_DAMAGE_L] = 1;
	m_ReserveStates.push_back(STATE_ENEMY_HYENAGANG_MINIONS_DAMAGE_R);
	m_ReserveProbabilitys[STATE_ENEMY_HYENAGANG_MINIONS_DAMAGE_R] = 1;
	m_ReserveStates.push_back(STATE_ENEMY_HYENAGANG_MINIONS_DAMAGE_F);
	m_ReserveProbabilitys[STATE_ENEMY_HYENAGANG_MINIONS_DAMAGE_F] = 1;

	m_ReserveStates.push_back(STATE_ENEMY_HYENAGANG_MINIONS_IDLE_BATTLE);
	m_ReserveProbabilitys[STATE_ENEMY_HYENAGANG_MINIONS_IDLE_BATTLE] = 1;

	return S_OK;
}

HRESULT CState_Enemy_HyenaGang_Minions_Walk_F::Initialize(void* pArg)
{

	return S_OK;
}

void CState_Enemy_HyenaGang_Minions_Walk_F::Enter(void* pArg)
{
	
}

void CState_Enemy_HyenaGang_Minions_Walk_F::Tick(_float fTimeDelta)
{

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_HyenaGang_Minions_Walk_F::Exit(void* pArg)
{

}

_bool CState_Enemy_HyenaGang_Minions_Walk_F::Check_Condition(CState_Handler* pHandler)
{
	return false;
}


CState_Enemy_HyenaGang_Minions_Walk_F* CState_Enemy_HyenaGang_Minions_Walk_F::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_HyenaGang_Minions_Walk_F* pInstance = new CState_Enemy_HyenaGang_Minions_Walk_F(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_HyenaGang_Minions_Walk_F");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_HyenaGang_Minions_Walk_F::Clone(void* pArg)
{
	CState_Enemy_HyenaGang_Minions_Walk_F* pInstance = new CState_Enemy_HyenaGang_Minions_Walk_F(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_HyenaGang_Minions_Walk_F");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_HyenaGang_Minions_Walk_F::Free()
{
	__super::Free();
}
