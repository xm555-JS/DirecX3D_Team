#include "stdafx.h"
#include "..\Public\State_Enemy_HunterDog_Walk_L.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CState_Enemy_HunterDog_Walk_L::CState_Enemy_HunterDog_Walk_L(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_HunterDog_Walk_L::CState_Enemy_HunterDog_Walk_L(const CState_Enemy_HunterDog_Walk_L& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_HunterDog_Walk_L::Initialize_Prototype()
{

	m_ReserveStates.push_back(STATE_ENEMY_HUNTERDOG_IDLE_BATTLE);
	m_ReserveProbabilitys[STATE_ENEMY_HUNTERDOG_IDLE_BATTLE] = 1;


	return S_OK;
}

HRESULT CState_Enemy_HunterDog_Walk_L::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Enemy_HunterDog_Walk_L::Enter(void* pArg)
{

}

void CState_Enemy_HunterDog_Walk_L::Tick(_float fTimeDelta)
{

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_HunterDog_Walk_L::Exit(void* pArg)
{

}

_bool CState_Enemy_HunterDog_Walk_L::Check_Condition(CState_Handler* pHandler)
{

	return true;
}


CState_Enemy_HunterDog_Walk_L* CState_Enemy_HunterDog_Walk_L::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_HunterDog_Walk_L* pInstance = new CState_Enemy_HunterDog_Walk_L(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_HunterDog_Walk_L");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_HunterDog_Walk_L::Clone(void* pArg)
{
	CState_Enemy_HunterDog_Walk_L* pInstance = new CState_Enemy_HunterDog_Walk_L(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_HunterDog_Walk_L");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_HunterDog_Walk_L::Free()
{
	__super::Free();
}
