#include "stdafx.h"
#include "..\Public\State_Enemy_HunterDog_Walk_Turn_R.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CState_Enemy_HunterDog_Walk_Turn_R::CState_Enemy_HunterDog_Walk_Turn_R(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_HunterDog_Walk_Turn_R::CState_Enemy_HunterDog_Walk_Turn_R(const CState_Enemy_HunterDog_Walk_Turn_R& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_HunterDog_Walk_Turn_R::Initialize_Prototype()
{

	m_ReserveStates.push_back(STATE_ENEMY_HUNTERDOG_WALK_R);
	m_ReserveProbabilitys[STATE_ENEMY_HUNTERDOG_WALK_R] = 1;

	return S_OK;
}

HRESULT CState_Enemy_HunterDog_Walk_Turn_R::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Enemy_HunterDog_Walk_Turn_R::Enter(void* pArg)
{

}

void CState_Enemy_HunterDog_Walk_Turn_R::Tick(_float fTimeDelta)
{

	CState_AI::Tick(fTimeDelta);

}

void CState_Enemy_HunterDog_Walk_Turn_R::Exit(void* pArg)
{

}

_bool CState_Enemy_HunterDog_Walk_Turn_R::Check_Condition(CState_Handler* pHandler)
{

	return false;
}


CState_Enemy_HunterDog_Walk_Turn_R* CState_Enemy_HunterDog_Walk_Turn_R::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_HunterDog_Walk_Turn_R* pInstance = new CState_Enemy_HunterDog_Walk_Turn_R(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_HunterDog_Walk_Turn_R");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_HunterDog_Walk_Turn_R::Clone(void* pArg)
{
	CState_Enemy_HunterDog_Walk_Turn_R* pInstance = new CState_Enemy_HunterDog_Walk_Turn_R(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_HunterDog_Walk_Turn_R");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_HunterDog_Walk_Turn_R::Free()
{
	__super::Free();
}
