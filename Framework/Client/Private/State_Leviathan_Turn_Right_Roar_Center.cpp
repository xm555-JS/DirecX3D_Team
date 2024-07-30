#include "stdafx.h"
#include "..\Public\State_Leviathan_Turn_Right_Roar_Center.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

#include "Enemy_Boss_Dragon.h"

#include "Easing_Utillity.h"
#include "Imgui_Manager.h"

CState_Leviathan_Turn_Right_Roar_Center::CState_Leviathan_Turn_Right_Roar_Center(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Leviathan(pManager, eState)
{
}

CState_Leviathan_Turn_Right_Roar_Center::CState_Leviathan_Turn_Right_Roar_Center(const CState_Leviathan_Turn_Right_Roar_Center& rhs)
	: CState_Leviathan(rhs)
{
}

HRESULT CState_Leviathan_Turn_Right_Roar_Center::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_LEVIATHAN_LOOK_CENTER_ROAR);
	m_ReserveProbabilitys[STATE_BOSS_LEVIATHAN_LOOK_CENTER_ROAR] = 1;

	return S_OK;
}

HRESULT CState_Leviathan_Turn_Right_Roar_Center::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Leviathan_Turn_Right_Roar_Center::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 2.f;

	m_pTargetModel->Set_ModelEasingRatio(0.2f);

	//_vector vCenterPos = XMVectorSet(256.f, 0.f, 256.f, 1.f);
	//_vector vPos = m_pOwner->Get_Pos();
	//_vector vEndLook = XMVector3Normalize(vCenterPos - vPos);
	//_vector vStartLook = XMVector3Normalize(m_pOwnerTranformCom->Get_State(CTransform::STATE_LOOK));

	//XMStoreFloat4(&m_vStartLook, vStartLook);
	//XMStoreFloat4(&m_vEndLook, vEndLook);

	//m_fAngle = XMVectorGetX(XMVector3AngleBetweenVectors(vStartLook, vEndLook));

}

void CState_Leviathan_Turn_Right_Roar_Center::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;


	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	_uint iEndKeyFrame = 565;
	if (iEndKeyFrame < Get_CurKeyFrame())
		m_bEndCurState = true;



	//_vector vStartLook = XMLoadFloat4(&m_vStartLook);
	//_vector vEndLook = XMLoadFloat4(&m_vEndLook);
	//_vector vCurLook = CEasing_Utillity::sinfInOut(vStartLook, vEndLook, Get_CurKeyFrame(), iEndKeyFrame);

	//m_pOwnerTranformCom->LookTo(vCurLook);

	CState_Boss::Tick_AnimationLerp(fTimeDelta, "Bone_ROOT", m_pOwnerTranformCom, XMMatrixRotationY(XMConvertToRadians(180.f)));
}

void CState_Leviathan_Turn_Right_Roar_Center::Exit(void* pArg)
{
	__super::Exit(pArg);

	m_fAnimSpeed = 1.f;
	m_pTargetModel->Set_ModelEasingRatio(0.2f);

}

_bool CState_Leviathan_Turn_Right_Roar_Center::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Leviathan_Turn_Right_Roar_Center* CState_Leviathan_Turn_Right_Roar_Center::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Leviathan_Turn_Right_Roar_Center* pInstance = new CState_Leviathan_Turn_Right_Roar_Center(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Leviathan_Turn_Right_Roar_Center");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Leviathan_Turn_Right_Roar_Center::Clone(void* pArg)
{
	CState_Leviathan_Turn_Right_Roar_Center* pInstance = new CState_Leviathan_Turn_Right_Roar_Center(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Leviathan_Turn_Right_Roar_Center");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Leviathan_Turn_Right_Roar_Center::Free()
{
	__super::Free();
}
