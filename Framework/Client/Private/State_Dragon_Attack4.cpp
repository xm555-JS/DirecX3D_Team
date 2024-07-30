#include "stdafx.h"
#include "..\Public\State_Dragon_Attack4.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Model.h"
#include "Animation.h"
#include "Channel.h"
#include "Player.h"
#include "Enemy_Boss_Dragon.h"

#include "Bullet_Normal.h"
#include "Enemy_Thunder_Dragon.h"

CState_Dragon_Attack4::CState_Dragon_Attack4(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Dragon_Attack4::CState_Dragon_Attack4(const CState_Dragon_Attack4& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Dragon_Attack4::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_DRAGON_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_DRAGON_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_DRAGON_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Dragon_Attack4::Initialize(void* pArg)
{
	m_bOnceSkill = true;
	return S_OK;
}

void CState_Dragon_Attack4::Enter(void* pArg)
{
	__super::Enter(pArg);

	//XMStoreFloat4(&m_vPos, m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION));

	if (((CState_Boss*)m_pState_Handler->Get_State(STATE_BOSS_DRAGON_ULT_ATTACK_1))->Get_ChangePhase() && !m_bChangePhase)
	{
		m_bChangePhase = true;
	}
}

void CState_Dragon_Attack4::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	m_fAnimSpeed = 1.f;

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySound(TEXT("Roar_Fall.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, 0.1f));

	m_fAccTime += fTimeDelta;

	if (0.2f < m_fAccTime)
	{
		XMStoreFloat4(&m_vThunderPos, m_pState_Handler->Get_Player()->Get_Pos());
		for(_uint i = 0; i < 4; ++i)
		{
			m_vThunderPos.x += fRandom(-10.f, 10.f);
			m_vThunderPos.z += fRandom(-10.f, 10.f);
			m_vThunderPos.w = (_float)m_bChangePhase; // Save Phase State
			GAMEINSTANCE->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_BULLET_THUNDER), ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_BULLET_THUNDER), &m_vThunderPos);

			m_fAccTime = 0.f;
		}
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Dragon_Attack4::Exit(void* pArg)
{
	m_pTargetModel->Set_ModelEasingRatio(0.2f);
}

_bool CState_Dragon_Attack4::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	return true;
}

CState_Dragon_Attack4* CState_Dragon_Attack4::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Dragon_Attack4* pInstance = new CState_Dragon_Attack4(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Dragon_Attack4");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Dragon_Attack4::Clone(void* pArg)
{
	CState_Dragon_Attack4* pInstance = new CState_Dragon_Attack4(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Dragon_Attack4");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Dragon_Attack4::Free()
{
	__super::Free();
}
