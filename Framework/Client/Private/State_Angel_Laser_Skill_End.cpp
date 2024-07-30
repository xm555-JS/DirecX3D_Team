#include "stdafx.h"
#include "..\Public\State_Angel_Laser_Skill_End.h"

CState_Angel_Laser_Skill_End::CState_Angel_Laser_Skill_End(CState_Manager* pManager, STATE_TYPE eState)
	:CState_AI(pManager, eState)
{
}

CState_Angel_Laser_Skill_End::CState_Angel_Laser_Skill_End(const CState_Angel_Laser_Skill_End& rhs)
	: CState_AI(rhs)
{
}

HRESULT CState_Angel_Laser_Skill_End::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CState_Angel_Laser_Skill_End::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Angel_Laser_Skill_End::Enter(void* pArg)
{
}

void CState_Angel_Laser_Skill_End::Tick(_float fTimeDelta)
{
}

void CState_Angel_Laser_Skill_End::Exit(void* pArg)
{
}

_bool CState_Angel_Laser_Skill_End::Check_Condition(CState_Handler* pHandler)
{
	return false;
}

CState_Angel_Laser_Skill_End* CState_Angel_Laser_Skill_End::Create(CState_Manager * pManager, STATE_TYPE eState)
{
	CState_Angel_Laser_Skill_End* pInstance = new CState_Angel_Laser_Skill_End(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Angel_Laser_Skill_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Angel_Laser_Skill_End::Clone(void* pArg)
{
	CState_Angel_Laser_Skill_End* pInstance = new CState_Angel_Laser_Skill_End(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Angel_Laser_Skill_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Angel_Laser_Skill_End::Free()
{
	__super::Free();
}
