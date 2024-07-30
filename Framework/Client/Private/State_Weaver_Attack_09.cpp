#include "stdafx.h"
#include "..\Public\State_Weaver_Attack_09.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

#include "Effect_Manager.h"
#include "Imgui_Manager.h"
#include "HIerarchyNode.h"

CState_Weaver_Attack_09::CState_Weaver_Attack_09(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Weaver_Attack_09::CState_Weaver_Attack_09(const CState_Weaver_Attack_09& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Weaver_Attack_09::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_WEAVER_WEAK_BEGIN);
	m_ForceStates.push_back(STATE_BOSS_WEAVER_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_WEAVER_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_WEAVER_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Weaver_Attack_09::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Weaver_Attack_09::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAccTime = 0.f;

	m_fDamage = fRandom(1040.f, 1120.f);
	m_pColCom = ((CCollider*)m_pOwner->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK, 6)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());

	EFFECT_POS("Boss_Weaver_Attack_09_ShockWave_Aura", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 0.8f, -0.1f, 1.f));
	EFFECT_EXCUTE("Boss_Weaver_Attack_09_ShockWave_Charge_Knife", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS, "Bone019");
	EFFECT_EXCUTE("Boss_Weaver_Attack_09_ShockWave_Charge_Knife", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS, "Bone020");
	EFFECT_EXCUTE("Boss_Weaver_Attack_09_ShockWave_Charge_Knife", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS, "Bone021");
	EFFECT_EXCUTE("Boss_Weaver_Attack_09_ShockWave_Charge_Knife", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS, "Bone022");
	EFFECT_EXCUTE("Boss_Weaver_Attack_09_ShockWave_Charge_Knife", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS, "Bone023");
	EFFECT_EXCUTE("Boss_Weaver_Attack_09_ShockWave_Charge_Knife", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS, "Bone024");
}

