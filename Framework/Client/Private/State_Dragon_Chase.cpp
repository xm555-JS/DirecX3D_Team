#include "stdafx.h"
#include "..\Public\State_Dragon_Chase.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"
#include "HIerarchyNode.h"

CState_Dragon_Chase::CState_Dragon_Chase(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Dragon_Chase::CState_Dragon_Chase(const CState_Dragon_Chase& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Dragon_Chase::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_DRAGON_APEAR);

	m_ReserveProbabilitys[STATE_BOSS_DRAGON_APEAR] = 1;

	return S_OK;
}

HRESULT CState_Dragon_Chase::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Dragon_Chase::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 1.2f;

	m_bEndCurState = true;

	_float4 vPos, vCenter;
	vCenter = _float4(100.f, 160.f, 435.27, 1.f);
	XMStoreFloat4(&vPos, XMLoadFloat4(&vCenter) + 10.f * m_pTarget->Get_Transform()->Get_State(CTransform::STATE_LOOK));
	vPos.y = 163.5f;

	m_pOwnerTranformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&vPos));

	//51.f, 96.f, 442.f
	//_vector vTargetPos = m_pTarget->Get_Pos();
	//_vector	vDir = XMVector3Normalize(vTargetPos - m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION));
	//
	//_float fLength = XMVectorGetX(XMVector3Length(m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION) - vTargetPos));
	//
	//XMStoreFloat4(&m_vSpot, m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION) + fLength * 0.5f * vDir);
	//
	//m_pColCom = ((CCollider*)m_pOwner->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACKRANGE)));
	//XMStoreFloat4(&m_vStartLook, m_pOwnerTranformCom->Get_State(CTransform::STATE_LOOK));
	//
	//Set_CurKeyFrame(565);
}

void CState_Dragon_Chase::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	//_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	//_vector	vDir = XMVector3Normalize(vTargetPos - m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION));
	//
	//_float fLength = XMVectorGetX(XMVector3Length(m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION) - vTargetPos));
	//
	//if (0 != m_pColCom->Get_EnterColliders()->size())			
	//{
	//	m_bEndCurState = true;
	//}
	//else
	//{
	//	m_pOwnerTranformCom->Go_Around(m_vSpot);
	//	m_pOwnerTranformCom->Go_Straight(fTimeDelta, ((CUnit*)m_pOwner)->Get_UnitInfo()->fSpeed * 4.5f);
	//	m_fAnimSpeed = 2.5f;
	//}

	//_float4 vTargetPos, vCurPos;
	//XMStoreFloat4(&vCurPos, m_pOwner->Get_Pos());
	//XMStoreFloat4(&vTargetPos, m_pTarget->Get_Pos());
	//vTargetPos.y = 163.5f;
	//
	////_vector vCurLook = m_pOwnerTranformCom->Get_State(CTransform::STATE_LOOK);
	//_vector vLook = XMLoadFloat4(&vTargetPos) - XMLoadFloat4(&vCurPos);
	//
	//
	////Look
	//if (620 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame()
	//	&& 670 > m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	//{
	//	m_fAnimSpeed = 1.f;
	//
	//	_float fTickperSec = m_fAnimSpeed * fTimeDelta * m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_TickperSecond();	//1프레임당 시간
	//	m_fEasingTime += fTickperSec;
	//
	//	XMStoreFloat4(&m_vTargetPos, m_pOwner->Get_Pos());
	//
	//	_float	fCurTime = min(1.f, m_fEasingTime / 49.f);
	//
	//	if (fCurTime == 1.f)
	//	{
	//		int a = 0;
	//	}
	//
	//	//vCurLook
	//	_vector vFinalLook = XMVectorLerp(XMLoadFloat4(&m_vStartLook), XMVector3Normalize(vLook), fCurTime);
	//
	//	m_pOwnerTranformCom->YFixedLook(vFinalLook);
	//}
	//
	////하강
	//if (670 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame()
	//	&& 685 > m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	//{
	//	m_fAnimSpeed = 1.f;
	//	m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Set_RootBoneName("ROOT");
	//
	//	_float fTickperSec = fTimeDelta * m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_TickperSecond();
	//	m_fDownTime += fTickperSec;
	//
	//	_vector vFinalPos = XMLoadFloat4(&vTargetPos) - 10.f * (m_pOwnerTranformCom->Get_State(CTransform::STATE_LOOK));
	//	XMVectorSetY(vFinalPos, 163.5f);
	//
	//	_vector vPos = CEasing_Utillity::sinfInOut(XMLoadFloat4(&m_vTargetPos), vFinalPos, m_fDownTime, 14.f);
	//	m_pOwnerTranformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
	//}
	//
	////684 ~ 5 까지 떨어짐//670까지 올라감
	//if (686 < Get_CurKeyFrame())
	//{
	//	m_bEndCurState = true;
	//}
	//
	//STATE_TYPE	eNextState = CState_AI::Ready_NextState();
	//if (STATE_END != eNextState)
	//{
	//	m_pOwnerTranformCom->Reset_RootBoneDiff();
	//
	//	m_pState_Handler->Change_State(eNextState);
	//
	//	m_pTargetModel->Set_RootEasing(true);
	//}
	//
	//m_pTargetModel->Play_Animation(fTimeDelta * m_fAnimSpeed, "ROOT");
	//
	//CHierarchyNode* pHierarchyNode = m_pTargetModel->Find_HierarcyNode("ROOT");
	//
	//XMStoreFloat4x4(&m_RootBoneMatrix, pHierarchyNode->Get_RealCombinedMatrix() * XMLoadFloat4x4(&m_pTargetModel->Get_TransformMatrix()));
	//
	//m_pOwner->Get_Transform()->Set_RootBoneMatrix(m_RootBoneMatrix);

	CState_AI::Tick(fTimeDelta);
}

void CState_Dragon_Chase::Exit(void* pArg)
{
	m_pTargetModel->Set_RootEasing(false);
	m_pTargetModel->Set_ModelEasingRatio(0.2f);
	m_fDownTime = 0.f;
}

_bool CState_Dragon_Chase::Check_Condition(CState_Handler* pHandler)
{
	return true;
}

CState_Dragon_Chase* CState_Dragon_Chase::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Dragon_Chase* pInstance = new CState_Dragon_Chase(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Dragon_Chase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Dragon_Chase::Clone(void* pArg)
{
	CState_Dragon_Chase* pInstance = new CState_Dragon_Chase(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Dragon_Chase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Dragon_Chase::Free()
{
	__super::Free();
}
