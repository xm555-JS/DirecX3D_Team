#include "stdafx.h"
#include "..\Public\State_Leviathan_Attack_Down_F_A.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

#include "Enemy_Boss_Dragon.h"

CState_Leviathan_Attack_Down_F_A::CState_Leviathan_Attack_Down_F_A(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Leviathan_Attack_Down_F_A::CState_Leviathan_Attack_Down_F_A(const CState_Leviathan_Attack_Down_F_A& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Leviathan_Attack_Down_F_A::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CState_Leviathan_Attack_Down_F_A::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Leviathan_Attack_Down_F_A::Enter(void* pArg)
{
	__super::Enter(pArg);
}

void CState_Leviathan_Attack_Down_F_A::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	CState_AI::Tick(fTimeDelta);
}

void CState_Leviathan_Attack_Down_F_A::Exit(void* pArg)
{
	__super::Exit(pArg);
}

_bool CState_Leviathan_Attack_Down_F_A::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Leviathan_Attack_Down_F_A* CState_Leviathan_Attack_Down_F_A::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Leviathan_Attack_Down_F_A* pInstance = new CState_Leviathan_Attack_Down_F_A(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Leviathan_Attack_Down_F_A");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Leviathan_Attack_Down_F_A::Clone(void* pArg)
{
	CState_Leviathan_Attack_Down_F_A* pInstance = new CState_Leviathan_Attack_Down_F_A(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Leviathan_Attack_Down_F_A");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Leviathan_Attack_Down_F_A::Free()
{
	__super::Free();
}
