#include "stdafx.h"
#include "..\Public\State_Weaver_Attack_02.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"
#include "HIerarchyNode.h"

#include "Unit.h"

#include "Easing_Utillity.h"
#include "Imgui_Manager.h"

#include "Effect_Manager.h"

CState_Weaver_Attack_02::CState_Weaver_Attack_02(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Weaver_Attack_02::CState_Weaver_Attack_02(const CState_Weaver_Attack_02& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Weaver_Attack_02::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_WEAVER_WEAK_BEGIN);
	m_ForceStates.push_back(STATE_BOSS_WEAVER_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_WEAVER_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_WEAVER_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Weaver_Attack_02::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Weaver_Attack_02::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAccTime = 0.f;
	m_fTimeAcc = 0.f;

	m_fDamage = fRandom(450.f, 550.f);
	m_pColCom = ((CCollider*)m_pOwner->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK, 6)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());

	EFFECT_POS("Boss_Weaver_Attack_09_ShockWave_Aura", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 0.8f, -0.1f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.3f);
}

void CState_Weaver_Attack_02::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
#if 0 // 레이저 공격에 달음(Attack_07)
	if (55 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect7,
			EFFECT_POS("Boss_Weaver_Attack_02_KnifeAttack_Sphere", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 6.2f, 0.f, 1.f));
		)
	}
#endif // 0 // 레이저 공격에 달음(Attack_07)

	if (48 <= Get_CurKeyFrame())
	{
#if 0	// 0.1 초 간격으로 생성
		m_fTimeAcc_Effect0 += fTimeDelta;
		if (0.2f * 1.f < m_fTimeAcc_Effect0)
			IMPL_ONCE(m_bEffect1, EFFECT_EXCUTE("Boss_Weaver_Attack_02_KnifeAttack_Knife", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone019"));
		if (0.2f * 2.f < m_fTimeAcc_Effect0)
			IMPL_ONCE(m_bEffect2, EFFECT_EXCUTE("Boss_Weaver_Attack_02_KnifeAttack_Knife", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone020"));
		if (0.2f * 3.f < m_fTimeAcc_Effect0)
			IMPL_ONCE(m_bEffect3, EFFECT_EXCUTE("Boss_Weaver_Attack_02_KnifeAttack_Knife", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone021"));
		if (0.2f * 4.f < m_fTimeAcc_Effect0)
			IMPL_ONCE(m_bEffect4, EFFECT_EXCUTE("Boss_Weaver_Attack_02_KnifeAttack_Knife", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone022"));
		if (0.2f * 5.f < m_fTimeAcc_Effect0)
			IMPL_ONCE(m_bEffect5, EFFECT_EXCUTE("Boss_Weaver_Attack_02_KnifeAttack_Knife", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone023"));
		if (0.2f * 6.f < m_fTimeAcc_Effect0)
			IMPL_ONCE(m_bEffect6, EFFECT_EXCUTE("Boss_Weaver_Attack_02_KnifeAttack_Knife", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone024"));
#endif // 0
		// 동시에 생성
		IMPL_ONCE(m_bEffect1, EFFECT_EXCUTE("Boss_Weaver_Attack_02_KnifeAttack_Knife", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone019"));
		IMPL_ONCE(m_bEffect2, EFFECT_EXCUTE("Boss_Weaver_Attack_02_KnifeAttack_Knife", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone020"));
		IMPL_ONCE(m_bEffect3, EFFECT_EXCUTE("Boss_Weaver_Attack_02_KnifeAttack_Knife", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone021"));
		IMPL_ONCE(m_bEffect4, EFFECT_EXCUTE("Boss_Weaver_Attack_02_KnifeAttack_Knife", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone022"));
		IMPL_ONCE(m_bEffect5, EFFECT_EXCUTE("Boss_Weaver_Attack_02_KnifeAttack_Knife", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone023"));
		IMPL_ONCE(m_bEffect6, EFFECT_EXCUTE("Boss_Weaver_Attack_02_KnifeAttack_Knife", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone024"));

	}
	m_fTimeAcc_Effect1 += fTimeDelta;
	if (0.02f < m_fTimeAcc_Effect1)
	{
		m_fTimeAcc_Effect1 = 0.f;
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

	_matrix ColMatrix = XMLoadFloat4x4(m_pState_Handler->Get_Owner()->Get_WorldMatrix());
	ColMatrix.r[3] -= XMVector3Normalize(ColMatrix.r[2]) * 2.f;
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 10.f);
	m_pColCom->Update(ColMatrix);

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;
	IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_02_Begin.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));

	if(75 < Get_CurKeyFrame()
		&& 200 > Get_CurKeyFrame())
	{ 
		IMPL_ALARM(m_fSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_02_2.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f), 0.3f, fTimeDelta);

	}

	if (0.5f * m_iCount < m_pState_Handler->Get_TimeAcc())
	{
		Hit_Unit(m_pColCom, ELEMENTAL_FIRE, m_fDamage, 75, 200, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit);
		m_bHit = false;
		++m_iCount;
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Weaver_Attack_02::Exit(void* pArg)
{
	m_iCount = 0;
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));

	RESET_ONCE(m_bEffect0);
	RESET_ONCE(m_bEffect1);
	RESET_ONCE(m_bEffect2);
	RESET_ONCE(m_bEffect3);
	RESET_ONCE(m_bEffect4);
	RESET_ONCE(m_bEffect5);
	RESET_ONCE(m_bEffect6);
	RESET_ONCE(m_bEffect7);
	m_fTimeAcc_Effect0 = 0.f;
	m_iEffectNum0 = 0.f;
}

_bool CState_Weaver_Attack_02::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

void CState_Weaver_Attack_02::Excute_Particles(_uint _iEffectNum0, char* _szBoneName)
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

CState_Weaver_Attack_02* CState_Weaver_Attack_02::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Weaver_Attack_02* pInstance = new CState_Weaver_Attack_02(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Weaver_Attack_02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Weaver_Attack_02::Clone(void* pArg)
{
	CState_Weaver_Attack_02* pInstance = new CState_Weaver_Attack_02(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Weaver_Attack_02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Weaver_Attack_02::Free()
{
	__super::Free();
}
