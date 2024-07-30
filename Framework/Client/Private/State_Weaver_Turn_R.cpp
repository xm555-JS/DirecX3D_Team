#include "stdafx.h"
#include "..\Public\State_Weaver_Turn_R.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Weaver_Turn_R::CState_Weaver_Turn_R(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Weaver_Turn_R::CState_Weaver_Turn_R(const CState_Weaver_Turn_R& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Weaver_Turn_R::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_WEAVER_WEAK_BEGIN);
	m_ForceStates.push_back(STATE_BOSS_WEAVER_DEAD);

	m_ForceStates.push_back(STATE_BOSS_WEAVER_ATTACK_10);				//페이즈 넘어감

	m_ReserveStates.push_back(STATE_BOSS_WEAVER_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_WEAVER_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Weaver_Turn_R::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Weaver_Turn_R::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fEasingTime = m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_Duration();
	m_fAccTime = 0.f;
	m_fAnimSpeed = 2.f;

	m_ReserveStates.clear();

	m_ReserveStates = *(m_pState_Handler->Get_State(STATE_BOSS_WEAVER_IDLE)->Get_ReserveState());

	XMStoreFloat4(&m_vStartLook, m_pState_Handler->Get_Owner()->Get_Transform()->Get_State(CTransform::STATE_LOOK));
}

void CState_Weaver_Turn_R::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	m_fAccTime += fTimeDelta * m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_TickperSecond();

	_float4 vTargetPos;
	XMStoreFloat4(&vTargetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vLook = XMLoadFloat4(&vTargetPos) - m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION);

	_float	fCurTime = min(1.f, m_fAccTime / m_fEasingTime);

	_vector vFinalLook = XMVectorLerp(XMLoadFloat4(&m_vStartLook), vLook, fCurTime * fCurTime);

	m_pOwnerTranformCom->YFixedLook(vFinalLook);
	CState_Boss::Tick(fTimeDelta);
}

void CState_Weaver_Turn_R::Exit(void* pArg)
{
	m_pState_Handler->Get_State(STATE_BOSS_WEAVER_IDLE)->Set_ReserveState(&m_ReserveStates);
}

_bool CState_Weaver_Turn_R::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	_vector vCurLook = pHandler->Get_Owner()->Get_Transform()->Get_State(CTransform::STATE_LOOK);
	_vector vGoalLook = pHandler->Get_Player()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION) -
		pHandler->Get_Owner()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_float fDgree = XMConvertToDegrees(XMVectorGetX(XMVector3AngleBetweenVectors(vCurLook, vGoalLook)));

	if (fDgree > 30.f)
		return true;

	return false;
}

CState_Weaver_Turn_R* CState_Weaver_Turn_R::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Weaver_Turn_R* pInstance = new CState_Weaver_Turn_R(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Weaver_Turn_R");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Weaver_Turn_R::Clone(void* pArg)
{
	CState_Weaver_Turn_R* pInstance = new CState_Weaver_Turn_R(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Weaver_Turn_R");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Weaver_Turn_R::Free()
{
	__super::Free();
}
