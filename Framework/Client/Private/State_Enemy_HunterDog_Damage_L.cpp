#include "stdafx.h"
#include "..\Public\State_Enemy_HunterDog_Damage_L.h"
#include "GameInstance.h"
#include "Enemy.h"
CState_Enemy_HunterDog_Damage_L::CState_Enemy_HunterDog_Damage_L(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_HunterDog_Damage_L::CState_Enemy_HunterDog_Damage_L(const CState_Enemy_HunterDog_Damage_L& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_HunterDog_Damage_L::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_ENEMY_HUNTERDOG_IDLE_BATTLE);
	m_ReserveProbabilitys[STATE_ENEMY_HUNTERDOG_IDLE_BATTLE] = 1;

	return S_OK;
}

HRESULT CState_Enemy_HunterDog_Damage_L::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Enemy_HunterDog_Damage_L::Enter(void* pArg)
{

	m_fAnimSpeed = 0.5f;

	Set_CurrentHp();

}

void CState_Enemy_HunterDog_Damage_L::Tick(_float fTimeDelta)
{

	if (Is_AnimFinished(m_pState_Handler,m_pTargetModel))
		m_bEndCurState = true;

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_HunterDog_Damage_L::Exit(void* pArg)
{
	Set_CurrentHp();

}

_bool CState_Enemy_HunterDog_Damage_L::Check_Condition(CState_Handler* pHandler)
{
	return false;
}


CState_Enemy_HunterDog_Damage_L* CState_Enemy_HunterDog_Damage_L::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_HunterDog_Damage_L* pInstance = new CState_Enemy_HunterDog_Damage_L(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_HunterDog_Damage_L");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_HunterDog_Damage_L::Clone(void* pArg)
{
	CState_Enemy_HunterDog_Damage_L* pInstance = new CState_Enemy_HunterDog_Damage_L(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_HunterDog_Damage_L");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_HunterDog_Damage_L::Free()
{
	__super::Free();
}
