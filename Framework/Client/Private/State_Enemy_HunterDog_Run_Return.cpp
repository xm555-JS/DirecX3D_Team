#include "stdafx.h"
#include "..\Public\State_Enemy_HunterDog_Run_Return.h"
#include "GameInstance.h"
#include "GameObject.h"

CState_Enemy_HunterDog_Run_Return::CState_Enemy_HunterDog_Run_Return(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_HunterDog_Run_Return::CState_Enemy_HunterDog_Run_Return(const CState_Enemy_HunterDog_Run_Return& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_HunterDog_Run_Return::Initialize_Prototype()
{

	m_ReserveStates.push_back(STATE_ENEMY_HUNTERDOG_IDLE_NORMAL);
	m_ReserveStates.push_back(STATE_ENEMY_HUNTERDOG_IDLE_BATTLE);

	return S_OK;
}

HRESULT CState_Enemy_HunterDog_Run_Return::Initialize(void * pArg)
{

	return S_OK;
}

void CState_Enemy_HunterDog_Run_Return::Enter(void* pArg)
{

}

void CState_Enemy_HunterDog_Run_Return::Tick(_float fTimeDelta)
{


	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_HunterDog_Run_Return::Exit(void* pArg)
{

}

_bool CState_Enemy_HunterDog_Run_Return::Check_Condition(CState_Handler* pHandler)
{


	return false;
}


CState_Enemy_HunterDog_Run_Return* CState_Enemy_HunterDog_Run_Return::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_HunterDog_Run_Return* pInstance = new CState_Enemy_HunterDog_Run_Return(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_HunterDog_Run_Return");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_HunterDog_Run_Return::Clone(void* pArg)
{
	CState_Enemy_HunterDog_Run_Return* pInstance = new CState_Enemy_HunterDog_Run_Return(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_HunterDog_Run_Return");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_HunterDog_Run_Return::Free()
{
	__super::Free();
}
