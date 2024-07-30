#include "stdafx.h"
#include "..\Public\State_Enemy_HyenaGang_Minions_Attack_Charge_Shot.h"
#include "GameInstance.h"
#include "Player.h"
#include "HIerarchyNode.h"

CState_Enemy_HyenaGang_Minions_Attack_Charge_Shot::CState_Enemy_HyenaGang_Minions_Attack_Charge_Shot(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_HyenaGang_Minions_Attack_Charge_Shot::CState_Enemy_HyenaGang_Minions_Attack_Charge_Shot(const CState_Enemy_HyenaGang_Minions_Attack_Charge_Shot& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_HyenaGang_Minions_Attack_Charge_Shot::Initialize_Prototype()
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

HRESULT CState_Enemy_HyenaGang_Minions_Attack_Charge_Shot::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Enemy_HyenaGang_Minions_Attack_Charge_Shot::Enter(void* pArg)
{
	m_strRootBoneName = "Root_bone";

}

void CState_Enemy_HyenaGang_Minions_Attack_Charge_Shot::Tick(_float fTimeDelta)
{

	if (Is_Damaged())
		m_bEndCurState = true;

	if (Is_AnimFinished())
		m_bEndCurState = true;

	m_pTargetModel->Get_Owner()->Get_Transform()->LookAt(m_pState_Handler->Get_Player()->Get_Pos());

	CState_Enemy_Normal::Tick(fTimeDelta);


}

void CState_Enemy_HyenaGang_Minions_Attack_Charge_Shot::Exit(void* pArg)
{

}

_bool CState_Enemy_HyenaGang_Minions_Attack_Charge_Shot::Check_Condition(CState_Handler* pHandler)
{


	return false;
}


CState_Enemy_HyenaGang_Minions_Attack_Charge_Shot* CState_Enemy_HyenaGang_Minions_Attack_Charge_Shot::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_HyenaGang_Minions_Attack_Charge_Shot* pInstance = new CState_Enemy_HyenaGang_Minions_Attack_Charge_Shot(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_HyenaGang_Minions_Attack_Charge_Shot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_HyenaGang_Minions_Attack_Charge_Shot::Clone(void* pArg)
{
	CState_Enemy_HyenaGang_Minions_Attack_Charge_Shot* pInstance = new CState_Enemy_HyenaGang_Minions_Attack_Charge_Shot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_HyenaGang_Minions_Attack_Charge_Shot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_HyenaGang_Minions_Attack_Charge_Shot::Free()
{
	__super::Free();
}
