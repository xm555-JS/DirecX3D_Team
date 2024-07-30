#include "stdafx.h"
#include "..\Public\State_Weaver_Walk_R.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Weaver_Walk_R::CState_Weaver_Walk_R(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Weaver_Walk_R::CState_Weaver_Walk_R(const CState_Weaver_Walk_R& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Weaver_Walk_R::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_WEAVER_WEAK_BEGIN);
	m_ForceStates.push_back(STATE_BOSS_WEAVER_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_WEAVER_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_WEAVER_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Weaver_Walk_R::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Weaver_Walk_R::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAccTime = 0.f;
}

void CState_Weaver_Walk_R::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	//LooKAt 상태로 회전 후 패턴

	m_fAccTime += fTimeDelta;

	XMStoreFloat4(&m_vTargetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	if (m_fAccTime > 3.f)
		m_bEndCurState = true;

	m_pOwnerTranformCom->LookAt(XMLoadFloat4(&m_vTargetPos));
	m_pOwnerTranformCom->Go_Right(fTimeDelta, ((CUnit*)m_pOwner)->Get_UnitInfo()->fSpeed, m_pNaviCom);

	CState_AI::Tick(fTimeDelta);
}

void CState_Weaver_Walk_R::Exit(void* pArg)
{
}

_bool CState_Weaver_Walk_R::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	_float4 vTargetPos;
	XMStoreFloat4(&vTargetPos, pHandler->Get_Player()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_float fDistance = XMVectorGetX(XMVector3Length(pHandler->Get_Owner()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION) - XMLoadFloat4(&vTargetPos)));

	if (fDistance <= 7.f)
		return false;

	return true;
}

CState_Weaver_Walk_R* CState_Weaver_Walk_R::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Weaver_Walk_R* pInstance = new CState_Weaver_Walk_R(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Weaver_Walk_R");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Weaver_Walk_R::Clone(void* pArg)
{
	CState_Weaver_Walk_R* pInstance = new CState_Weaver_Walk_R(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Weaver_Walk_R");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Weaver_Walk_R::Free()
{
	__super::Free();
}
