#include "stdafx.h"
#include "..\Public\State_Robella_Attack7.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"
#include "Bullet_Normal.h"
#include "Enemy_Robella_FireBall2.h"

#include "Unit.h"

#include "Easing_Utillity.h"

#include "Imgui_Manager.h"

CState_Robella_Attack7::CState_Robella_Attack7(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Robella_Attack7::CState_Robella_Attack7(const CState_Robella_Attack7& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Robella_Attack7::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_IDLE2);

	m_ReserveProbabilitys[STATE_BOSS_ROBELLA_IDLE2] = 1;

	return S_OK;
}

HRESULT CState_Robella_Attack7::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Robella_Attack7::Enter(void* pArg)
{
	__super::Enter(pArg);
}

void CState_Robella_Attack7::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	// 해당 클래스의 애니메이션이 끝나면 Idle_state로 넘어감
	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	if (20 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect0, EFFECT_EXCUTE("Boss_Robella_FireBall2_Ready", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS, "Bone_eyes"));
	}
	for (_uint i = 0; i < 7; ++i)
	{
	//파이어볼 20에서 생성 55에서 날라가기 7개
		if (20 + i * 3.f < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame() && !m_bCreate[i])
		{
			CBullet_Normal::BULLETDESC BulletDesc;
			BulletDesc.pTarget = m_pTarget;

			BulletDesc.fBulletSpeed = 10.f;
			_matrix		RotationMatrix = XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(270.f));
			RotationMatrix *= XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));
			_matrix		WorldMatrix = RotationMatrix * m_pOwnerTranformCom->Get_WorldMatrix();

			WorldMatrix.r[0] = XMVector3Normalize(WorldMatrix.r[0]);
			WorldMatrix.r[1] = XMVector3Normalize(WorldMatrix.r[1]);
			WorldMatrix.r[2] = XMVector3Normalize(WorldMatrix.r[2]);
			WorldMatrix.r[3] += XMVectorSet(0.f, 7.f, 0.f, 0.f);

			XMStoreFloat4x4(&BulletDesc.WorldMatrix, WorldMatrix);

			_matrix		TargetRotationMatrix = XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(i * -30.f + 360.f));
			TargetRotationMatrix *= XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));
			_matrix		TargetWorldMatrix = TargetRotationMatrix * m_pOwnerTranformCom->Get_WorldMatrix();

			TargetWorldMatrix.r[3] += XMVectorSet(0.f, 7.f, 0.f, 0.f) + XMVector3Normalize(TargetWorldMatrix.r[2]) * 3.f;

			XMStoreFloat4(&BulletDesc.TargetPos, TargetWorldMatrix.r[3]);

			m_pBullet[i] = (CEnemy_Robella_FireBall2*)pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_ROBELLA_FIREBALL2), ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_ROBELLA_FIREBALL2), &BulletDesc);

			m_bCreate[i] = true;

			IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Add_FireBall_2Phase.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.3f));
			RESET_ONCE(m_bSound[0]);
		}

	}

	for (_uint i = 0; i < 7; ++i)
	{
		if (55 + i * 5.f < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame() && !m_bAttack[i])
		{
			m_pBullet[i]->Set_Shoot();
			m_bAttack[i] = true;
		}
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Robella_Attack7::Exit(void* pArg)
{
	for (_uint i = 0; i < 7; ++i)
	{
		m_bCreate[i] = false;
		m_bAttack[i] = false;
	}
	RESET_ONCE(m_bEffect0);
}

_bool CState_Robella_Attack7::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Robella_Attack7* CState_Robella_Attack7::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Robella_Attack7* pInstance = new CState_Robella_Attack7(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Robella_Attack7");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Robella_Attack7::Clone(void* pArg)
{
	CState_Robella_Attack7* pInstance = new CState_Robella_Attack7(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Robella_Attack7");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Robella_Attack7::Free()
{
	__super::Free();
}
