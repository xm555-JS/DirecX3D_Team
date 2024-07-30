#include "stdafx.h"
#include "..\Public\State_Enemy_Habaka_Admiral_Dead_End.h"
#include "GameInstance.h"
CState_Enemy_Habaka_Admiral_Dead_End::CState_Enemy_Habaka_Admiral_Dead_End(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_Habaka_Admiral_Dead_End::CState_Enemy_Habaka_Admiral_Dead_End(const CState_Enemy_Habaka_Admiral_Dead_End& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_Habaka_Admiral_Dead_End::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CState_Enemy_Habaka_Admiral_Dead_End::Initialize(void* pArg)
{

	return S_OK;
}

void CState_Enemy_Habaka_Admiral_Dead_End::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 1.f;

}

void CState_Enemy_Habaka_Admiral_Dead_End::Tick(_float fTimeDelta)
{


	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_Habaka_Admiral_Dead_End::Exit(void* pArg)
{

}

_bool CState_Enemy_Habaka_Admiral_Dead_End::Check_Condition(CState_Handler* pHandler)
{


	return Is_OwnerDead(pHandler);
}


CState_Enemy_Habaka_Admiral_Dead_End* CState_Enemy_Habaka_Admiral_Dead_End::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_Habaka_Admiral_Dead_End* pInstance = new CState_Enemy_Habaka_Admiral_Dead_End(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_Habaka_Admiral_Dead_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_Habaka_Admiral_Dead_End::Clone(void* pArg)
{
	CState_Enemy_Habaka_Admiral_Dead_End* pInstance = new CState_Enemy_Habaka_Admiral_Dead_End(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_Habaka_Admiral_Dead_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_Habaka_Admiral_Dead_End::Free()
{
	__super::Free();
}
