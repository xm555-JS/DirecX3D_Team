#include "stdafx.h"
#include "..\Public\State_Player_Sword_Skill_Special_D.h"
#include "GameInstance.h"
#include "Channel.h"
#include "Player.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"

#include "Imgui_Manager.h"

CState_Player_Sword_Skill_Special_D::CState_Player_Sword_Skill_Special_D(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Sword_Skill_Special_D::CState_Player_Sword_Skill_Special_D(const CState_Player_Sword_Skill_Special_D& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Sword_Skill_Special_D::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_JUMP_A);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_SWORD_SKILL_SPECIAL_E);

	return S_OK;
}

HRESULT CState_Player_Sword_Skill_Special_D::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Sword_Skill_Special_D::Enter(void* pArg)
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

void CState_Player_Sword_Skill_Special_D::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_pColCom->Update(XMLoadFloat4x4(&m_ColMatrix));
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 10.f);

	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(100.f, 150.f), 20, 27, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0);

	if (20 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_SwordSkill_Special_D.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(),0.5f));

		IMPL_ONCE(m_bEffect0,
			EFFECT_POS("Player_Sword_Attack_A", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(0.f, 2.1f, 1.f, 1.f),
				//XMVectorSet(XMConvertToRadians(-20.f), XMConvertToRadians(90.f), XMConvertToRadians(0.f), 0.f),
				XMVectorSet(XMConvertToRadians(200), XMConvertToRadians(90), 0.f, 0.f),
				1.5f);
		);

	}
	if (22 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bShaked0,
			pGameInstance->Add_DirCA(_float2(-1.f, 1.f), _float4(0.f, 0.015f, 0.075f, 0.f), 0.2f, 0.2f);
		pGameInstance->Add_Camera_Shaking(XMVectorSet(1.f, -1.f, 0.f, 0.f), 0.02f, 0.2f, 0.2f));
	}
	if (30 < Get_CurKeyFrame())
	{
		m_bEndCurState = true;
	}

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Sword_Skill_Special_D::Exit(void* pArg)
{
	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(false);
	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	__super::Exit();
}

_bool CState_Player_Sword_Skill_Special_D::Check_Condition(CState_Handler* pHandler)
{
	return true;
}


CState_Player_Sword_Skill_Special_D* CState_Player_Sword_Skill_Special_D::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Sword_Skill_Special_D* pInstance = new CState_Player_Sword_Skill_Special_D(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Sword_Skill_Special_D");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Sword_Skill_Special_D::Clone(void* pArg)
{
	CState_Player_Sword_Skill_Special_D* pInstance = new CState_Player_Sword_Skill_Special_D(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Sword_Skill_Special_D");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Sword_Skill_Special_D::Free()
{
}
