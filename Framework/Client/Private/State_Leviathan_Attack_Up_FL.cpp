#include "stdafx.h"
#include "..\Public\State_Leviathan_Attack_Up_FL.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

#include "Enemy_Boss_Dragon.h"

CState_Leviathan_Attack_Up_FL::CState_Leviathan_Attack_Up_FL(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Leviathan_Attack_Up_FL::CState_Leviathan_Attack_Up_FL(const CState_Leviathan_Attack_Up_FL& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Leviathan_Attack_Up_FL::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CState_Leviathan_Attack_Up_FL::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Leviathan_Attack_Up_FL::Enter(void* pArg)
{
	__super::Enter(pArg);
}

void CState_Leviathan_Attack_Up_FL::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	CState_AI::Tick(fTimeDelta);
}

void CState_Leviathan_Attack_Up_FL::Exit(void* pArg)
{
	__super::Exit(pArg);
}

_bool CState_Leviathan_Attack_Up_FL::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Leviathan_Attack_Up_FL* CState_Leviathan_Attack_Up_FL::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Leviathan_Attack_Up_FL* pInstance = new CState_Leviathan_Attack_Up_FL(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Leviathan_Attack_Up_FL");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Leviathan_Attack_Up_FL::Clone(void* pArg)
{
	CState_Leviathan_Attack_Up_FL* pInstance = new CState_Leviathan_Attack_Up_FL(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Leviathan_Attack_Up_FL");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Leviathan_Attack_Up_FL::Free()
{
	__super::Free();
}
