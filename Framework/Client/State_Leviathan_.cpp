#include "stdafx.h"
#include "..\Public\State_Leviathan_Roar_0.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

#include "Enemy_Boss_Dragon.h"

#include "Easing_Utillity.h"
#include "Imgui_Manager.h"

CState_Leviathan_Roar_0::CState_Leviathan_Roar_0(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Leviathan(pManager, eState)
{
}

CState_Leviathan_Roar_0::CState_Leviathan_Roar_0(const CState_Leviathan_Roar_0& rhs)
	: CState_Leviathan(rhs)
{
}

HRESULT CState_Leviathan_Roar_0::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_LEVIATHAN_WALK);
	m_ReserveProbabilitys[STATE_BOSS_LEVIATHAN_WALK] = 1;

	return S_OK;
}

HRESULT CState_Leviathan_Roar_0::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Leviathan_Roar_0::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 2.f;

	m_pTargetModel->Set_ModelEasingRatio(1.f);

}

void CState_Leviathan_Roar_0::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;


	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;


	CState_Boss::Tick_AnimationLerp(fTimeDelta, "Bone_ROOT", m_pOwnerTranformCom, XMMatrixRotationY(XMConvertToRadians(0.0f)));
}

void CState_Leviathan_Roar_0::Exit(void* pArg)
{
	__super::Exit(pArg);

	m_fAnimSpeed = 1.f;
	m_pTargetModel->Set_ModelEasingRatio(0.2f);

	//m_pOwnerTranformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-180.f));
}

_bool CState_Leviathan_Roar_0::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Leviathan_Roar_0* CState_Leviathan_Roar_0::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Leviathan_Roar_0* pInstance = new CState_Leviathan_Roar_0(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Leviathan_Roar_0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Leviathan_Roar_0::Clone(void* pArg)
{
	CState_Leviathan_Roar_0* pInstance = new CState_Leviathan_Roar_0(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Leviathan_Roar_0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Leviathan_Roar_0::Free()
{
	__super::Free();
}
