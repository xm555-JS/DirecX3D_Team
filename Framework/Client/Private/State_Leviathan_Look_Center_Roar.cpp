#include "stdafx.h"
#include "..\Public\State_Leviathan_Look_Center_Roar.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

#include "Enemy_Boss_Leviathan.h"

#include "Easing_Utillity.h"
#include "Imgui_Manager.h"

CState_Leviathan_Look_Center_Roar::CState_Leviathan_Look_Center_Roar(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Leviathan(pManager, eState)
{
}

CState_Leviathan_Look_Center_Roar::CState_Leviathan_Look_Center_Roar(const CState_Leviathan_Look_Center_Roar& rhs)
	: CState_Leviathan(rhs)
{
}

HRESULT CState_Leviathan_Look_Center_Roar::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_LEVIATHAN_DEAD);
	m_ForceStates.push_back(STATE_BOSS_LEVIATHAN_WEAK);
	//m_ReserveStates.push_back(STATE_BOSS_LEVIATHAN_TURN);
	//m_ReserveProbabilitys[STATE_BOSS_LEVIATHAN_TURN] = 1;

	m_ReserveStates.push_back(STATE_BOSS_LEVIATHAN_JUMP_TURN);
	m_ReserveProbabilitys[STATE_BOSS_LEVIATHAN_JUMP_TURN] = 1;


	return S_OK;
}

HRESULT CState_Leviathan_Look_Center_Roar::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Leviathan_Look_Center_Roar::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 2.f;

	m_pTargetModel->Set_ModelEasingRatio(1.f);
	m_bSetupEnterState = false;

	//_vector vStartPos = m_pOwner->Get_Pos();
	//_vector vEndPos = XMVectorSet(290.f, 0.f, 225.f, 1.f);

	//_vector vStartLook = m_pOwnerTranformCom->Get_State(CTransform::STATE_LOOK);
	//_vector vEndLook = XMVector3Normalize(vEndPos - vStartPos);
	//
	//_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	//_vector vStartRight = XMVector3Cross(vUp, vStartLook);
	//vStartLook = XMVector3Cross(vStartRight, vUp);

	//_vector vEndRight = XMVector3Cross(vUp, vEndLook);
	//vEndLook = XMVector3Cross(vEndRight, vUp);

	//m_fStartHight = XMVectorGetY(vStartPos);

	//XMStoreFloat4(&m_vStartPos, vStartPos);
	//XMStoreFloat4(&m_vEndPos, vEndPos);

	//XMStoreFloat4(&m_vStartLook, vStartLook);
	//XMStoreFloat4(&m_vEndLook, vEndLook);

}

