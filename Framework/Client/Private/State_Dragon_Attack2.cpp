#include "stdafx.h"
#include "..\Public\State_Dragon_Attack2.h"
#include "GameInstance.h"
#include "Model.h"

#include "Channel.h"
#include "Animation.h"
#include "Player.h"

#include "Enemy_Boss_Dragon.h"
#include "Bullet_Normal.h"

CState_Dragon_Attack2::CState_Dragon_Attack2(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Dragon_Attack2::CState_Dragon_Attack2(const CState_Dragon_Attack2& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Dragon_Attack2::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_DRAGON_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_DRAGON_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_DRAGON_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Dragon_Attack2::Initialize(void* pArg)
{
	m_bOnceSkill = true;
	return S_OK;
}

void CState_Dragon_Attack2::Enter(void* pArg)
{
	__super::Enter(pArg);
}

void CState_Dragon_Attack2::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	m_fAnimSpeed = 1.f;

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySound(TEXT("Dragon_Roar2.mp3"), CSound_Manager::CHANNEL_ENEMY_EFFECT, 0.3f));

	for (_uint i = 0; i < 8; ++i)
	{
		CBullet_Normal::BULLETDESC BulletDesc;
		BulletDesc.pTarget = m_pTarget;
		
		if (50 + (10 * i) < Get_CurKeyFrame() && !m_bAttack[i])
		{
			_float fRanSpeed = fRandom(5.f, 10.f);
			BulletDesc.fBulletSpeed = fRanSpeed;
			_matrix		RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(i * 45.f));
			_matrix		WorldMatrix = RotationMatrix * m_pOwnerTranformCom->Get_WorldMatrix();

			WorldMatrix.r[0] = XMVector3Normalize(WorldMatrix.r[0]);
			WorldMatrix.r[1] = XMVector3Normalize(WorldMatrix.r[1]);
			WorldMatrix.r[2] = XMVector3Normalize(WorldMatrix.r[2]);
			WorldMatrix.r[3] += XMVector3Normalize(WorldMatrix.r[2]);

			XMStoreFloat4x4(&BulletDesc.WorldMatrix, WorldMatrix);
			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_BULLET_FROSTDRAGON), ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_BULLET_FROSTDRAGON), &BulletDesc);

			m_bAttack[i] = true;

			IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySound(TEXT("Missile.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, 0.2f));
			RESET_ONCE(m_bSound[0]);
		}
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Dragon_Attack2::Exit(void* pArg)
{
	m_pTargetModel->Set_ModelEasingRatio(0.2f);
	for(_uint i = 0; i < 10; ++i)
		m_bAttack [i]= false;
}

_bool CState_Dragon_Attack2::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	return true;
}

CState_Dragon_Attack2* CState_Dragon_Attack2::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Dragon_Attack2* pInstance = new CState_Dragon_Attack2(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Dragon_Attack2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Dragon_Attack2::Clone(void* pArg)
{
	CState_Dragon_Attack2* pInstance = new CState_Dragon_Attack2(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Dragon_Attack2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Dragon_Attack2::Free()
{
	__super::Free();
}
