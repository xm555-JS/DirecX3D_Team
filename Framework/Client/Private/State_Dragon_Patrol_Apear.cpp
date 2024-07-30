#include "stdafx.h"
#include "..\Public\State_Dragon_Patrol_Apear.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"
#include "Enemy_Boss_Dragon.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Dragon_Patrol_Apear::CState_Dragon_Patrol_Apear(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Dragon_Patrol_Apear::CState_Dragon_Patrol_Apear(const CState_Dragon_Patrol_Apear& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Dragon_Patrol_Apear::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_DRAGON_CHASE);

	m_ReserveProbabilitys[STATE_BOSS_DRAGON_CHASE] = 1;

	return S_OK;
}

HRESULT CState_Dragon_Patrol_Apear::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Dragon_Patrol_Apear::Enter(void* pArg)
{
	__super::Enter(pArg);
}

void CState_Dragon_Patrol_Apear::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_fAccTime += fTimeDelta;

	//_float fDistance = XMVectorGetX(XMVector3Length(m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION) - m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
	//
	//if (fDistance < 1.f)	//타겟과의 거리
	//{
	//	m_bEndCurState = true; 
	//}

	if (nullptr == ((CEnemy_Boss*)m_pOwner)->Get_Target())			// 트리거 전
	{
		m_pOwnerTranformCom->Go_Around(_float4(100.f, 160.f, 435.27, 1.f));	//대략 중심점 하나XMVectorSet(100.f, 160.f, 435.27, 1.f)
		m_pOwnerTranformCom->Go_Front(fTimeDelta, ((CUnit*)m_pOwner)->Get_UnitInfo()->fSpeed * 2.f);
		m_fAnimSpeed = 1.8f;
	}
	else
	{
		//m_pTargetModel->Set_ModelEasingRatio(1.f);
		m_fAnimSpeed = 1.f;
		m_bEndCurState = true;
	}



	CState_AI::Tick(fTimeDelta);
}

void CState_Dragon_Patrol_Apear::Exit(void* pArg)
{
	m_pTargetModel->Set_ModelEasingRatio(0.2f);
}

_bool CState_Dragon_Patrol_Apear::Check_Condition(CState_Handler* pHandler)
{
	return true;
}

CState_Dragon_Patrol_Apear* CState_Dragon_Patrol_Apear::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Dragon_Patrol_Apear* pInstance = new CState_Dragon_Patrol_Apear(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Dragon_Patrol_Apear");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Dragon_Patrol_Apear::Clone(void* pArg)
{
	CState_Dragon_Patrol_Apear* pInstance = new CState_Dragon_Patrol_Apear(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Dragon_Patrol_Apear");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Dragon_Patrol_Apear::Free()
{
	__super::Free();
}
