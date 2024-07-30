#include "stdafx.h"
#include "..\Public\State_Dragon_Attack5.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Channel.h"
#include "Player.h"
#include "Enemy_Boss_Dragon.h"

CState_Dragon_Attack5::CState_Dragon_Attack5(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Dragon_Attack5::CState_Dragon_Attack5(const CState_Dragon_Attack5& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Dragon_Attack5::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_DRAGON_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_DRAGON_DOEND);

	m_ReserveProbabilitys[STATE_BOSS_DRAGON_DOEND] = 1;

	return S_OK;
}

HRESULT CState_Dragon_Attack5::Initialize(void* pArg)
{
	m_bOnceSkill = true;
	m_fDamage = fRandom(810.f, 990.f);
	return S_OK;
}

void CState_Dragon_Attack5::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_pTargetModel->Set_ModelEasingRatio(0.1f);

	m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Set_KeyFrame(8);

	m_pColCom = ((CCollider*)m_pOwner->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK)));
}

void CState_Dragon_Attack5::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	m_fAnimSpeed = 0.5f;

	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, m_fDamage, 0, 30, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit);

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	_float fTickperSec = fTimeDelta * m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_TickperSecond();	//1프레임당 시간
	m_pOwnerTranformCom->Go_Front(fTickperSec, 2.f);

	//30
	if (30 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_pOwnerTranformCom->Go_Up(fTickperSec, 2.f);
		_float4 vColor = ((CEnemy_Boss_FrostDragon*)m_pOwner)->Get_Color();
		vColor.w -= fTimeDelta;

		((CEnemy_Boss_FrostDragon*)m_pOwner)->Set_Color(vColor);
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Dragon_Attack5::Exit(void* pArg)
{
	_float4 vColor = ((CEnemy_Boss_FrostDragon*)m_pOwner)->Get_Color();
	vColor.w = 0.f;

	((CEnemy_Boss_FrostDragon*)m_pOwner)->Set_Color(vColor);
	m_bHit = false;
}

_bool CState_Dragon_Attack5::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Dragon_Attack5* CState_Dragon_Attack5::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Dragon_Attack5* pInstance = new CState_Dragon_Attack5(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Dragon_Attack5");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Dragon_Attack5::Clone(void* pArg)
{
	CState_Dragon_Attack5* pInstance = new CState_Dragon_Attack5(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Dragon_Attack5");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Dragon_Attack5::Free()
{
	__super::Free();
}
