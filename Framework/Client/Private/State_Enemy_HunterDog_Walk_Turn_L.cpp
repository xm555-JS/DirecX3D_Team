#include "stdafx.h"
#include "..\Public\State_Enemy_HunterDog_Walk_Turn_L.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CState_Enemy_HunterDog_Walk_Turn_L::CState_Enemy_HunterDog_Walk_Turn_L(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_HunterDog_Walk_Turn_L::CState_Enemy_HunterDog_Walk_Turn_L(const CState_Enemy_HunterDog_Walk_Turn_L& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_HunterDog_Walk_Turn_L::Initialize_Prototype()
{

	m_ReserveStates.push_back(STATE_ENEMY_HUNTERDOG_WALK_L);
	m_ReserveProbabilitys[STATE_ENEMY_HUNTERDOG_WALK_L] = 1;


	return S_OK;
}

HRESULT CState_Enemy_HunterDog_Walk_Turn_L::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Enemy_HunterDog_Walk_Turn_L::Enter(void* pArg)
{

}

void CState_Enemy_HunterDog_Walk_Turn_L::Tick(_float fTimeDelta)
{


	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_HunterDog_Walk_Turn_L::Exit(void* pArg)
{
}

_bool CState_Enemy_HunterDog_Walk_Turn_L::Check_Condition(CState_Handler* pHandler)
{


	return false;
}


CState_Enemy_HunterDog_Walk_Turn_L* CState_Enemy_HunterDog_Walk_Turn_L::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_HunterDog_Walk_Turn_L* pInstance = new CState_Enemy_HunterDog_Walk_Turn_L(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_HunterDog_Walk_L");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_HunterDog_Walk_Turn_L::Clone(void* pArg)
{
	CState_Enemy_HunterDog_Walk_Turn_L* pInstance = new CState_Enemy_HunterDog_Walk_Turn_L(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_HunterDog_Walk_L");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_HunterDog_Walk_Turn_L::Free()
{
	__super::Free();
}
