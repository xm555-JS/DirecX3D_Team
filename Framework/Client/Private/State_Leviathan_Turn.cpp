#include "stdafx.h"
#include "..\Public\State_Leviathan_Turn.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

#include "Enemy_Boss_Leviathan.h"

#include "Easing_Utillity.h"
#include "Imgui_Manager.h"

CState_Leviathan_Turn::CState_Leviathan_Turn(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Leviathan(pManager, eState)
{
}

CState_Leviathan_Turn::CState_Leviathan_Turn(const CState_Leviathan_Turn& rhs)
	: CState_Leviathan(rhs)
{
}

HRESULT CState_Leviathan_Turn::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_LEVIATHAN_LOOK_OUT_ROAR);
	m_ReserveProbabilitys[STATE_BOSS_LEVIATHAN_LOOK_OUT_ROAR] = 1;

	//m_ReserveStates.push_back(STATE_BOSS_LEVIATHAN_ROAR_0);
	//m_ReserveProbabilitys[STATE_BOSS_LEVIATHAN_ROAR_0] = 1;
	

	return S_OK;
}

HRESULT CState_Leviathan_Turn::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Leviathan_Turn::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 1.f;
	m_fAccTime = 0.f;
	m_pTargetModel->Set_ModelEasingRatio(0.1f);
	m_bSetupEnterState = false;
}

void CState_Leviathan_Turn::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	//m_fAccTime += fTimeDelta;

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	//_float fMaxSpeed = 300.f;
	//_float fMoveTime = 3.f;
	//_float fMoveSpeed = XMVectorGetX(CEasing_Utillity::sinfOut(XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(fMaxSpeed, 0.f, 0.f, 0.f), m_fAccTime, fMoveTime * 0.5f));
	//m_pOwnerTranformCom->Go_Front(fTimeDelta, fMoveSpeed);

	if (300 < Get_CurKeyFrame())
	{
		m_bEndCurState = true;
	}

	//if(m_fAccTime > fMoveTime)
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

	if (m_bSetupEnterState)
	{
		_vector vStartLook = XMLoadFloat4(&m_vStartLook);
		_vector vEndLook = XMLoadFloat4(&m_vEndLook);

		_vector vCurLook = m_pOwnerTranformCom->Get_State(CTransform::STATE_LOOK);
		vCurLook = CEasing_Utillity::sinfInOut(vStartLook, vEndLook, Get_CurKeyFrame() - m_iStartKeyFrame, 300);


		m_pOwnerTranformCom->LookTo(vCurLook);
	}

	CState_Boss::Tick_AnimationLerp(fTimeDelta, "Bone_ROOT", m_pOwnerTranformCom, XMMatrixRotationY(XMConvertToRadians(180.0f)));

	if (m_pTargetModel->Get_Transform_Turn_End() && false == m_bSetupEnterState)
		SetUp_EnterInfo();
}

void CState_Leviathan_Turn::Exit(void* pArg)
{
	__super::Exit(pArg);

	m_fAnimSpeed = 1.f;
	m_pTargetModel->Set_ModelEasingRatio(0.2f);

	//m_pOwnerTranformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-180.f));
}

_bool CState_Leviathan_Turn::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

void CState_Leviathan_Turn::SetUp_EnterInfo()
{
	_uint	iCurIslandIdx = ((CEnemy_Boss_Leviathan*)m_pOwner)->Get_NextIslandIdx();

	_vector vStartPos = m_pOwner->Get_Pos();
	_vector vEndPos = ((CEnemy_Boss_Leviathan*)m_pOwner)->Get_IslandPos(iCurIslandIdx);

	_vector vStartLook = m_pOwnerTranformCom->Get_State(CTransform::STATE_LOOK);
	_vector vEndLook = XMVector3Normalize(vEndPos - vStartPos);

	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector vStartRight = XMVector3Cross(vUp, vStartLook);
	vStartLook = XMVector3Cross(vStartRight, vUp);

	_vector vEndRight = XMVector3Cross(vUp, vEndLook);
	vEndLook = XMVector3Cross(vEndRight, vUp);

	m_fStartHight = XMVectorGetY(vStartPos);

	XMStoreFloat4(&m_vStartPos, vStartPos);
	XMStoreFloat4(&m_vEndPos, vEndPos);

	XMStoreFloat4(&m_vStartLook, vStartLook);
	XMStoreFloat4(&m_vEndLook, vEndLook);

	m_iStartKeyFrame = Get_CurKeyFrame();

	m_bSetupEnterState = true;

}

CState_Leviathan_Turn* CState_Leviathan_Turn::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Leviathan_Turn* pInstance = new CState_Leviathan_Turn(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Leviathan_Turn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Leviathan_Turn::Clone(void* pArg)
{
	CState_Leviathan_Turn* pInstance = new CState_Leviathan_Turn(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Leviathan_Turn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Leviathan_Turn::Free()
{
	__super::Free();
}
