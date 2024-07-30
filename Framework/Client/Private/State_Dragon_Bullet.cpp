#include "stdafx.h"
#include "..\Public\State_Dragon_Bullet.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"

CState_Dragon_Bullet::CState_Dragon_Bullet(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Dragon_Bullet::CState_Dragon_Bullet(const CState_Dragon_Bullet& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Dragon_Bullet::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_DRAGON_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_DRAGON_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Dragon_Bullet::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Dragon_Bullet::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fEasingTime = m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_Duration();
	m_fAccTime = 0.f;

	XMStoreFloat4(&m_vStartLook, m_pOwnerTranformCom->Get_State(CTransform::STATE_LOOK));
}

void CState_Dragon_Bullet::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_fAccTime += fTimeDelta * m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_TickperSecond();

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	_float4 vTargetPos;
	XMStoreFloat4(&vTargetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vCurLook = m_pOwnerTranformCom->Get_State(CTransform::STATE_LOOK);
	_vector vLook = XMLoadFloat4(&vTargetPos) - m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION);

	_float	fCurTime = min(1.f, m_fAccTime / m_fEasingTime);

	//vCurLook
	_vector vFinalLook = XMVectorLerp(XMLoadFloat4(&m_vStartLook), vLook, fCurTime);

	m_pOwnerTranformCom->YFixedLook(vFinalLook);

	CState_AI::Tick(fTimeDelta);
}

void CState_Dragon_Bullet::Exit(void* pArg)
{
	m_pTargetModel->Set_ModelEasingRatio(0.5f);
}

_bool CState_Dragon_Bullet::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Dragon_Bullet* CState_Dragon_Bullet::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Dragon_Bullet* pInstance = new CState_Dragon_Bullet(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Dragon_Bullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Dragon_Bullet::Clone(void* pArg)
{
	CState_Dragon_Bullet* pInstance = new CState_Dragon_Bullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Dragon_Bullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Dragon_Bullet::Free()
{
	__super::Free();
}
