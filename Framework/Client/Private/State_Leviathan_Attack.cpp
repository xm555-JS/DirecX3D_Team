#include "stdafx.h"
#include "..\Public\State_Leviathan_Attack.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

#include "Enemy_Boss_Dragon.h"

CState_Leviathan_Attack::CState_Leviathan_Attack(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Leviathan_Attack::CState_Leviathan_Attack(const CState_Leviathan_Attack& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Leviathan_Attack::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CState_Leviathan_Attack::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Leviathan_Attack::Enter(void* pArg)
{
	__super::Enter(pArg);
}

void CState_Leviathan_Attack::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	CState_AI::Tick(fTimeDelta);
}

void CState_Leviathan_Attack::Exit(void* pArg)
{
	__super::Exit(pArg);
}

_bool CState_Leviathan_Attack::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Leviathan_Attack* CState_Leviathan_Attack::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Leviathan_Attack* pInstance = new CState_Leviathan_Attack(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Leviathan_Attack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Leviathan_Attack::Clone(void* pArg)
{
	CState_Leviathan_Attack* pInstance = new CState_Leviathan_Attack(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Leviathan_Attack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Leviathan_Attack::Free()
{
	__super::Free();
}
