#include "stdafx.h"
#include "..\Public\State_Leviathan_Attack_Up_F.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

#include "Enemy_Boss_Dragon.h"

CState_Leviathan_Attack_Up_F::CState_Leviathan_Attack_Up_F(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Leviathan_Attack_Up_F::CState_Leviathan_Attack_Up_F(const CState_Leviathan_Attack_Up_F& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Leviathan_Attack_Up_F::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CState_Leviathan_Attack_Up_F::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Leviathan_Attack_Up_F::Enter(void* pArg)
{
	__super::Enter(pArg);
}

void CState_Leviathan_Attack_Up_F::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	CState_AI::Tick(fTimeDelta);
}

void CState_Leviathan_Attack_Up_F::Exit(void* pArg)
{
	__super::Exit(pArg);
}

_bool CState_Leviathan_Attack_Up_F::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Leviathan_Attack_Up_F* CState_Leviathan_Attack_Up_F::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Leviathan_Attack_Up_F* pInstance = new CState_Leviathan_Attack_Up_F(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Leviathan_Attack_Up_F");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Leviathan_Attack_Up_F::Clone(void* pArg)
{
	CState_Leviathan_Attack_Up_F* pInstance = new CState_Leviathan_Attack_Up_F(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Leviathan_Attack_Up_F");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Leviathan_Attack_Up_F::Free()
{
	__super::Free();
}
