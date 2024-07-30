#include "stdafx.h"
#include "..\Public\State_Weaver_Idle.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Weaver_Idle::CState_Weaver_Idle(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Weaver_Idle::CState_Weaver_Idle(const CState_Weaver_Idle& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Weaver_Idle::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_WEAVER_WEAK_BEGIN);
	m_ForceStates.push_back(STATE_BOSS_WEAVER_DEAD);

	m_ForceStates.push_back(STATE_BOSS_WEAVER_ATTACK_10);				//페이즈 넘어감


	m_ForceStates.push_back(STATE_BOSS_WEAVER_TURN_L);
	m_ForceStates.push_back(STATE_BOSS_WEAVER_TURN_R);

	//m_ForceStates.push_back(STATE_BOSS_WEAVER_WALK_B);
	//m_ForceStates.push_back(STATE_BOSS_WEAVER_WALK_F);

	//기본공격류 공통
	m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//대쉬
	m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_01);			//평타 - 좌우로 한번씩 긁음
	
	m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//대쉬
	m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_09);			//충격파
	
	m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//대쉬
	m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_02);			//칼난사 - 칼들 왔다리 갔다리
	
	m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//대쉬
	m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_03);			//칼 발사1 주문처럼 날림 일정거리에 그냥 물리적으로 꽂을예정 - 칼에 달린 콜라이더로 공격	//칼 발사2 공격모션 날리기만 하고 이펙트로 처리할 예정 - 불릿 객체로 할 예정. 이펙트로하고 큰콜라이더로도 가능.
	
	m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//대쉬
	m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_07);			//레이저

	//m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_11_BEGIN);		//잡기기술인데 쓸지 어떨지 모르겠음
	//m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_12);

	return S_OK;
}

HRESULT CState_Weaver_Idle::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Weaver_Idle::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fEasingTime = 2.f;
	m_fAccTime = 0.f;
	m_fStateTime = fRandom(1.f, 1.5f);
	m_fAnimSpeed = 1.f;

	//2페이즈
	if (((CUnit*)m_pOwner)->Get_UnitInfo()->fHP * 0.7f > ((CUnit*)m_pOwner)->Get_UnitCurInfo()->fHP
		&& STATE_BOSS_WEAVER_ATTACK_05_1 != m_pState_Handler->Get_PreStateType()
		&& 0 == m_iPhase)
	{
		m_ReserveStates.clear();

		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//대쉬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_08);

		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//대쉬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_04);			//허리케인 }

		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//대쉬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_02);			//칼난사 - 칼들 왔다리 갔다리

		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//대쉬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_03);			//칼 발사1 주문처럼 날림 일정거리에 그냥 물리적으로 꽂을예정 - 칼에 달린 콜라이더로 공격	//칼 발사2 공격모션 날리기만 하고 이펙트로 처리할 예정 - 불릿 객체로 할 예정. 이펙트로하고 큰콜라이더로도 가능.

		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//대쉬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_07);			//레이저

		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//대쉬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_01);			//평타 - 좌우로 한번씩 긁음

		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//대쉬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_09);			//충격파

		m_iPhase = 1;
	}

	if (((CUnit*)m_pOwner)->Get_UnitInfo()->fHP * 0.4f > ((CUnit*)m_pOwner)->Get_UnitCurInfo()->fHP
		&& STATE_BOSS_WEAVER_ATTACK_05_1 != m_pState_Handler->Get_PreStateType()
		&& 1 == m_iPhase)
	{
		m_ReserveStates.clear();

		//3페이즈 추가//체력 40프로 이하
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//대쉬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_06_BEGIN);	//얼음 창 }

		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//대쉬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_08);

		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//대쉬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_07);			//레이저

		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//대쉬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_03);			//칼 발사1 주문처럼 날림 일정거리에 그냥 물리적으로 꽂을예정 - 칼에 달린 콜라이더로 공격	//칼 발사2 공격모션 날리기만 하고 이펙트로 처리할 예정 - 불릿 객체로 할 예정. 이펙트로하고 큰콜라이더로도 가능.

		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//대쉬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_04);			//허리케인 }

		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//대쉬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_01);			//평타 - 좌우로 한번씩 긁음

		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//대쉬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_09);			//충격파

		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//대쉬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_02);			//칼난사 - 칼들 왔다리 갔다리

		m_iPhase = 2;
	}

	XMStoreFloat4(&m_vStartLook, m_pOwnerTranformCom->Get_State(CTransform::STATE_LOOK));
}

void CState_Weaver_Idle::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_fAccTime += fTimeDelta;

	_float4 vTargetPos;
	XMStoreFloat4(&vTargetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vCurLook = m_pOwnerTranformCom->Get_State(CTransform::STATE_LOOK);
	_vector vLook = XMLoadFloat4(&vTargetPos) - m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION);

	_float	fCurTime = min(1.f, m_fAccTime / m_fEasingTime);

	//vCurLook
	_vector vFinalLook = XMVectorLerp(XMLoadFloat4(&m_vStartLook), vLook, fCurTime);

	m_pOwnerTranformCom->YFixedLook(vFinalLook);

	if (m_fStateTime > m_fAccTime)
		m_bEndCurState = true;

	CState_Boss::Tick(fTimeDelta);
}

void CState_Weaver_Idle::Exit(void* pArg)
{
}

_bool CState_Weaver_Idle::Check_Condition(CState_Handler* pHandler)
{
	return true;
}

CState_Weaver_Idle* CState_Weaver_Idle::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Weaver_Idle* pInstance = new CState_Weaver_Idle(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Weaver_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Weaver_Idle::Clone(void* pArg)
{
	CState_Weaver_Idle* pInstance = new CState_Weaver_Idle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Weaver_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Weaver_Idle::Free()
{
	__super::Free();
}
