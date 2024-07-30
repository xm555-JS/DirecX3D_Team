#include "stdafx.h"
#include "..\Public\State_Robella_Attack6.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

#include "Effect_Manager.h"

CState_Robella_Attack6::CState_Robella_Attack6(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Robella_Attack6::CState_Robella_Attack6(const CState_Robella_Attack6& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Robella_Attack6::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_IDLE2);

	m_ReserveProbabilitys[STATE_BOSS_ROBELLA_IDLE2] = 1;

	return S_OK;
}

HRESULT CState_Robella_Attack6::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Robella_Attack6::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fAnimSpeed = 2.f;

	m_fDamage = fRandom(700.f,800.f);

	m_pColCom = ((CCollider*)m_pOwner->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Robella_Attack6::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	_matrix ColMatrix = XMLoadFloat4x4(m_pState_Handler->Get_Owner()->Get_WorldMatrix());
	ColMatrix.r[3] += XMVector3Normalize(ColMatrix.r[2]) * 5.f;
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 25.f);
	m_pColCom->Update(ColMatrix);

	// �ش� Ŭ������ �ִϸ��̼��� ������ Idle_state�� �Ѿ
	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	//165~205 ���� 5������ �а�
	_float fDis = XMVectorGetX(XMVector3Length(m_pOwner->Get_Pos() - m_pTarget->Get_Pos()));
	if (7.5f < fDis)
		Hit_Unit(m_pColCom, ELEMENTAL_FIRE, m_fDamage, 165, 205, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit);
	// 2��° ä��

	if (130 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Robella_Whip_Short_Voice.wav"),
			CSound_Manager::CHANNEL_ENEMY, m_pOwner->Get_Pos(), 0.3f));
	}

	if (155 < Get_CurKeyFrame())
	{
		// ���� ȸ�� ������
		IMPL_ONCE(m_bEffect0, EFFECT_POS("Robella_Whip_2", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, -9.4f, 0.f, 0.f), 0.65f,
			XMVectorSet(1.f, 1.f, 1.f, 0.f)));

		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Robella_Whip.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Robella_Attack6::Exit(void* pArg)
{
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	RESET_ONCE(m_bEffect0);
}

_bool CState_Robella_Attack6::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Robella_Attack6* CState_Robella_Attack6::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Robella_Attack6* pInstance = new CState_Robella_Attack6(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Robella_Attack6");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Robella_Attack6::Clone(void* pArg)
{
	CState_Robella_Attack6* pInstance = new CState_Robella_Attack6(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Robella_Attack6");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Robella_Attack6::Free()
{
	__super::Free();
}
