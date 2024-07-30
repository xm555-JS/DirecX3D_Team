#include "stdafx.h"
#include "..\Public\State_Dragon_Attack8.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"
#include "Enemy_Boss_Dragon.h"

#include "Imgui_Manager.h"

CState_Dragon_Attack8::CState_Dragon_Attack8(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Dragon_Attack8::CState_Dragon_Attack8(const CState_Dragon_Attack8& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Dragon_Attack8::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_DRAGON_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_DRAGON_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_DRAGON_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Dragon_Attack8::Initialize(void* pArg)
{
	m_bOnceSkill = true; 
	return S_OK;
}

void CState_Dragon_Attack8::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fDamage = fRandom(720.f, 880.f);
	m_pColCom = ((CCollider*)m_pOwner->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK, 1)));
}

void CState_Dragon_Attack8::Tick(_float fTimeDelta)
{
	IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Roar_Fall.wav"), CSound_Manager::CHANNEL_ENEMY, m_pOwner->Get_Pos(), 1.f));

	if (11 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect0, EFFECT_POS("Boss_Dragon_Tornado", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, -6.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f, XMVectorSet(1.5f, 1.5f, 1.5f, 0.f)));
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySound(TEXT("Tornado.mp3"), CSound_Manager::CHANNEL_ENEMY_EFFECT, 0.8f));
	}

	USEGAMEINSTANCE;
	m_fAnimSpeed = 1.f;

	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, m_fDamage, 0, 160, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit);

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	STATE_TYPE	eNextState = CState_AI::Ready_NextState();
	if (STATE_END != eNextState)
	{
		m_pState_Handler->Change_State(eNextState, &m_eState);
	}

	m_pTargetModel->Play_Animation(fTimeDelta * m_fAnimSpeed);
}

void CState_Dragon_Attack8::Exit(void* pArg)
{
	m_pTargetModel->Set_ModelEasingRatio(0.2f);
	m_bHit = false;
	RESET_ONCE(m_bEffect0);
}

_bool CState_Dragon_Attack8::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Dragon_Attack8* CState_Dragon_Attack8::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Dragon_Attack8* pInstance = new CState_Dragon_Attack8(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Dragon_Attack8");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Dragon_Attack8::Clone(void* pArg)
{
	CState_Dragon_Attack8* pInstance = new CState_Dragon_Attack8(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Dragon_Attack8");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Dragon_Attack8::Free()
{
	__super::Free();
}
