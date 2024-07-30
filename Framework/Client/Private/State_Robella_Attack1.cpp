#include "stdafx.h"
#include "..\Public\State_Robella_Attack1.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"
#include "Bullet_Normal.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Robella_Attack1::CState_Robella_Attack1(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Robella_Attack1::CState_Robella_Attack1(const CState_Robella_Attack1& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Robella_Attack1::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_ROBELLA_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Robella_Attack1::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Robella_Attack1::Enter(void* pArg)
{
	__super::Enter(pArg);
}

void CState_Robella_Attack1::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	// 해당 클래스의 애니메이션이 끝나면 Idle_state로 넘어감
	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}
	if (65 < Get_CurKeyFrame())
		m_fAnimSpeed = 1.f;

	//65~90 fireball 사방으로 6개 한번
	for (_uint i = 0; i < 3; ++i)
	{
		if (65 + i * 8 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame() && !m_bAttack[i])
		{
			for (_uint j = 0; j < 6; ++j)
			{
				CBullet_Normal::BULLETDESC BulletDesc;
				BulletDesc.pTarget = m_pOwner;

				BulletDesc.fBulletSpeed = 4.f + 2.f * i;
				_matrix		RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(j * 60.f + i * 30.f));
				_matrix		WorldMatrix = RotationMatrix * m_pOwnerTranformCom->Get_WorldMatrix();

				WorldMatrix.r[0] = XMVector3Normalize(WorldMatrix.r[0]);
				WorldMatrix.r[1] = XMVector3Normalize(WorldMatrix.r[1]);
				WorldMatrix.r[2] = XMVector3Normalize(WorldMatrix.r[2]);
				WorldMatrix.r[3] += XMVector3Normalize(WorldMatrix.r[2]) + (XMVector3Normalize(WorldMatrix.r[1]) * 3.f);

				IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Add_FireBall.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.1f));
				RESET_ONCE(m_bSound[0]);

				XMStoreFloat4x4(&BulletDesc.WorldMatrix, WorldMatrix);
				pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_ROBELLA_FIREBALL), ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_ROBELLA_FIREBALL), &BulletDesc);
			}

			m_bAttack[i] = true;
		}
	}



	CState_AI::Tick(fTimeDelta);
}

void CState_Robella_Attack1::Exit(void* pArg)
{
	for(_uint i = 0; i < 3; ++i)
		m_bAttack[i] = false;
}

_bool CState_Robella_Attack1::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Robella_Attack1* CState_Robella_Attack1::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Robella_Attack1* pInstance = new CState_Robella_Attack1(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Robella_Attack1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Robella_Attack1::Clone(void* pArg)
{
	CState_Robella_Attack1* pInstance = new CState_Robella_Attack1(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Robella_Attack1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Robella_Attack1::Free()
{
	__super::Free();
}
