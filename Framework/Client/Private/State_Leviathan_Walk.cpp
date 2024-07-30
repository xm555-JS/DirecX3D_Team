#include "stdafx.h"
#include "..\Public\State_Leviathan_Walk.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

#include "Enemy_Boss_Dragon.h"

CState_Leviathan_Walk::CState_Leviathan_Walk(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Leviathan_Walk::CState_Leviathan_Walk(const CState_Leviathan_Walk& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Leviathan_Walk::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CState_Leviathan_Walk::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Leviathan_Walk::Enter(void* pArg)
{
	__super::Enter(pArg);

	//m_pOwnerTranformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-180.f));
}

void CState_Leviathan_Walk::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	//CState_AI::Tick(fTimeDelta);
	CState_Boss::Tick_AnimationLerp(fTimeDelta, "Bone_ROOT", m_pOwnerTranformCom, XMMatrixRotationY(XMConvertToRadians(0.0f)), XMConvertToRadians(0.f));
	//CState_Boss::Tick_AnimationFix(fTimeDelta, "Bone_ROOT", m_pOwnerTranformCom);
	//CState_AI::Tick(fTimeDelta);
}

void CState_Leviathan_Walk::Exit(void* pArg)
{
	__super::Exit(pArg);
}

_bool CState_Leviathan_Walk::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Leviathan_Walk* CState_Leviathan_Walk::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Leviathan_Walk* pInstance = new CState_Leviathan_Walk(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Leviathan_Walk");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Leviathan_Walk::Clone(void* pArg)
{
	CState_Leviathan_Walk* pInstance = new CState_Leviathan_Walk(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Leviathan_Walk");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Leviathan_Walk::Free()
{
	__super::Free();
}