void CState_Leviathan_Look_Center_Roar::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	if (190 < Get_CurKeyFrame())
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySound(TEXT("Leviathan_Roar.mp3"), CSound_Manager::CHANNEL_ENEMY_EFFECT, 0.5f));

	if (m_bSetupEnterState)
	{

		_vector vStartPos = XMLoadFloat4(&m_vStartPos);
		_vector vMidPos = XMLoadFloat4(&m_vMidPos);
		_vector vEndPos = XMLoadFloat4(&m_vEndPos);
		_vector vStartLook = XMLoadFloat4(&m_vStartLook);
		_vector vMidLook = XMLoadFloat4(&m_vMidLook);
		_vector vEndLook = XMLoadFloat4(&m_vEndLook);

		//_vector vMiddlePos = (vStartPos + vEndPos) * 0.5f;

		_uint	iKeyFrame_0 = 0;
		_uint	iKeyFrame_1 = 269;
		_uint	iKeyFrame_2 = 609;
		_uint	iKeyFrame_3 = 800;

		_uint	iCurFrame = Get_CurKeyFrame();


		//if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		//	m_bEndCurState = true;
		if (800 < iCurFrame)
			m_bEndCurState = true;

		_vector vCurPos = m_pOwner->Get_Pos();
		_vector vCurLook = m_pOwnerTranformCom->Get_State(CTransform::STATE_LOOK);
		//Move
		if (iKeyFrame_1 > iCurFrame)
		{
			vCurPos = CEasing_Utillity::sinfInOut(vStartPos, vMidPos, iCurFrame - m_iStartKeyFrame, iKeyFrame_1 - m_iStartKeyFrame);
			vCurLook = CEasing_Utillity::sinfInOut(vStartLook, vMidLook, iCurFrame - m_iStartKeyFrame, iKeyFrame_1 - m_iStartKeyFrame);
			m_fAnimSpeed = 1.f + (iCurFrame - m_iStartKeyFrame) / (iKeyFrame_1 - m_iStartKeyFrame);
		}
		//Roar
		else if (iKeyFrame_1 < iCurFrame && iKeyFrame_2 > iCurFrame)
		{
			//vCurPos = CEasing_Utillity::sinfInOut(vStartPos, vMiddlePos, Get_CurKeyFrame() - iKeyFrame_1, iKeyFrame_2);
			m_fAnimSpeed = 1.5f;
			_uint	iCurIslandIdx = ((CEnemy_Boss_Leviathan*)m_pOwner)->Get_CurIslandIdx();
			Use_Skill((LEVIATHAN_SKILL)iCurIslandIdx, fTimeDelta);

			IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySound(TEXT("Death.mp3"), CSound_Manager::CHANNEL_ENEMY_EFFECT, 0.5f));
		}
		//Move
		else if (iKeyFrame_2 < iCurFrame)
		{
			vCurPos = CEasing_Utillity::sinfInOut(vMidPos, vEndPos, iCurFrame - iKeyFrame_2, iKeyFrame_3 - iKeyFrame_2);
			vCurLook = CEasing_Utillity::sinfInOut(vMidLook, vEndLook, iCurFrame - iKeyFrame_2, iKeyFrame_3 - iKeyFrame_2);
			m_fAnimSpeed = 1.f + (1.f - (iCurFrame - iKeyFrame_2) / (iKeyFrame_3 - iKeyFrame_2));
		}

		vCurPos = XMVectorSetY(vCurPos, m_fStartHight);
		m_pOwnerTranformCom->Set_State(CTransform::STATE_TRANSLATION, vCurPos);
		m_pOwnerTranformCom->LookTo(vCurLook);




		//_uint iMinFrame_0 = 0;
		//_uint iMaxFrame_0 = 267;
		//_float fMaxSpeed = 100.f;
		//if (iMinFrame_0 < Get_CurKeyFrame() && iMaxFrame_0 > Get_CurKeyFrame())
		//{
		//	_vector vStartLook = XMLoadFloat4(&m_vStartLook);
		//	_vector vEndLook = XMLoadFloat4(&m_vEndLook);

		//	_vector vCurLook = CEasing_Utillity::sinfInOut(vStartLook, vEndLook, Get_CurKeyFrame(), iMaxFrame_0);
		//	_vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vCurLook);
		//	vCurLook = XMVector3Cross(vRight, XMVectorSet(0.f, 1.f, 0.f, 0.f));
		//	m_pOwnerTranformCom->LookTo(vCurLook);


		//	_float fMoveSpeed = XMVectorGetX(CEasing_Utillity::sinfInOut(XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(fMaxSpeed, 0.f, 0.f, 0.f), Get_CurKeyFrame() - iMinFrame_0, (iMaxFrame_0 - iMinFrame_0) * 0.5f));
		//	m_pOwnerTranformCom->Go_Front(fTimeDelta, fMoveSpeed);
		//	m_pOwnerTranformCom->Go_Down(fTimeDelta, fMoveSpeed * 0.6f);
		//	XMStoreFloat4(&m_vCurPos, m_pOwner->Get_Pos());
		//}



		//_uint iMinFrame_1 = 606;
		//_uint iMaxFrame_1 = 800;
		//if (iMinFrame_1 < Get_CurKeyFrame() && iMaxFrame_1 > Get_CurKeyFrame())
		//{
		//	_vector vCurPos = XMLoadFloat4(&m_vCurPos);
		//	_float fMoveSpeed = XMVectorGetX(CEasing_Utillity::sinfInOut(XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(fMaxSpeed, 0.f, 0.f, 0.f), Get_CurKeyFrame() - iMinFrame_1, (iMaxFrame_1 - iMinFrame_1) * 0.5f));
		//	vCurPos = CEasing_Utillity::sinfInOut(vCurPos, XMLoadFloat4(&m_vPosition), Get_CurKeyFrame() - iMinFrame_1, (iMaxFrame_1 - iMinFrame_1));
		//	m_pOwnerTranformCom->Go_Front(fTimeDelta, fMoveSpeed);
		//	_vector vFinalPos = m_pOwner->Get_Pos();
		//	vFinalPos = XMVectorSetY(vFinalPos, XMVectorGetY(vCurPos));
		//	m_pOwnerTranformCom->Set_State(CTransform::STATE_TRANSLATION, vFinalPos);
		//}

	}


	CState_Boss::Tick_AnimationLerp(fTimeDelta, "Bone_ROOT", m_pOwnerTranformCom, XMMatrixRotationY(XMConvertToRadians(0.0f)));

	if (m_pTargetModel->Get_Transform_Turn_End() && false == m_bSetupEnterState)
		SetUp_EnterInfo();
}

