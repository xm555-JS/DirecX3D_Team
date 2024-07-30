#include "stdafx.h"
#include "..\Public\State_Enemy_Gangster_Cap_Dance.h"
#include "GameInstance.h"
#include "Player.h"
#include "Enemy.h"
CState_Enemy_Gangster_Cap_Dance::CState_Enemy_Gangster_Cap_Dance(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_Gangster_Cap_Dance::CState_Enemy_Gangster_Cap_Dance(const CState_Enemy_Gangster_Cap_Dance& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_Gangster_Cap_Dance::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CState_Enemy_Gangster_Cap_Dance::Initialize(void * pArg)
{

	return S_OK;
}

void CState_Enemy_Gangster_Cap_Dance::Enter(void* pArg)
{

}

void CState_Enemy_Gangster_Cap_Dance::Tick(_float fTimeDelta)
{
	


	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_Gangster_Cap_Dance::Exit(void* pArg)
{

}

_bool CState_Enemy_Gangster_Cap_Dance::Check_Condition(CState_Handler* pHandler)
{

	return false;
}


CState_Enemy_Gangster_Cap_Dance* CState_Enemy_Gangster_Cap_Dance::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_Gangster_Cap_Dance* pInstance = new CState_Enemy_Gangster_Cap_Dance(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_Gangster_Cap_Dance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_Gangster_Cap_Dance::Clone(void* pArg)
{
	CState_Enemy_Gangster_Cap_Dance* pInstance = new CState_Enemy_Gangster_Cap_Dance(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_Gangster_Cap_Dance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_Gangster_Cap_Dance::Free()
{
	__super::Free();
}
