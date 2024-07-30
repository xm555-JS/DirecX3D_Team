#include "stdafx.h"
#include "..\Public\State_Leviathan_Jump_TurnR.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

#include "Enemy_Boss_Dragon.h"

CState_Leviathan_Jump_TurnR::CState_Leviathan_Jump_TurnR(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Leviathan_Jump_TurnR::CState_Leviathan_Jump_TurnR(const CState_Leviathan_Jump_TurnR& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Leviathan_Jump_TurnR::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CState_Leviathan_Jump_TurnR::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Leviathan_Jump_TurnR::Enter(void* pArg)
{
	__super::Enter(pArg);
}

void CState_Leviathan_Jump_TurnR::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	CState_AI::Tick(fTimeDelta);
}

void CState_Leviathan_Jump_TurnR::Exit(void* pArg)
{
	__super::Exit(pArg);
}

_bool CState_Leviathan_Jump_TurnR::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Leviathan_Jump_TurnR* CState_Leviathan_Jump_TurnR::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Leviathan_Jump_TurnR* pInstance = new CState_Leviathan_Jump_TurnR(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Leviathan_Jump_TurnR");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Leviathan_Jump_TurnR::Clone(void* pArg)
{
	CState_Leviathan_Jump_TurnR* pInstance = new CState_Leviathan_Jump_TurnR(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Leviathan_Jump_TurnR");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Leviathan_Jump_TurnR::Free()
{
	__super::Free();
}
