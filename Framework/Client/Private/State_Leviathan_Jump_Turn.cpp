#include "stdafx.h"
#include "..\Public\State_Leviathan_Jump_Turn.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

#include "Enemy_Boss_Dragon.h"

#include "Easing_Utillity.h"
#include "Imgui_Manager.h"

CState_Leviathan_Jump_Turn::CState_Leviathan_Jump_Turn(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Leviathan(pManager, eState)
{
}

CState_Leviathan_Jump_Turn::CState_Leviathan_Jump_Turn(const CState_Leviathan_Jump_Turn& rhs)
	: CState_Leviathan(rhs)
{
}

HRESULT CState_Leviathan_Jump_Turn::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_LEVIATHAN_DEAD);
	m_ForceStates.push_back(STATE_BOSS_LEVIATHAN_WEAK);

	m_ReserveStates.push_back(STATE_BOSS_LEVIATHAN_LOOK_CENTER_ROAR);
	m_ReserveProbabilitys[STATE_BOSS_LEVIATHAN_LOOK_CENTER_ROAR] = 1;

	return S_OK;
}

HRESULT CState_Leviathan_Jump_Turn::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Leviathan_Jump_Turn::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 1.f;
	m_fAccTime = 0.f;
	m_pTargetModel->Set_ModelEasingRatio(0.1f);

}

void CState_Leviathan_Jump_Turn::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	m_fAccTime += fTimeDelta;

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	//_float fMaxSpeed = 150.f;
	//_float fMoveTime = 5.f;
	//_float fMoveSpeed = XMVectorGetX(CEasing_Utillity::sinfInOut(XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(fMaxSpeed, 0.f, 0.f, 0.f), m_fAccTime, fMoveTime * 0.5f));
	//m_pOwnerTranformCom->Go_Front(fTimeDelta, fMoveSpeed);

	if (300 < Get_CurKeyFrame())
	{
		m_bEndCurState = true;
	}

	//if (m_fAccTime > fMoveTime)
	//	m_bEndCurState = true;

	//_float fMoveLegthY;
	//_uint iMinFrame = 100;
	//_uint iMaxFrame = 180;
	//_float fMovePower = 3.f;
	//if (iMinFrame < Get_CurKeyFrame() && iMaxFrame > Get_CurKeyFrame())
	//{
	//	fMoveLegthY = XMVectorGetX(CEasing_Utillity::sinfOut(XMVectorSet(80.f, 0.f, 0.f, 0.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), Get_CurKeyFrame() - iMinFrame, iMaxFrame - iMinFrame));
	//	m_pOwnerTranformCom->Go_Up(fTimeDelta, fMoveLegthY * fMovePower);
	//}


	CState_Boss::Tick_AnimationLerp(fTimeDelta, "Bone_ROOT", m_pOwnerTranformCom, XMMatrixRotationY(XMConvertToRadians(180.0f)));
}

void CState_Leviathan_Jump_Turn::Exit(void* pArg)
{
	__super::Exit(pArg);

	m_fAnimSpeed = 1.f;
	m_pTargetModel->Set_ModelEasingRatio(0.2f);
}

_bool CState_Leviathan_Jump_Turn::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Leviathan_Jump_Turn* CState_Leviathan_Jump_Turn::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Leviathan_Jump_Turn* pInstance = new CState_Leviathan_Jump_Turn(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Leviathan_Jump_Turn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Leviathan_Jump_Turn::Clone(void* pArg)
{
	CState_Leviathan_Jump_Turn* pInstance = new CState_Leviathan_Jump_Turn(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Leviathan_Jump_Turn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Leviathan_Jump_Turn::Free()
{
	__super::Free();
}
