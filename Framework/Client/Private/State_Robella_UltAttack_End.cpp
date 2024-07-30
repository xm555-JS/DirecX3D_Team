#include "stdafx.h"
#include "..\Public\State_Robella_UltAttack_End.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

#include "Effect_Manager.h"
#include "Imgui_Manager.h"

#include "Enemy_Robella_PoisonFog.h"

CState_Robella_UltAttack_End::CState_Robella_UltAttack_End(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Robella_UltAttack_End::CState_Robella_UltAttack_End(const CState_Robella_UltAttack_End& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Robella_UltAttack_End::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_IDLE2);

	m_ReserveProbabilitys[STATE_BOSS_ROBELLA_IDLE2] = 1;

	return S_OK;
}

HRESULT CState_Robella_UltAttack_End::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Robella_UltAttack_End::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fDamage = fRandom(150.f, 250.f);

	m_pColCom = ((CCollider*)m_pOwner->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Robella_UltAttack_End::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	// 해당 클래스의 애니메이션이 끝나면 Idle_state로 넘어감
	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	_matrix ColMatrix = XMLoadFloat4x4(m_pState_Handler->Get_Owner()->Get_WorldMatrix());
	ColMatrix.r[3] -= XMVector3Normalize(ColMatrix.r[2]);
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 30.f);
	m_pColCom->Update(ColMatrix);

	for(_uint i = 0; i < 20; ++i)//115
		Hit_Unit(m_pColCom, ELEMENTAL_FIRE, m_fDamage, 110 + i * 6 , 230, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[i]);

	if (25 < Get_CurKeyFrame())
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Poison_Charge.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));

	//110~230
	// 독안개.
	if (41 < Get_CurKeyFrame())
		
		IMPL_ONCE(m_bEffect1, 
			EFFECT_POS("Boss_Robella_Poision_Mark", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 0.f, 0.f, 1.f), 
				XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f, XMVectorSet(3.f, 3.f, 3.f, 1.f))
		);
	if (68 < Get_CurKeyFrame())
		IMPL_ONCE(m_bEffect2,
			EFFECT_POS("Boss_Robella_Poision_Mark", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 0.f, 0.f, 1.f),
				XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f, XMVectorSet(3.f, 3.f, 3.f, 1.f))
		);
	if (108 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect3, EFFECT_POS("Boss_Robella_Poision_Ready", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 6.f, 0.f, 1.f)));
		
		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Robella_Poison.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bSound[2], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Robella_Whip_Voice.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
	}

	if (115 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect0,
			CGameObject * pPoisonFogEffect = (pGameInstance->Add_GameObject(pGameInstance->Get_CurLevelIndex(), ID_UINT(ENEMY, ENEMY_BULLET), ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_ROBELLA_POISONFOG), m_pState_Handler->Get_Owner()));
		NULLCHECK(pPoisonFogEffect);
		static_cast<CEnemy_Robella_PoisonFog*>(pPoisonFogEffect)->Set_FollowingMode(XMMatrixIdentity(), 1.f, (CGameObject*)pGameInstance->Get_CurrentCamera(), CEffect_Manager::FOLLOW_POS);
		);
		// IMPL_ONCE(m_bEffect0, EFFECT_EXCUTE("Boss_Robella_Poision", XMMatrixIdentity(), 1.f, (CGameObject*)pGameInstance->Get_CurrentCamera(), CEffect_Manager::FOLLOW_POS))
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Robella_UltAttack_End::Exit(void* pArg)
{
	for (_uint i = 0; i < 20; ++i)
		m_bHit[i] = false;

	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	RESET_ONCE(m_bEffect0);
	RESET_ONCE(m_bEffect1);
	RESET_ONCE(m_bEffect2);
	RESET_ONCE(m_bEffect3);
}

_bool CState_Robella_UltAttack_End::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Robella_UltAttack_End* CState_Robella_UltAttack_End::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Robella_UltAttack_End* pInstance = new CState_Robella_UltAttack_End(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Robella_UltAttack_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Robella_UltAttack_End::Clone(void* pArg)
{
	CState_Robella_UltAttack_End* pInstance = new CState_Robella_UltAttack_End(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Robella_UltAttack_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Robella_UltAttack_End::Free()
{
	__super::Free();
}
