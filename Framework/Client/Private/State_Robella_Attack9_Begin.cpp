#include "stdafx.h"
#include "..\Public\State_Robella_Attack9_Begin.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"
#include "Enemy.h"

#include "Easing_Utillity.h"
#include "Effect_Manager.h"

CState_Robella_Attack9_Begin::CState_Robella_Attack9_Begin(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Robella_Attack9_Begin::CState_Robella_Attack9_Begin(const CState_Robella_Attack9_Begin& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Robella_Attack9_Begin::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_ATTACK_09_END);

	m_ReserveProbabilitys[STATE_BOSS_ROBELLA_ATTACK_09_END] = 1;

	return S_OK;
}

HRESULT CState_Robella_Attack9_Begin::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Robella_Attack9_Begin::Enter(void* pArg)
{
	__super::Enter(pArg);
	
	m_pColCom = (CCollider*)((CEnemy*)m_pState_Handler->Get_Owner())->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK));
}

void CState_Robella_Attack9_Begin::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	//Hit_Unit(m_pColCom, ELEMENTAL_FIRE, m_fDamage, 0, 30, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit);

	// 해당 클래스의 애니메이션이 끝나면 Idle_state로 넘어감
	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	if (45 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Robella_Tornado_Voice.wav"),
			CSound_Manager::CHANNEL_ENEMY, m_pOwner->Get_Pos(), 0.1f));
	}

	// 꽃기둥
	if (51 < Get_CurKeyFrame() && !m_bCreate)
	{
		//// 플레이어의 위치
		//m_pPlayer = (CPlayer*)PLAYERPTR;
		//CTransform* pPlayerTransform = m_pPlayer->Get_Transform();
		//
		//m_vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_TRANSLATION);
		//_float fPlayerPosX = XMVectorGetX(m_vPlayerPos);
		////_float fPlayerPosZ = XMVectorGetZ(m_vPlayerPos);
		//
		//// 로벨라의 위치
		//_vector vRobellaPos = m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION);
		//
		//_float fRobellaPosX = XMVectorGetX(vRobellaPos);
		////_float fRobellaPosZ = XMVectorGetZ(vRobellaPos);
		//
		//// 거리 계산
		//_float PosX = fRobellaPosX - fPlayerPosX;
		////_float PosZ = fRobellaPosZ - fPlayerPosZ;
		//
		//// 포스 회전 스케일
		//IMPL_ONCE(m_bEffect0, EFFECT_POS("Robella_Explo", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
		//	XMVectorSet(0.f, -1.49f, PosX, 1.f),
		//	XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f,
		//	XMVectorSet(1.f, 1.5f, 1.f, 0.f)));

		pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_ROBELLA_FIRESTROM2), ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_ROBELLA_FIRESTROM2), m_pTarget);
		m_bCreate = true;
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Robella_Attack9_Begin::Exit(void* pArg)
{
	m_bCreate = false;
	RESET_ONCE(m_bEffect0);
	m_fAccTime = 0.f;
}

_bool CState_Robella_Attack9_Begin::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Robella_Attack9_Begin* CState_Robella_Attack9_Begin::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Robella_Attack9_Begin* pInstance = new CState_Robella_Attack9_Begin(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Robella_Attack9_Begin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Robella_Attack9_Begin::Clone(void* pArg)
{
	CState_Robella_Attack9_Begin* pInstance = new CState_Robella_Attack9_Begin(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Robella_Attack9_Begin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Robella_Attack9_Begin::Free()
{
	m_pPlayer = nullptr;

	__super::Free();
}
