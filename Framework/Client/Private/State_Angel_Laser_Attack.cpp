#include "stdafx.h"
#include "..\Public\State_Angel_Laser_Attack.h"

CState_Angel_Laser_Attack::CState_Angel_Laser_Attack(CState_Manager* pManager, STATE_TYPE eState)
	:CState_AI(pManager, eState)
{
}

CState_Angel_Laser_Attack::CState_Angel_Laser_Attack(const CState_Angel_Laser_Attack& rhs)
	: CState_AI(rhs)
{
}

HRESULT CState_Angel_Laser_Attack::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CState_Angel_Laser_Attack::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Angel_Laser_Attack::Enter(void* pArg)
{
}

void CState_Angel_Laser_Attack::Tick(_float fTimeDelta)
{
}

void CState_Angel_Laser_Attack::Exit(void* pArg)
{
}

_bool CState_Angel_Laser_Attack::Check_Condition(CState_Handler* pHandler)
{
	return false;
}

CState_Angel_Laser_Attack* CState_Angel_Laser_Attack::Create(CState_Manager * pManager, STATE_TYPE eState)
{
	CState_Angel_Laser_Attack* pInstance = new CState_Angel_Laser_Attack(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Angel_Laser_Attack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Angel_Laser_Attack::Clone(void* pArg)
{
	CState_Angel_Laser_Attack* pInstance = new CState_Angel_Laser_Attack(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Angel_Laser_Attack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Angel_Laser_Attack::Free()
{
	__super::Free();
}
