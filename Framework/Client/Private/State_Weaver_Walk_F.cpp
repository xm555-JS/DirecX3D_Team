#include "stdafx.h"
#include "..\Public\State_Weaver_Walk_F.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Weaver_Walk_F::CState_Weaver_Walk_F(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Weaver_Walk_F::CState_Weaver_Walk_F(const CState_Weaver_Walk_F& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Weaver_Walk_F::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_WEAVER_WEAK_BEGIN);
	m_ForceStates.push_back(STATE_BOSS_WEAVER_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_WEAVER_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_WEAVER_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Weaver_Walk_F::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Weaver_Walk_F::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAccTime = 0.f;
}

void CState_Weaver_Walk_F::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_fAccTime += fTimeDelta;

	if (m_fAccTime >= 1.f)
		m_bEndCurState = true;

	m_pOwnerTranformCom->Go_Straight(fTimeDelta, ((CUnit*)m_pOwner)->Get_UnitInfo()->fSpeed, m_pNaviCom);

	//LookAt, Chase

	//if (/*타겟과의 거리 비교*/)
	//	m_bEndCurState = true;


	CState_AI::Tick(fTimeDelta);
}

void CState_Weaver_Walk_F::Exit(void* pArg)
{
}

_bool CState_Weaver_Walk_F::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	_float4 vTargetPos;
	XMStoreFloat4(&vTargetPos, pHandler->Get_Player()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_float fDistance = XMVectorGetX(XMVector3Length(pHandler->Get_Owner()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION) - XMLoadFloat4(&vTargetPos)));

	if (fDistance <= 5.f)
		return false;

	return true;
}

CState_Weaver_Walk_F* CState_Weaver_Walk_F::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Weaver_Walk_F* pInstance = new CState_Weaver_Walk_F(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Weaver_Walk_F");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Weaver_Walk_F::Clone(void* pArg)
{
	CState_Weaver_Walk_F* pInstance = new CState_Weaver_Walk_F(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Weaver_Walk_F");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Weaver_Walk_F::Free()
{
	__super::Free();
}
