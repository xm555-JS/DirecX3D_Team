#include "stdafx.h"
#include "..\Public\State_Angel_Dead.h"
#include "GameObject.h"
#include "GameObject.h"
#include "Unit.h"

CState_Angel_Dead::CState_Angel_Dead(CState_Manager* pManager, STATE_TYPE eState)
	:CState_AI(pManager, eState)
{
}

CState_Angel_Dead::CState_Angel_Dead(const CState_Angel_Dead& rhs)
	: CState_AI(rhs)
{
}

HRESULT CState_Angel_Dead::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CState_Angel_Dead::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Angel_Dead::Enter(void* pArg)
{
}

void CState_Angel_Dead::Tick(_float fTimeDelta)
{
}

void CState_Angel_Dead::Exit(void* pArg)
{
}

_bool CState_Angel_Dead::Check_Condition(CState_Handler* pHandler)
{
	if (static_cast<CUnit*>(m_pState_Handler->Get_Owner())->Get_UnitCurInfo()->fHP < 0.f)
		return true;

	return false;
}

CState_Angel_Dead* CState_Angel_Dead::Create(CState_Manager * pManager, STATE_TYPE eState)
{
	CState_Angel_Dead* pInstance = new CState_Angel_Dead(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Angel_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Angel_Dead::Clone(void* pArg)
{
	CState_Angel_Dead* pInstance = new CState_Angel_Dead(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Angel_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Angel_Dead::Free()
{
	__super::Free();
}
