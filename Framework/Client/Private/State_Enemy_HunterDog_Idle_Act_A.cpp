#include "stdafx.h"
#include "..\Public\State_Enemy_HunterDog_Idle_Act_A.h"
#include "GameInstance.h"
#include "GameObject.h"

CState_Enemy_HunterDog_Idle_Act_A::CState_Enemy_HunterDog_Idle_Act_A(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_HunterDog_Idle_Act_A::CState_Enemy_HunterDog_Idle_Act_A(const CState_Enemy_HunterDog_Idle_Act_A& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_HunterDog_Idle_Act_A::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_ENEMY_HUNTERDOG_IDLE_NORMAL);

	return S_OK;
}

HRESULT CState_Enemy_HunterDog_Idle_Act_A::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Enemy_HunterDog_Idle_Act_A::Enter(void* pArg)
{

}

void CState_Enemy_HunterDog_Idle_Act_A::Tick(_float fTimeDelta)
{

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_HunterDog_Idle_Act_A::Exit(void* pArg)
{

}

_bool CState_Enemy_HunterDog_Idle_Act_A::Check_Condition(CState_Handler* pHandler)
{


	return false;
}


CState_Enemy_HunterDog_Idle_Act_A* CState_Enemy_HunterDog_Idle_Act_A::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_HunterDog_Idle_Act_A* pInstance = new CState_Enemy_HunterDog_Idle_Act_A(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_HunterDog_Idle_Act_A");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_HunterDog_Idle_Act_A::Clone(void* pArg)
{
	CState_Enemy_HunterDog_Idle_Act_A* pInstance = new CState_Enemy_HunterDog_Idle_Act_A(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_HunterDog_Idle_Act_A");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_HunterDog_Idle_Act_A::Free()
{
	__super::Free();
}