void CState_Leviathan_Look_Center_Roar::Exit(void* pArg)
{
	__super::Exit(pArg);

	m_fAnimSpeed = 1.f;
	m_pTargetModel->Set_ModelEasingRatio(0.2f);

	//m_pOwnerTranformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-180.f));
}

_bool CState_Leviathan_Look_Center_Roar::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

void CState_Leviathan_Look_Center_Roar::SetUp_EnterInfo()
{
	_uint	iCurIslandIdx = ((CEnemy_Boss_Leviathan*)m_pOwner)->Get_NextIslandIdx();

	_vector vStartPos = m_pOwner->Get_Pos();
	_vector vMidPos = XMVectorSet(292.f, 0.f, 246.f, 1.f);
	_vector vEndPos = ((CEnemy_Boss_Leviathan*)m_pOwner)->Get_IslandPos(iCurIslandIdx);

	_vector vStartLook = m_pOwnerTranformCom->Get_State(CTransform::STATE_LOOK);
	_vector vMidLook = XMVector3Normalize(vMidPos - vStartPos);
	_vector vEndLook = XMVector3Normalize(vEndPos - vMidPos);

	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector vStartRight = XMVector3Cross(vUp, vStartLook);
	vStartLook = XMVector3Cross(vStartRight, vUp);

	_vector vMidRight = XMVector3Cross(vUp, vMidLook);
	vMidLook = XMVector3Cross(vMidRight, vUp);

	_vector vEndRight = XMVector3Cross(vUp, vEndLook);
	vEndLook = XMVector3Cross(vEndRight, vUp);

	m_fStartHight = XMVectorGetY(vStartPos);

	XMStoreFloat4(&m_vStartPos, vStartPos);
	XMStoreFloat4(&m_vMidPos, vMidPos);
	XMStoreFloat4(&m_vEndPos, vEndPos);

	XMStoreFloat4(&m_vStartLook, vStartLook);
	XMStoreFloat4(&m_vMidLook, vMidLook);
	XMStoreFloat4(&m_vEndLook, vEndLook);

	m_iStartKeyFrame = Get_CurKeyFrame();

	m_bSetupEnterState = true;
}

CState_Leviathan_Look_Center_Roar* CState_Leviathan_Look_Center_Roar::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Leviathan_Look_Center_Roar* pInstance = new CState_Leviathan_Look_Center_Roar(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Leviathan_Look_Center_Roar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Leviathan_Look_Center_Roar::Clone(void* pArg)
{
	CState_Leviathan_Look_Center_Roar* pInstance = new CState_Leviathan_Look_Center_Roar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Leviathan_Look_Center_Roar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Leviathan_Look_Center_Roar::Free()
{
	__super::Free();
}
