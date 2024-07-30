#include "stdafx.h"
#include "..\Public\State_Dragon_RunEnd.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"
#include "Easing_Utillity.h"
#include "Imgui_Manager.h"

CState_Dragon_RunEnd::CState_Dragon_RunEnd(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Dragon_RunEnd::CState_Dragon_RunEnd(const CState_Dragon_RunEnd& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Dragon_RunEnd::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_DRAGON_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_DRAGON_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_DRAGON_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Dragon_RunEnd::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Dragon_RunEnd::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAccTime = 0.f;

	Set_CurKeyFrame(70);
}

void CState_Dragon_RunEnd::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (Get_IsFinished())
		m_bEndCurState = true;

	m_fAccTime += fTimeDelta * m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_TickperSecond();

	_float4 vTargetPos;
	XMStoreFloat4(&vTargetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vLook = XMLoadFloat4(&vTargetPos) - m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION);

	_vector vFinalLook = CEasing_Utillity::sinfInOut(XMLoadFloat4(&m_vStartLook), vLook, m_fAccTime, 0.1f + /*DEBUGFLOAT4(0).x*/0.f);

	m_pOwnerTranformCom->YFixedLook(vFinalLook);

	CState_AI::Tick(fTimeDelta);
}

void CState_Dragon_RunEnd::Exit(void* pArg)
{
	m_pTargetModel->Set_ModelEasingRatio(0.1f);
	m_pTargetModel->Set_RootEasing(false);
}

_bool CState_Dragon_RunEnd::Check_Condition(CState_Handler* pHandler)
{
	return true;
}

CState_Dragon_RunEnd* CState_Dragon_RunEnd::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Dragon_RunEnd* pInstance = new CState_Dragon_RunEnd(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Dragon_RunEnd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Dragon_RunEnd::Clone(void* pArg)
{
	CState_Dragon_RunEnd* pInstance = new CState_Dragon_RunEnd(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Dragon_RunEnd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Dragon_RunEnd::Free()
{
	__super::Free();
}
