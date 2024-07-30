#include "stdafx.h"
#include "..\Public\State_Dragon_TurnR.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

CState_Dragon_TurnR::CState_Dragon_TurnR(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Dragon_TurnR::CState_Dragon_TurnR(const CState_Dragon_TurnR& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Dragon_TurnR::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_DRAGON_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_DRAGON_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_DRAGON_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Dragon_TurnR::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Dragon_TurnR::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fEasingTime = m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_Duration();
	m_fAccTime = 0.f;

	XMStoreFloat4(&m_vStartLook, m_pState_Handler->Get_Owner()->Get_Transform()->Get_State(CTransform::STATE_LOOK));
}

void CState_Dragon_TurnR::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	m_fAccTime += fTimeDelta * m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_TickperSecond();

	_float4 vTargetPos;
	XMStoreFloat4(&vTargetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vLook = XMLoadFloat4(&vTargetPos) - m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION);

	_float	fCurTime = min(1.f, m_fAccTime / m_fEasingTime);

	_vector vFinalLook = XMVectorLerp(XMLoadFloat4(&m_vStartLook), vLook, fCurTime);

	m_pOwnerTranformCom->YFixedLook(vFinalLook);

	STATE_TYPE	eNextState = CState_AI::Ready_NextState();
	if (STATE_END != eNextState)
	{
		m_pState_Handler->Change_State(eNextState, &m_eState);
	}

	m_pTargetModel->Play_Animation(fTimeDelta * m_fAnimSpeed);
}

void CState_Dragon_TurnR::Exit(void* pArg)
{
}

_bool CState_Dragon_TurnR::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	_vector vCurLook = pHandler->Get_Owner()->Get_Transform()->Get_State(CTransform::STATE_LOOK);
	_float4 vCurPos, vTargetPos;
	XMStoreFloat4(&vCurPos, pHandler->Get_Owner()->Get_Pos());
	XMStoreFloat4(&vTargetPos, pHandler->Get_Player()->Get_Pos());
	vTargetPos.y = vCurPos.y;
	_vector vGoalLook = XMLoadFloat4(&vTargetPos) - XMLoadFloat4(&vCurPos);

	_float fDgree = XMConvertToDegrees(XMVectorGetX(XMVector3AngleBetweenVectors(vCurLook, XMVector3Normalize(vGoalLook))));

	if (fDgree > 30.f)
		return true;

	return false;
}

CState_Dragon_TurnR* CState_Dragon_TurnR::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Dragon_TurnR* pInstance = new CState_Dragon_TurnR(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Dragon_TurnR");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Dragon_TurnR::Clone(void* pArg)
{
	CState_Dragon_TurnR* pInstance = new CState_Dragon_TurnR(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Dragon_TurnR");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Dragon_TurnR::Free()
{
	__super::Free();
}
