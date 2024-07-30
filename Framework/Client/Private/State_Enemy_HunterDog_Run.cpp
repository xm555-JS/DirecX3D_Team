#include "stdafx.h"
#include "..\Public\State_Enemy_HunterDog_Run.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CState_Enemy_HunterDog_Run::CState_Enemy_HunterDog_Run(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_HunterDog_Run::CState_Enemy_HunterDog_Run(const CState_Enemy_HunterDog_Run& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_HunterDog_Run::Initialize_Prototype()
{

	m_ReserveStates.push_back(STATE_ENEMY_HUNTERDOG_IDLE_BATTLE);

	m_ReserveProbabilitys[STATE_ENEMY_HUNTERDOG_IDLE_BATTLE] = 1;
	return S_OK;
}

HRESULT CState_Enemy_HunterDog_Run::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Enemy_HunterDog_Run::Enter(void* pArg)
{

}

void CState_Enemy_HunterDog_Run::Tick(_float fTimeDelta)
{


	CState_AI::Tick(fTimeDelta);
	
	
}

void CState_Enemy_HunterDog_Run::Exit(void* pArg)
{

}

_bool CState_Enemy_HunterDog_Run::Check_Condition(CState_Handler* pHandler)
{	

	return false;
}


CState_Enemy_HunterDog_Run* CState_Enemy_HunterDog_Run::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_HunterDog_Run* pInstance = new CState_Enemy_HunterDog_Run(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_HunterDog_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_HunterDog_Run::Clone(void* pArg)
{
	CState_Enemy_HunterDog_Run* pInstance = new CState_Enemy_HunterDog_Run(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_HunterDog_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_HunterDog_Run::Free()
{
	__super::Free();
}
