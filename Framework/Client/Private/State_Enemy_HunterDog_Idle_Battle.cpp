#include "stdafx.h"
#include "..\Public\State_Enemy_HunterDog_Idle_Battle.h"
#include "GameInstance.h"
#include "GameObject.h"

CState_Enemy_HunterDog_Idle_Battle::CState_Enemy_HunterDog_Idle_Battle(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_HunterDog_Idle_Battle::CState_Enemy_HunterDog_Idle_Battle(const CState_Enemy_HunterDog_Idle_Battle& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_HunterDog_Idle_Battle::Initialize_Prototype()//플레이어가 근처에 있는 상태에서 갈수있는 모션
{


	return S_OK;
}

HRESULT CState_Enemy_HunterDog_Idle_Battle::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Enemy_HunterDog_Idle_Battle::Enter(void* pArg)
{


}

void CState_Enemy_HunterDog_Idle_Battle::Tick(_float fTimeDelta)
{

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_HunterDog_Idle_Battle::Exit(void* pArg)
{

}

_bool CState_Enemy_HunterDog_Idle_Battle::Check_Condition(CState_Handler* pHandler)
{

	return false;
}


CState_Enemy_HunterDog_Idle_Battle* CState_Enemy_HunterDog_Idle_Battle::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_HunterDog_Idle_Battle* pInstance = new CState_Enemy_HunterDog_Idle_Battle(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_HunterDog_Idle_Battle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_HunterDog_Idle_Battle::Clone(void* pArg)
{
	CState_Enemy_HunterDog_Idle_Battle* pInstance = new CState_Enemy_HunterDog_Idle_Battle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_HunterDog_Idle_Battle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_HunterDog_Idle_Battle::Free()
{
	__super::Free();
}
