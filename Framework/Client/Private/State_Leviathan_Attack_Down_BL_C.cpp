#include "stdafx.h"
#include "..\Public\State_Leviathan_Attack_Down_BL_C.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

#include "Enemy_Boss_Dragon.h"

CState_Leviathan_Attack_Down_BL_C::CState_Leviathan_Attack_Down_BL_C(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Leviathan_Attack_Down_BL_C::CState_Leviathan_Attack_Down_BL_C(const CState_Leviathan_Attack_Down_BL_C& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Leviathan_Attack_Down_BL_C::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CState_Leviathan_Attack_Down_BL_C::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Leviathan_Attack_Down_BL_C::Enter(void* pArg)
{
	__super::Enter(pArg);
}

void CState_Leviathan_Attack_Down_BL_C::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	CState_AI::Tick(fTimeDelta);
}

void CState_Leviathan_Attack_Down_BL_C::Exit(void* pArg)
{
	__super::Exit(pArg);
}

_bool CState_Leviathan_Attack_Down_BL_C::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Leviathan_Attack_Down_BL_C* CState_Leviathan_Attack_Down_BL_C::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Leviathan_Attack_Down_BL_C* pInstance = new CState_Leviathan_Attack_Down_BL_C(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Leviathan_Attack_Down_BL_C");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Leviathan_Attack_Down_BL_C::Clone(void* pArg)
{
	CState_Leviathan_Attack_Down_BL_C* pInstance = new CState_Leviathan_Attack_Down_BL_C(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Leviathan_Attack_Down_BL_C");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Leviathan_Attack_Down_BL_C::Free()
{
	__super::Free();
}
