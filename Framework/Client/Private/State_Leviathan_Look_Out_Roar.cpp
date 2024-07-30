#include "stdafx.h"
#include "..\Public\State_Leviathan_Look_Out_Roar.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

#include "Enemy_Boss_Leviathan.h"

#include "Easing_Utillity.h"
#include "Imgui_Manager.h"

CState_Leviathan_Look_Out_Roar::CState_Leviathan_Look_Out_Roar(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Leviathan(pManager, eState)
{
}

CState_Leviathan_Look_Out_Roar::CState_Leviathan_Look_Out_Roar(const CState_Leviathan_Look_Out_Roar& rhs)
	: CState_Leviathan(rhs)
{
}

HRESULT CState_Leviathan_Look_Out_Roar::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_LEVIATHAN_JUMP_TURN);
	m_ReserveProbabilitys[STATE_BOSS_LEVIATHAN_JUMP_TURN] = 1;

	return S_OK;
}

HRESULT CState_Leviathan_Look_Out_Roar::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Leviathan_Look_Out_Roar::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 2.f;

	m_pTargetModel->Set_ModelEasingRatio(1.f);
	m_bSetupEnterState = false;

	//_uint	iCurIslandIdx = ((CEnemy_Boss_Leviathan*)m_pOwner)->Get_NextIslandIdx();

	//_vector vStartPos = m_pOwner->Get_Pos();
	//_vector vEndPos = ((CEnemy_Boss_Leviathan*)m_pOwner)->Get_IslandPos(iCurIslandIdx);

	//_vector vStartLook = m_pOwnerTranformCom->Get_State(CTransform::STATE_LOOK);
	//_vector vEndLook = XMVector3Normalize(vEndPos - vStartPos);

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

void CState_Leviathan_Look_Out_Roar::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (m_bSetupEnterState)
	{

		_vector vStartPos = XMLoadFloat4(&m_vStartPos);
		_vector vEndPos = XMLoadFloat4(&m_vEndPos);
		_vector vStartLook = XMLoadFloat4(&m_vStartLook);
		_vector vEndLook = XMLoadFloat4(&m_vEndLook);

		_vector vMiddlePos = (vStartPos + vEndPos) * 0.5f;

		_uint	iKeyFrame_0 = 0;
		_uint	iKeyFrame_1 = 269;
		_uint	iKeyFrame_2 = 609;
		_uint	iKeyFrame_3 = 800;


		//if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		//	m_bEndCurState = true;
		if (800 < Get_CurKeyFrame())
			m_bEndCurState = true;

		_vector vCurPos = m_pOwner->Get_Pos();
		_vector vCurLook = m_pOwnerTranformCom->Get_State(CTransform::STATE_LOOK);



		//Move
		if (iKeyFrame_1 > Get_CurKeyFrame())
		{
			vCurPos = CEasing_Utillity::sinfInOut(vStartPos, vMiddlePos, Get_CurKeyFrame() - iKeyFrame_0, iKeyFrame_1);
			vCurLook = CEasing_Utillity::sinfInOut(vStartLook, vEndLook, Get_CurKeyFrame() - iKeyFrame_0, iKeyFrame_1);
		}
		//Roar
		else if (iKeyFrame_1 < Get_CurKeyFrame() && iKeyFrame_2 > Get_CurKeyFrame())
		{
			//vCurPos = CEasing_Utillity::sinfInOut(vStartPos, vMiddlePos, Get_CurKeyFrame() - iKeyFrame_1, iKeyFrame_2);

		}
		//Move
		else if (iKeyFrame_2 < Get_CurKeyFrame())
		{
			vCurPos = CEasing_Utillity::sinfInOut(vMiddlePos, vEndPos, Get_CurKeyFrame() - iKeyFrame_2, iKeyFrame_3 - iKeyFrame_2);

		}

		vCurPos = XMVectorSetY(vCurPos, m_fStartHight);
		m_pOwnerTranformCom->Set_State(CTransform::STATE_TRANSLATION, vCurPos);
		m_pOwnerTranformCom->LookTo(vCurLook);
	}




	//_vector vStartLook = XMLoadFloat4(&m_vStartLook);
	//_vector vEndLook = XMLoadFloat4(&m_vEndLook);
	//_vector vCurLook = CEasing_Utillity::sinfInOut(vStartLook, vEndLook, Get_CurKeyFrame(), iEndKeyFrame);

	//m_pOwnerTranformCom->LookTo(vCurLook);

	CState_Boss::Tick_AnimationLerp(fTimeDelta, "Bone_ROOT", m_pOwnerTranformCom, XMMatrixRotationY(XMConvertToRadians(0.f)));

	if (m_pTargetModel->Get_Transform_Turn_End() && false == m_bSetupEnterState)
		SetUp_EnterInfo();
}

void CState_Leviathan_Look_Out_Roar::Exit(void* pArg)
{
	__super::Exit(pArg);

	m_fAnimSpeed = 1.f;
	m_pTargetModel->Set_ModelEasingRatio(0.2f);

	//m_pOwnerTranformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-180.f));
}

_bool CState_Leviathan_Look_Out_Roar::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

void CState_Leviathan_Look_Out_Roar::SetUp_EnterInfo()
{
	//_uint	iCurIslandIdx = ((CEnemy_Boss_Leviathan*)m_pOwner)->Get_NextIslandIdx();
	_uint	iCurIslandIdx = ((CEnemy_Boss_Leviathan*)m_pOwner)->Get_CurIslandIdx();

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

	m_bSetupEnterState = true;
}

CState_Leviathan_Look_Out_Roar* CState_Leviathan_Look_Out_Roar::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Leviathan_Look_Out_Roar* pInstance = new CState_Leviathan_Look_Out_Roar(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Leviathan_Look_Out_Roar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Leviathan_Look_Out_Roar::Clone(void* pArg)
{
	CState_Leviathan_Look_Out_Roar* pInstance = new CState_Leviathan_Look_Out_Roar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Leviathan_Look_Out_Roar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Leviathan_Look_Out_Roar::Free()
{
	__super::Free();
}
