#include "stdafx.h"
#include "..\Public\State_Dragon_Idle.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Channel.h"

#include "Player.h"
#include "Easing_Utillity.h"
#include "Imgui_Manager.h"

CState_Dragon_Idle::CState_Dragon_Idle(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Dragon_Idle::CState_Dragon_Idle(const CState_Dragon_Idle& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Dragon_Idle::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_DRAGON_DEAD);

	m_ForceStates.push_back(STATE_BOSS_DRAGON_TURNL);
	m_ForceStates.push_back(STATE_BOSS_DRAGON_TURNR);

	m_ForceStates.push_back(STATE_BOSS_DRAGON_RUN);

	m_ForceStates.push_back(STATE_BOSS_DRAGON_ULT_ATTACK_1);

	m_ReserveStates.push_back(STATE_BOSS_DRAGON_ATTACK_01);		// 레이저
	m_ReserveStates.push_back(STATE_BOSS_DRAGON_ATTACK_09);		// 몸통박치기
	m_ReserveStates.push_back(STATE_BOSS_DRAGON_ATTACK_04);		// 번개
	m_ReserveStates.push_back(STATE_BOSS_DRAGON_ATTACK_02);		// 미사일
	
	return S_OK;
}

HRESULT CState_Dragon_Idle::Initialize(void* pArg)
{
	m_fChaseDistance = 14.f;
	return S_OK;
}

void CState_Dragon_Idle::Enter(void* pArg)
{	
	__super::Enter(pArg);

	if (nullptr != pArg)
	{
		memcpy(&m_ePreState, pArg, sizeof(STATE_TYPE));

		if (STATE_BOSS_DRAGON_TURNL == m_ePreState
			|| STATE_BOSS_DRAGON_TURNR == m_ePreState)
			m_bEndCurState = true;
	}

	if (((CState_Boss*)m_pState_Handler->Get_State(STATE_BOSS_DRAGON_ULT_ATTACK_1))->Get_ChangePhase() && !m_bChangePhase)
	{
		m_ForceStates.pop_back();

		m_ReserveStates.clear();

		m_ReserveStates.push_back(STATE_BOSS_DRAGON_ATTACK_03);			// 레이저
		m_ReserveStates.push_back(STATE_BOSS_DRAGON_ATTACK_04);			// 번개
		m_ReserveStates.push_back(STATE_BOSS_DRAGON_ATTACK_08);			// 꼬리치기
		m_ReserveStates.push_back(STATE_BOSS_DRAGON_ULT_ATTACK_1);		// 땅에 쳐박

		m_bChangePhase = true;
		m_fChaseDistance = 19.f;
	}

	m_pColCom = ((CCollider*)m_pOwner->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACKRANGE)));
	XMStoreFloat4(&m_vCurPos, m_pOwner->Get_Pos());
	m_vCurPos.y = 163.5f;

	m_fAnimSpeed = 1.2f;
	m_bEndCurState = true;

	Set_CurKeyFrame(m_iKeyFrame);
	m_pTargetModel->Set_ModelEasingRatio(0.1f);
}

void CState_Dragon_Idle::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_iKeyFrame = Get_CurKeyFrame();

	_vector vTargetPos = XMLoadFloat4(&m_vCurPos) + XMVectorSet(0.f, 2.f, 0.f, 0.f);
	_vector vCurPos;

	if (m_fAccTime < 0.8f)
	{
		m_fAccTime += fTimeDelta;
		vCurPos = CEasing_Utillity::sinfInOut(XMLoadFloat4(&m_vCurPos), vTargetPos, m_fAccTime, 0.8f);
		m_pOwnerTranformCom->Set_State(CTransform::STATE_TRANSLATION, vCurPos);
	}
	else if(m_fDownTime < 0.8f)
	{
		m_fDownTime += fTimeDelta;
		vCurPos = CEasing_Utillity::sinfInOut(vTargetPos, XMLoadFloat4(&m_vCurPos), m_fDownTime, 0.8f);
		m_pOwnerTranformCom->Set_State(CTransform::STATE_TRANSLATION, vCurPos);
	}
	else
	{
		m_fAccTime = 0.f;
		m_fDownTime = 0.f;
	}

	m_fWingTime += fTimeDelta;

	//if (m_fWingTime >= DEBUGFLOAT4(0).x)
	//{
	//	IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySound(TEXT("Dragon_Wing.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, 0.5f));
	//	m_fWingTime = 0.f;
	//}

	CState_Boss::Tick(fTimeDelta);
}

void CState_Dragon_Idle::Exit(void* pArg)
{
	m_iKeyFrame = Get_CurKeyFrame();
	m_fAnimSpeed = 1.2f;
	m_pTargetModel->Set_ModelEasingRatio(0.2f);
}

_bool CState_Dragon_Idle::Check_Condition(CState_Handler* pHandler)
{
	return true;
}

void CState_Dragon_Idle::Move_Idle(_float fTimeDelta)
{
}

CState_Dragon_Idle* CState_Dragon_Idle::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Dragon_Idle* pInstance = new CState_Dragon_Idle(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Dragon_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Dragon_Idle::Clone(void* pArg)
{
	CState_Dragon_Idle* pInstance = new CState_Dragon_Idle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Dragon_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Dragon_Idle::Free()
{
	__super::Free();
}
