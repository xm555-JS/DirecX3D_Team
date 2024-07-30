#include "stdafx.h"
#include "..\Public\State_Dragon_Patrol.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Dragon_Patrol::CState_Dragon_Patrol(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Dragon_Patrol::CState_Dragon_Patrol(const CState_Dragon_Patrol& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Dragon_Patrol::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_DRAGON_CHASE);

	m_ReserveProbabilitys[STATE_BOSS_DRAGON_CHASE] = 1;

	return S_OK;
}

HRESULT CState_Dragon_Patrol::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Dragon_Patrol::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAccTime = 0.f;
	m_fAnimSpeed = 4.f;

	XMStoreFloat4(&m_vTargetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
}

void CState_Dragon_Patrol::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_fAccTime += fTimeDelta;

	if (m_fAccTime <= 10.f)			// 트리거 전
	{
		m_pOwnerTranformCom->Go_Around(m_vTargetPos);
		m_pOwnerTranformCom->Go_Front(fTimeDelta, ((CUnit*)m_pOwner)->Get_UnitInfo()->fSpeed * 4.f);
	}
	else
	{
		//m_pTargetModel->Set_ModelEasingRatio(1.f);
		m_bEndCurState = true;
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Dragon_Patrol::Exit(void* pArg)
{
}

_bool CState_Dragon_Patrol::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Dragon_Patrol* CState_Dragon_Patrol::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Dragon_Patrol* pInstance = new CState_Dragon_Patrol(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Dragon_Patrol");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Dragon_Patrol::Clone(void* pArg)
{
	CState_Dragon_Patrol* pInstance = new CState_Dragon_Patrol(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Dragon_Patrol");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Dragon_Patrol::Free()
{
	__super::Free();
}
