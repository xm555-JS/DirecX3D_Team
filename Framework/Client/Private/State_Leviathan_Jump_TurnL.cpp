#include "stdafx.h"
#include "..\Public\State_Leviathan_Jump_TurnL.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

#include "Enemy_Boss_Dragon.h"

CState_Leviathan_Jump_TurnL::CState_Leviathan_Jump_TurnL(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Leviathan_Jump_TurnL::CState_Leviathan_Jump_TurnL(const CState_Leviathan_Jump_TurnL& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Leviathan_Jump_TurnL::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CState_Leviathan_Jump_TurnL::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Leviathan_Jump_TurnL::Enter(void* pArg)
{
	__super::Enter(pArg);
}

void CState_Leviathan_Jump_TurnL::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	CState_AI::Tick(fTimeDelta);
}

void CState_Leviathan_Jump_TurnL::Exit(void* pArg)
{
	__super::Exit(pArg);
}

_bool CState_Leviathan_Jump_TurnL::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Leviathan_Jump_TurnL* CState_Leviathan_Jump_TurnL::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Leviathan_Jump_TurnL* pInstance = new CState_Leviathan_Jump_TurnL(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Leviathan_Jump_TurnL");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Leviathan_Jump_TurnL::Clone(void* pArg)
{
	CState_Leviathan_Jump_TurnL* pInstance = new CState_Leviathan_Jump_TurnL(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Leviathan_Jump_TurnL");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Leviathan_Jump_TurnL::Free()
{
	__super::Free();
}