void CState_Weaver_Attack_09::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;	
	IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_09_Begin.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));

	_matrix ColMatrix = XMLoadFloat4x4(m_pState_Handler->Get_Owner()->Get_WorldMatrix());
	ColMatrix.r[3] -= XMVector3Normalize(ColMatrix.r[2]);
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 20.f);
	m_pColCom->Update(ColMatrix);

	Hit_Unit(m_pColCom, ELEMENTAL_ICE, m_fDamage, 170, 350, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0);



	// Effect
	if (92 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect0,
			for (_uint i = 0; i < 3; i++)
				EFFECT_POS("Boss_Weaver_Attack_09_ShockWave_Charge_P", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 3.f, 1.7f, 1.f));
		);

		m_fTimeAcc_Effect0 += fTimeDelta;
		if (170.f >= Get_CurKeyFrame())
		{
			if (0.6f <= m_fTimeAcc_Effect0)
			{
				m_fTimeAcc_Effect0 = 0.f;
				EFFECT_POS("Boss_Weaver_Attack_09_ShockWave_Charge_Distortion", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 3.f, 1.7f, 1.f));
			}
		}

	}
	if (100 <= Get_CurKeyFrame())
	{
		if(170 > Get_CurKeyFrame())
		{
			IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_09_Charge.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
			IMPL_ALARM(m_fSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_09_Charge.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f), 0.7f, fTimeDelta);
		}

		IMPL_ONCE(m_bEffect1, EFFECT_POS("Boss_Weaver_Attack_09_ShockWave_Explosion_SnowFlake_A2", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 3.f, 1.7f, 1.f)));
	}


	if (170.f <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[2], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_09_End.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));

		m_fTimeAcc_Effect1 += fTimeDelta;
		if (250.f >= Get_CurKeyFrame())
		{
			if (0.7f <= m_fTimeAcc_Effect1)
			{
				m_fTimeAcc_Effect1 = 0.f;
				EFFECT_POS("Boss_Weaver_Attack_09_ShockWave_Explosion_Distortion", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 3.f, 1.7f, 1.f));
			}
		}
		IMPL_ONCE(m_bEffect2,
			for (_uint i = 0; i < 4; i++)
				EFFECT_POS("Boss_Weaver_Attack_09_ShockWave_Explosion_P", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 3.f, 1.7f, 1.f));
		);
	}
	if (100 >= Get_CurKeyFrame())
	{
		m_fTimeAcc_Effect2 += fTimeDelta;
		if (0.1f <= m_fTimeAcc_Effect2)
		{
			m_fTimeAcc_Effect2 = 0.f;
			EFFECT_EXCUTE("Boss_Weaver_Attack_09_ShockWave_Charge_Knife_P", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone019");
			EFFECT_EXCUTE("Boss_Weaver_Attack_09_ShockWave_Charge_Knife_P", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone020");
			EFFECT_EXCUTE("Boss_Weaver_Attack_09_ShockWave_Charge_Knife_P", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone021");
			EFFECT_EXCUTE("Boss_Weaver_Attack_09_ShockWave_Charge_Knife_P", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone022");
			EFFECT_EXCUTE("Boss_Weaver_Attack_09_ShockWave_Charge_Knife_P", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone023");
			EFFECT_EXCUTE("Boss_Weaver_Attack_09_ShockWave_Charge_Knife_P", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone024");
		}
	}

	m_fTimeAcc_Effect3 += fTimeDelta;
	if (0.02f < m_fTimeAcc_Effect3)
	{
		m_fTimeAcc_Effect3 = 0.f;
		Excute_Particles(m_iEffectNum0, "Bone019");
		Excute_Particles(m_iEffectNum0, "Bone020");
		Excute_Particles(m_iEffectNum0, "Bone021");
		Excute_Particles(m_iEffectNum0, "Bone022");
		Excute_Particles(m_iEffectNum0, "Bone023");
		Excute_Particles(m_iEffectNum0, "Bone024");

		m_iEffectNum0 += 1;
		if (3 < m_iEffectNum0)
			m_iEffectNum0 = 0;
	}

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	//ShockWave »ý¼º 100~350
	if (175 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame()
		&& !m_bCreateObj)
	{
		m_bCreateObj = true;
		//m_pShockWave = GAMEINSTANCE->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_WEAVER_HURRICANE), ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_WEAVER_HURRICANE));
	}

	if (250 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame()
		&& m_bCreateObj)
	{
		if (nullptr != m_pShockWave)
		{
			//GAMEINSTANCE->Add_DeadObject(m_pShockWave);
		}
	}


	CState_AI::Tick(fTimeDelta);
}

void CState_Weaver_Attack_09::Exit(void* pArg)
{
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));

	m_bHit0 = false;
	m_bHit1 = false;

	RESET_ONCE(m_bEffect0);
	RESET_ONCE(m_bEffect1);
	RESET_ONCE(m_bEffect2);
	RESET_ONCE(m_bEffect3);
	RESET_ONCE(m_bEffect4);
	RESET_ONCE(m_bEffect5);
	m_fTimeAcc_Effect0 = 0.f;
	m_fTimeAcc_Effect1 = 0.f;
	m_fTimeAcc_Effect2 = 0.f;
	m_fTimeAcc_Effect3 = 0.f;
	m_iEffectNum0 = 0;
}

_bool CState_Weaver_Attack_09::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

void CState_Weaver_Attack_09::Excute_Particles(_uint _iEffectNum0, char* _szBoneName)
{
	CHierarchyNode* pHirarchyNnode = static_cast<CModel*>((m_pState_Handler->Get_Owner()->Get_Component(ID_UINT(MODEL_DYNAMIC))))->Find_HierarcyNode(_szBoneName);
	_matrix matFollow = pHirarchyNnode->Get_CombinedMatrix() * pHirarchyNnode->Get_TransformMatrix() * XMLoadFloat4x4(m_pState_Handler->Get_Owner()->Get_WorldMatrix());
	matFollow.r[0] = XMVector3Normalize(matFollow.r[0]) * 3.f;
	matFollow.r[1] = XMVector3Normalize(matFollow.r[1]) * 3.f;
	matFollow.r[2] = XMVector3Normalize(matFollow.r[2]) * 3.f;

	if (0 == _iEffectNum0)
	{
		EFFECT_EXCUTE("Player_Cube_Object_P1", matFollow);
	}
	else if (1 == _iEffectNum0)
	{
		EFFECT_EXCUTE("Player_Cube_Object_P2", matFollow);
	}
	else if (2 == _iEffectNum0)
	{
		EFFECT_EXCUTE("Player_Cube_Object_P3", matFollow);
	}
	else if (3 == _iEffectNum0)
	{
		EFFECT_EXCUTE("Player_Cube_Object_P4", matFollow);
	}
}

CState_Weaver_Attack_09* CState_Weaver_Attack_09::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Weaver_Attack_09* pInstance = new CState_Weaver_Attack_09(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Weaver_Attack_09");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Weaver_Attack_09::Clone(void* pArg)
{
	CState_Weaver_Attack_09* pInstance = new CState_Weaver_Attack_09(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Weaver_Attack_09");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Weaver_Attack_09::Free()
{
	__super::Free();
}
