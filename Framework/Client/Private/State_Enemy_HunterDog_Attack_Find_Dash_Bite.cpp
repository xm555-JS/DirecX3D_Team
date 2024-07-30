#include "stdafx.h"
#include "..\Public\State_Enemy_HunterDog_Attack_Find_Dash_Bite.h"
#include "GameInstance.h"
#include "GameObject.h"

CState_Enemy_HunterDog_Attack_Find_Dash_Bite::CState_Enemy_HunterDog_Attack_Find_Dash_Bite(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_HunterDog_Attack_Find_Dash_Bite::CState_Enemy_HunterDog_Attack_Find_Dash_Bite(const CState_Enemy_HunterDog_Attack_Find_Dash_Bite& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_HunterDog_Attack_Find_Dash_Bite::Initialize_Prototype()
{

	m_ReserveStates.push_back(STATE_ENEMY_HUNTERDOG_IDLE_NORMAL);

	m_ReserveProbabilitys[STATE_ENEMY_HUNTERDOG_IDLE_NORMAL] = 1;
	return S_OK;
}

HRESULT CState_Enemy_HunterDog_Attack_Find_Dash_Bite::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Enemy_HunterDog_Attack_Find_Dash_Bite::Enter(void* pArg)
{
}

void CState_Enemy_HunterDog_Attack_Find_Dash_Bite::Tick(_float fTimeDelta)
{

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_HunterDog_Attack_Find_Dash_Bite::Exit(void* pArg)
{

}

_bool CState_Enemy_HunterDog_Attack_Find_Dash_Bite::Check_Condition(CState_Handler* pHandler)
{

	return false;
}


CState_Enemy_HunterDog_Attack_Find_Dash_Bite* CState_Enemy_HunterDog_Attack_Find_Dash_Bite::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_HunterDog_Attack_Find_Dash_Bite* pInstance = new CState_Enemy_HunterDog_Attack_Find_Dash_Bite(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_HunterDog_Attack_Find_Dash_Bite");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_HunterDog_Attack_Find_Dash_Bite::Clone(void* pArg)
{
	CState_Enemy_HunterDog_Attack_Find_Dash_Bite* pInstance = new CState_Enemy_HunterDog_Attack_Find_Dash_Bite(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_HunterDog_Attack_Find_Dash_Bite");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_HunterDog_Attack_Find_Dash_Bite::Free()
{
	__super::Free();
}
