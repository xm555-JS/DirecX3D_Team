#include "stdafx.h"
#include "..\Public\State_Player_Sword_Skill_Special_C.h"
#include "GameInstance.h"
#include "Channel.h"
#include "Player.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"

CState_Player_Sword_Skill_Special_C::CState_Player_Sword_Skill_Special_C(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Sword_Skill_Special_C::CState_Player_Sword_Skill_Special_C(const CState_Player_Sword_Skill_Special_C& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Sword_Skill_Special_C::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_JUMP_A);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_SWORD_SKILL_SPECIAL_D);

	return S_OK;
}

HRESULT CState_Player_Sword_Skill_Special_C::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Sword_Skill_Special_C::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;

	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(true);

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());

	XMStoreFloat4x4(&m_ColMatrix, XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix()) * XMMatrixTranslation(0.f, -2.f, 0.f));
}

void CState_Player_Sword_Skill_Special_C::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_pColCom->Update(XMLoadFloat4x4(&m_ColMatrix));
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 10.f);

	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(100.f, 150.f), 5, 10, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0);
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(100.f, 150.f), 13, 19, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit1);
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(100.f, 150.f), 22, 28, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit2);
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(100.f, 150.f), 30, 36, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit3);

	if (1 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_SwordSkill_Special_C_1.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect0,
			EFFECT_POS("Player_Sword_Attack_A", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(1.8f, 1.7, 2.f, 1.f),
				XMVectorSet(XMConvertToRadians(160.f), XMConvertToRadians(90.f), XMConvertToRadians(0.f), 0.f),
				1.5f);
		EFFECT_POS("Player_Gun_Attack_Foot", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(1.2f, 0.7f, 1.3f, 1.f),
			XMVectorSet(0.f, XMConvertToRadians(50.f), 0.f, 0.f),
			1.5f,
			XMVectorSet(0.7f, 0.7f, 0.7f, 1.f));
		);
	}
	if (13 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_SwordSkill_Special_C_2.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect1,
			EFFECT_POS("Player_Sword_Attack_A", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(-1.6f, 0.f, -1.2f, 1.f),
				XMVectorSet(XMConvertToRadians(0.f), XMConvertToRadians(150.f), XMConvertToRadians(0.f), 0.f),
				1.5f);
		EFFECT_POS("Player_Gun_Attack_Foot", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(-1.5f, 0.7f, -2.5f, 1.f),
			XMVectorSet(0.f, XMConvertToRadians(230.f), 0.f, 0.f),
			1.5f,
			XMVectorSet(0.7f, 0.7f, 0.7f, 1.f));
		);
	}
	if (22 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[2], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_SwordSkill_Special_C_3.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect2,
			EFFECT_POS("Player_Sword_Attack_A", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(0.3f, 1.8f, 3.f, 1.f),
				XMVectorSet(XMConvertToRadians(200), XMConvertToRadians(90), 0.f, 0.f),
				1.5f);
		EFFECT_POS("Player_Gun_Attack_Foot", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 0.7f, 2.3f, 1.f),
			XMVectorSet(0.f, XMConvertToRadians(25.f), 0.f, 0.f),
			1.5f,
			XMVectorSet(0.7f, 0.7f, 0.7f, 1.f));
		);
	}
	if (26 < Get_CurKeyFrame())
	{

		IMPL_ONCE(m_bEffect3,
			EFFECT_POS("Player_Sword_Attack_A", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(-0.5f, 1.5f, -3.f, 1.f),
				XMVectorSet(XMConvertToRadians(90.f), XMConvertToRadians(90.f), 0.f, 0.f),
				1.5f)
		);
	}
	if (35 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[3], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_SwordSkill_Special_C_4.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));


		IMPL_ONCE(m_bEffect4,
			EFFECT_POS("Player_Gun_Attack_Foot", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(0.f, 0.7f, 0.f, 1.f),
				XMVectorSet(0.f, XMConvertToRadians(180), 0.f, 0.f),
				1.5f,
				XMVectorSet(0.7f, 0.7f, 0.7f, 1.f));
		);
	}

	if (45 < Get_CurKeyFrame())
	{
		m_bEndCurState = true;
	}

	


	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Sword_Skill_Special_C::Exit(void* pArg)
{
	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(false);
	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	__super::Exit();
}

_bool CState_Player_Sword_Skill_Special_C::Check_Condition(CState_Handler* pHandler)
{
	return true;
}


CState_Player_Sword_Skill_Special_C* CState_Player_Sword_Skill_Special_C::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Sword_Skill_Special_C* pInstance = new CState_Player_Sword_Skill_Special_C(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Sword_Skill_Special_C");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Sword_Skill_Special_C::Clone(void* pArg)
{
	CState_Player_Sword_Skill_Special_C* pInstance = new CState_Player_Sword_Skill_Special_C(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Sword_Skill_Special_C");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Sword_Skill_Special_C::Free()
{
}
