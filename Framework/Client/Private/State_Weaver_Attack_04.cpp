#include "stdafx.h"
#include "..\Public\State_Weaver_Attack_04.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"
#include "Enemy_Weaver_Hurricane.h"

#include "Easing_Utillity.h"

#include "Effect_Manager.h"
#include "Imgui_Manager.h"

CState_Weaver_Attack_04::CState_Weaver_Attack_04(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Weaver_Attack_04::CState_Weaver_Attack_04(const CState_Weaver_Attack_04& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Weaver_Attack_04::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_WEAVER_WEAK_BEGIN);
	m_ForceStates.push_back(STATE_BOSS_WEAVER_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_WEAVER_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_WEAVER_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Weaver_Attack_04::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Weaver_Attack_04::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAccTime = 0.f;
	EFFECT_POS("Boss_Weaver_Attack_09_ShockWave_Aura", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 0.8f, -0.1f, 1.f));
	m_pColCom = ((CCollider*)m_pOwner->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK, 6)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Weaver_Attack_04::Tick(_float fTimeDelta)
{
	_matrix ColMatrix = XMLoadFloat4x4(m_pState_Handler->Get_Owner()->Get_WorldMatrix());
	ColMatrix.r[3] -= XMVector3Normalize(ColMatrix.r[2]) * 2.f;
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 7.f);

	IMPL_ONCE(m_bSound[0], m_iSoundChannel = MGR(CSound_Manager)->PlaySurroundSound(TEXT("Tornado.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));

	for(_uint i = 0; i < 20; ++i)
		Hit_Unit(m_pColCom, ELEMENTAL_ICE, fRandom(135.f, 165.f), 95 + i * 6, 100 + i * 6, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[i]);

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;
	//93 165 255
	 
	
	//Effect
	if (93 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect0,
			EFFECT_POS("Boss_Weaver_Attack_04_Tornado_Center", m_pState_Handler->Get_Owner()->Get_WorldMatrix());
		)
	}
	//if (260 <= Get_CurKeyFrame())
	//{
	//	IMPL_ONCE(m_bEffect1,
	//		EFFECT_POS("Boss_Weaver_Attack_04_Tornado_Center_End", m_pState_Handler->Get_Owner()->Get_WorldMatrix());
	//	)
	//}

	m_fTimeAcc_Effect0 += fTimeDelta;
	m_fTimeAcc_Effect1 += fTimeDelta;
	m_fTimeAcc_Effect2 += fTimeDelta;
	if (93 <= Get_CurKeyFrame() && 250 >= Get_CurKeyFrame())
	{
		_float fRange = 12.5f;
		// 작은 얼음
		if (0.12 < m_fTimeAcc_Effect0)
		{
			m_fTimeAcc_Effect0 = 0.f;
			// 색 다르게 해서 
			EFFECT_POS("Boss_Weaver_Attack_04_Tornado_Ice_A1", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(fRandom(-fRange, fRange), 0.f, fRandom(-fRange, fRange), 1.f), XMVectorSet(0.f, XMConvertToRadians(fRandom(-180.f, 180.f)), 0.f, 0.f), 1.f, XMVectorSet(fRandom(0.8f, 1.2f), fRandom(0.8f, 1.2f), fRandom(0.8f, 1.2f), 0.f));
			EFFECT_POS("Boss_Weaver_Attack_04_Tornado_Ice_B1", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(fRandom(-fRange, fRange), 0.f, fRandom(-fRange, fRange), 1.f), XMVectorSet(0.f, XMConvertToRadians(fRandom(-180.f, 180.f)), 0.f, 0.f), 1.f, XMVectorSet(fRandom(0.8f, 1.2f), fRandom(0.8f, 1.2f), fRandom(0.8f, 1.2f), 0.f));
			EFFECT_POS("Boss_Weaver_Attack_04_Tornado_Ice_C1", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(fRandom(-fRange, fRange), 0.f, fRandom(-fRange, fRange), 1.f), XMVectorSet(0.f, XMConvertToRadians(fRandom(-180.f, 180.f)), 0.f, 0.f), 1.f, XMVectorSet(fRandom(0.8f, 1.2f), fRandom(0.8f, 1.2f), fRandom(0.8f, 1.2f), 0.f));
			EFFECT_POS("Boss_Weaver_Attack_04_Tornado_Ice_D1", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(fRandom(-fRange, fRange), 0.f, fRandom(-fRange, fRange), 1.f), XMVectorSet(0.f, XMConvertToRadians(fRandom(-180.f, 180.f)), 0.f, 0.f), 1.f, XMVectorSet(fRandom(0.8f, 1.2f), fRandom(0.8f, 1.2f), fRandom(0.8f, 1.2f), 0.f));

			EFFECT_POS("Boss_Weaver_Attack_04_Tornado_Ice_A2", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(fRandom(-fRange, fRange), 0.f, fRandom(-fRange, fRange), 1.f), XMVectorSet(0.f, XMConvertToRadians(fRandom(-180.f, 180.f)), 0.f, 0.f), 1.f, XMVectorSet(fRandom(0.8f, 1.2f), fRandom(0.8f, 1.2f), fRandom(0.8f, 1.2f), 0.f));
			EFFECT_POS("Boss_Weaver_Attack_04_Tornado_Ice_B2", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(fRandom(-fRange, fRange), 0.f, fRandom(-fRange, fRange), 1.f), XMVectorSet(0.f, XMConvertToRadians(fRandom(-180.f, 180.f)), 0.f, 0.f), 1.f, XMVectorSet(fRandom(0.8f, 1.2f), fRandom(0.8f, 1.2f), fRandom(0.8f, 1.2f), 0.f));
			EFFECT_POS("Boss_Weaver_Attack_04_Tornado_Ice_C2", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(fRandom(-fRange, fRange), 0.f, fRandom(-fRange, fRange), 1.f), XMVectorSet(0.f, XMConvertToRadians(fRandom(-180.f, 180.f)), 0.f, 0.f), 1.f, XMVectorSet(fRandom(0.8f, 1.2f), fRandom(0.8f, 1.2f), fRandom(0.8f, 1.2f), 0.f));
			EFFECT_POS("Boss_Weaver_Attack_04_Tornado_Ice_D2", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(fRandom(-fRange, fRange), 0.f, fRandom(-fRange, fRange), 1.f), XMVectorSet(0.f, XMConvertToRadians(fRandom(-180.f, 180.f)), 0.f, 0.f), 1.f, XMVectorSet(fRandom(0.8f, 1.2f), fRandom(0.8f, 1.2f), fRandom(0.8f, 1.2f), 0.f));

			EFFECT_POS("Boss_Weaver_Attack_04_Tornado_Ice_A3", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(fRandom(-fRange, fRange), 0.f, fRandom(-fRange, fRange), 1.f), XMVectorSet(0.f, XMConvertToRadians(fRandom(-180.f, 180.f)), 0.f, 0.f), 1.f, XMVectorSet(fRandom(0.8f, 1.2f), fRandom(0.8f, 1.2f), fRandom(0.8f, 1.2f), 0.f));
			EFFECT_POS("Boss_Weaver_Attack_04_Tornado_Ice_B3", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(fRandom(-fRange, fRange), 0.f, fRandom(-fRange, fRange), 1.f), XMVectorSet(0.f, XMConvertToRadians(fRandom(-180.f, 180.f)), 0.f, 0.f), 1.f, XMVectorSet(fRandom(0.8f, 1.2f), fRandom(0.8f, 1.2f), fRandom(0.8f, 1.2f), 0.f));
			EFFECT_POS("Boss_Weaver_Attack_04_Tornado_Ice_C3", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(fRandom(-fRange, fRange), 0.f, fRandom(-fRange, fRange), 1.f), XMVectorSet(0.f, XMConvertToRadians(fRandom(-180.f, 180.f)), 0.f, 0.f), 1.f, XMVectorSet(fRandom(0.8f, 1.2f), fRandom(0.8f, 1.2f), fRandom(0.8f, 1.2f), 0.f));
			EFFECT_POS("Boss_Weaver_Attack_04_Tornado_Ice_D3", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(fRandom(-fRange, fRange), 0.f, fRandom(-fRange, fRange), 1.f), XMVectorSet(0.f, XMConvertToRadians(fRandom(-180.f, 180.f)), 0.f, 0.f), 1.f, XMVectorSet(fRandom(0.8f, 1.2f), fRandom(0.8f, 1.2f), fRandom(0.8f, 1.2f), 0.f));
		}
		// 파티클
		if (0.5f < m_fTimeAcc_Effect1)
		{
			m_fTimeAcc_Effect1 = 0.f;
			EFFECT_POS("Boss_Weaver_Attack_04_Tornado_P", m_pState_Handler->Get_Owner()->Get_WorldMatrix());
		}
#if 1 // 객체에 달 토네이도.
		// 미니 토네이도
		if(!m_bCreateObj)
		{
			Set_HurricanePos();
			m_bCreateObj = true;
		}
#endif // 1 // 객체에 달 토네이도.

	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Weaver_Attack_04::Exit(void* pArg)
{
	MGR(CSound_Manager)->StopSound((CSound_Manager::CHANNELID)m_iSoundChannel);
	m_fTimeAcc_Effect0 = 0.f;
	m_fTimeAcc_Effect1 = 0.f;
	m_fTimeAcc_Effect2 = 0.f;
	RESET_ONCE(m_bEffect0);
	RESET_ONCE(m_bEffect1);
	RESET_ONCE(m_bEffect2);
	RESET_ONCE(m_bEffect3);
	RESET_ONCE(m_bEffect4);
	RESET_ONCE(m_bEffect5);

	m_bCreateObj = false;

	for (_uint i = 0; i < 20; ++i)
		m_bHit[i] = false;

	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
}

_bool CState_Weaver_Attack_04::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

void CState_Weaver_Attack_04::Set_HurricanePos()
{
	_float fDgree = 120.f;
	_matrix		TargetRotationMatrix, TargetCurRotationMatrix;
	_matrix		TargetWorldMatrix = XMLoadFloat4x4(m_pOwner->Get_WorldMatrix());
	CEnemy_Weaver_Hurricane::HURRICANEDESC HurricaneDesc;

	for (_uint i = 0; i < 3; ++i)
	{
		TargetRotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(i * fDgree));

		TargetCurRotationMatrix = TargetRotationMatrix * TargetWorldMatrix;
		TargetRotationMatrix.r[3] += XMVectorSet(0.f, 0.f, 0.f, 0.f) + 10.3f * XMVector3Normalize(TargetRotationMatrix.r[2]);
		TargetCurRotationMatrix.r[3] += XMVectorSet(0.f, 0.f, 0.f, 0.f) + 10.3f * XMVector3Normalize(TargetCurRotationMatrix.r[2]);

		XMStoreFloat4(&HurricaneDesc.OrbitPos, TargetRotationMatrix.r[3]);
		XMStoreFloat4(&HurricaneDesc.StartPos, TargetCurRotationMatrix.r[3]);
		HurricaneDesc.pTarget = m_pOwner;
		HurricaneDesc.iIndex = i;

		GAMEINSTANCE->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_WEAVER_HURRICANE), ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_WEAVER_HURRICANE), &HurricaneDesc);

	}
}

CState_Weaver_Attack_04* CState_Weaver_Attack_04::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Weaver_Attack_04* pInstance = new CState_Weaver_Attack_04(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Weaver_Attack_04");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Weaver_Attack_04::Clone(void* pArg)
{
	CState_Weaver_Attack_04* pInstance = new CState_Weaver_Attack_04(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Weaver_Attack_04");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Weaver_Attack_04::Free()
{
	__super::Free();
}
