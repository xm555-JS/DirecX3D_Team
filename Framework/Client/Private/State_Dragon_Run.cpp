#include "stdafx.h"
#include "..\Public\State_Dragon_Run.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Dragon_Run::CState_Dragon_Run(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Dragon_Run::CState_Dragon_Run(const CState_Dragon_Run& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Dragon_Run::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_DRAGON_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_DRAGON_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_DRAGON_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Dragon_Run::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Dragon_Run::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 1.2f;
	m_pTargetModel->Set_ModelEasingRatio(0.2f);

	XMStoreFloat4(&m_vCurPos, m_pOwner->Get_Pos());

	m_fAccTime = 0.f;
	m_fDownTime = 0.f;
}

void CState_Dragon_Run::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_fAccTime += fTimeDelta;

	_float4 vTargetPos, vDir;

	XMStoreFloat4(&vTargetPos, m_pTarget->Get_Pos());

	vTargetPos.y = m_vCurPos.y;

	XMStoreFloat4(&vDir, XMLoadFloat4(&m_vCurPos) - XMLoadFloat4(&vTargetPos));

	_float fDistance = XMVectorGetX(XMVector3Length(XMLoadFloat4(&vDir)));

	_vector vFinalPos = XMLoadFloat4(&vTargetPos) + 10.f * XMVector3Normalize(XMLoadFloat4(&vDir));

	_vector vPos = CEasing_Utillity::ExpoInOut(XMLoadFloat4(&m_vCurPos), vFinalPos, m_fAccTime, 1.5f);

	m_pOwnerTranformCom->YFixedLookAt(XMLoadFloat4(&vTargetPos));
	m_pOwnerTranformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);

	if (m_fAccTime > 1.5f)
		m_bEndCurState = true;

	CState_Boss::Tick(fTimeDelta);
}

void CState_Dragon_Run::Exit(void* pArg)
{
	m_fAccTime = 0.f;
	m_pTargetModel->Set_ModelEasingRatio(0.1f);
}

_bool CState_Dragon_Run::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	_float4 vTargetPos, vDir;

	XMStoreFloat4(&vTargetPos, pHandler->Get_Player()->Get_Pos());

	XMStoreFloat4(&vDir, pHandler->Get_Owner()->Get_Pos() - XMLoadFloat4(&vTargetPos));

	_float fDistance = XMVectorGetX(XMVector3Length(XMLoadFloat4(&vDir)));

	if(fDistance > 20.f)
		return true;

	return false;
}

CState_Dragon_Run* CState_Dragon_Run::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Dragon_Run* pInstance = new CState_Dragon_Run(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Dragon_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Dragon_Run::Clone(void* pArg)
{
	CState_Dragon_Run* pInstance = new CState_Dragon_Run(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Dragon_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Dragon_Run::Free()
{
	__super::Free();
}
