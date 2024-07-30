#include "stdafx.h"
#include "..\Public\State_Enemy_HunterDog_Walk.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CState_Enemy_HunterDog_Walk::CState_Enemy_HunterDog_Walk(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_HunterDog_Walk::CState_Enemy_HunterDog_Walk(const CState_Enemy_HunterDog_Walk& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_HunterDog_Walk::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CState_Enemy_HunterDog_Walk::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Enemy_HunterDog_Walk::Enter(void* pArg)
{


}

void CState_Enemy_HunterDog_Walk::Tick(_float fTimeDelta)
{

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_HunterDog_Walk::Exit(void* pArg)
{

}

_bool CState_Enemy_HunterDog_Walk::Check_Condition(CState_Handler* pHandler)
{

	return false;
}


CState_Enemy_HunterDog_Walk* CState_Enemy_HunterDog_Walk::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_HunterDog_Walk* pInstance = new CState_Enemy_HunterDog_Walk(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_HunterDog_Walk");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_HunterDog_Walk::Clone(void* pArg)
{
	CState_Enemy_HunterDog_Walk* pInstance = new CState_Enemy_HunterDog_Walk(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_HunterDog_Walk");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_HunterDog_Walk::Free()
{
	__super::Free();
}
