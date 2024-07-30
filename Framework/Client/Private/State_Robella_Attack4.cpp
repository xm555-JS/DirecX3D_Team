#include "stdafx.h"
#include "..\Public\State_Robella_Attack4.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"

CState_Robella_Attack4::CState_Robella_Attack4(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Robella_Attack4::CState_Robella_Attack4(const CState_Robella_Attack4& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Robella_Attack4::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_IDLE2);

	m_ReserveProbabilitys[STATE_BOSS_ROBELLA_IDLE2] = 1;

	return S_OK;
}

HRESULT CState_Robella_Attack4::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Robella_Attack4::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fDamage = fRandom(1200.f, 1300.f);

	m_pColCom = ((CCollider*)m_pOwner->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Robella_Attack4::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	_matrix ColMatrix = XMLoadFloat4x4(m_pState_Handler->Get_Owner()->Get_WorldMatrix());
	ColMatrix.r[3] -= XMVector3Normalize(ColMatrix.r[2]);
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 28.f);
	m_pColCom->Update(ColMatrix);

	// 해당 클래스의 애니메이션이 끝나면 Idle_state로 넘어감
	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	//65~110 주변 충격파 디스토션	//안되면 1페랑 같이 불똥처리
	Hit_Unit(m_pColCom, ELEMENTAL_FIRE, m_fDamage, 65, 110, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit);

	_matrix OffsetMatrix = XMMatrixIdentity();


	if (0 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect8, EFFECT_POS("Boss_Robella_SoundWave_Mark", m_pState_Handler->Get_Owner()->Get_WorldMatrix()));
		IMPL_ONCE(m_bEffect0, EFFECT_EXCUTE("Boss_Robella_SoundWave_Charge1", OffsetMatrix, 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone_eyes"));
		IMPL_ONCE(m_bEffect1, EFFECT_EXCUTE("Boss_Robella_SoundWave_Charge2", OffsetMatrix, 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS, "Bone_eyes"));
	
		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Fire_Charge.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
	}

	OffsetMatrix *= XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(45.f))/* * XMMatrixTranslation(1.5f, 0.f, 0.f)*/;
	if (5 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect2, EFFECT_EXCUTE("Boss_Robella_SoundWave_Charge1", OffsetMatrix, 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone_eyes"));
	}
	OffsetMatrix *= XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(45.f))/* * XMMatrixTranslation(1.5f, 0.f, 0.f)*/;
	if (10 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect3, EFFECT_EXCUTE("Boss_Robella_SoundWave_Charge1", OffsetMatrix, 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone_eyes"));
	}
	OffsetMatrix *= XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(45.f))/* * XMMatrixTranslation(1.5f, 0.f, 0.f)*/;
	if (15 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect4, EFFECT_EXCUTE("Boss_Robella_SoundWave_Charge1", OffsetMatrix, 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone_eyes"));
	}
	OffsetMatrix *= XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(45.f))/* * XMMatrixTranslation(1.5f, 0.f, 0.f)*/;
	if (25 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect5, EFFECT_EXCUTE("Boss_Robella_SoundWave_Charge1", OffsetMatrix, 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone_eyes"));
	}
	OffsetMatrix *= XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(45.f))/* * XMMatrixTranslation(1.5f, 0.f, 0.f)*/;
	if (35 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect6, EFFECT_EXCUTE("Boss_Robella_SoundWave_Charge1", OffsetMatrix, 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone_eyes"));
	}

	OffsetMatrix = XMMatrixIdentity();
	if (63.f < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect7, EFFECT_EXCUTE("Boss_Robella_SoundWave_Attack", OffsetMatrix, 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone_eyes"));
	
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("SoundWave1.mp3"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
	}


	CState_AI::Tick(fTimeDelta);
}

void CState_Robella_Attack4::Exit(void* pArg)
{
	m_bHit = false;
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	RESET_ONCE(m_bEffect0);
	RESET_ONCE(m_bEffect1);
	RESET_ONCE(m_bEffect2);
	RESET_ONCE(m_bEffect3);
	RESET_ONCE(m_bEffect4);
	RESET_ONCE(m_bEffect5);
	RESET_ONCE(m_bEffect6);
	RESET_ONCE(m_bEffect7);
	RESET_ONCE(m_bEffect8);
}

_bool CState_Robella_Attack4::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Robella_Attack4* CState_Robella_Attack4::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Robella_Attack4* pInstance = new CState_Robella_Attack4(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Robella_Attack4");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Robella_Attack4::Clone(void* pArg)
{
	CState_Robella_Attack4* pInstance = new CState_Robella_Attack4(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Robella_Attack4");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Robella_Attack4::Free()
{
	__super::Free();
}
