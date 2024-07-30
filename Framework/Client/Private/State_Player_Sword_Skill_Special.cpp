#include "stdafx.h"
#include "..\Public\State_Player_Sword_Skill_Special.h"
#include "GameInstance.h"
#include "Channel.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"
#include "Navigation.h"

CState_Player_Sword_Skill_Special::CState_Player_Sword_Skill_Special(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Sword_Skill_Special::CState_Player_Sword_Skill_Special(const CState_Player_Sword_Skill_Special& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Sword_Skill_Special::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_JUMP_A);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_SWORD_SKILL_SPECIAL_A);

	return S_OK;
}

HRESULT CState_Player_Sword_Skill_Special::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Sword_Skill_Special::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 1.5f;
	m_fKeyPressTimeAcc = 0.f;

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Player_Sword_Skill_Special::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	IMPL_ONCE(m_bShaked0,
		pGameInstance->Add_Camera_Shaking({ 1.f,0.f,0.f,0.f }, 0.1f, 0.4f, 0.6f));
	if (6 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_SwordSkill_Special.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect0,
			EFFECT_POS("Player_Sword_Attack_A", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(-0.8f, 2.2f, 0.5f, 1.f),
				XMVectorSet(XMConvertToRadians(90.f), XMConvertToRadians(90.f), 0.f, 0.f),
				1.5f
			)
		);
	}

	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(130.f, 170.f), 7, 20, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0, "Player_Sword_Hit_Degree90");
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 4.f);

	if (20 < Get_CurKeyFrame())
	{
		m_bEndCurState = true;
	}

	if (7 < Get_CurKeyFrame() && !m_bJump)
	{
		CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
		m_pState_Handler->Get_Player()->Get_Transform()->Jump(8.f, pPlayerNavi);
		m_bJump = true;
	}
	

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Sword_Skill_Special::Exit(void* pArg)
{
	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	m_bJump = false;
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	__super::Exit();
}

_bool CState_Player_Sword_Skill_Special::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (KEY_DOWN(VK_LBUTTON))
	{
		return true;
	}

	return false;
}


CState_Player_Sword_Skill_Special* CState_Player_Sword_Skill_Special::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Sword_Skill_Special* pInstance = new CState_Player_Sword_Skill_Special(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Sword_Skill_Special");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Sword_Skill_Special::Clone(void* pArg)
{
	CState_Player_Sword_Skill_Special* pInstance = new CState_Player_Sword_Skill_Special(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Sword_Skill_Special");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Sword_Skill_Special::Free()
{
}
