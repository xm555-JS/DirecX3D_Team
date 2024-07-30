#include "stdafx.h"
#include "..\Public\State_Leviathan_Attack_Up_BR.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

#include "Enemy_Boss_Dragon.h"

CState_Leviathan_Attack_Up_BR::CState_Leviathan_Attack_Up_BR(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Leviathan_Attack_Up_BR::CState_Leviathan_Attack_Up_BR(const CState_Leviathan_Attack_Up_BR& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Leviathan_Attack_Up_BR::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CState_Leviathan_Attack_Up_BR::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Leviathan_Attack_Up_BR::Enter(void* pArg)
{
	__super::Enter(pArg);
}

void CState_Leviathan_Attack_Up_BR::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	CState_AI::Tick(fTimeDelta);
}

void CState_Leviathan_Attack_Up_BR::Exit(void* pArg)
{
	__super::Exit(pArg);
}

_bool CState_Leviathan_Attack_Up_BR::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Leviathan_Attack_Up_BR* CState_Leviathan_Attack_Up_BR::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Leviathan_Attack_Up_BR* pInstance = new CState_Leviathan_Attack_Up_BR(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Leviathan_Attack_Up_BR");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Leviathan_Attack_Up_BR::Clone(void* pArg)
{
	CState_Leviathan_Attack_Up_BR* pInstance = new CState_Leviathan_Attack_Up_BR(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Leviathan_Attack_Up_BR");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Leviathan_Attack_Up_BR::Free()
{
	__super::Free();
}
