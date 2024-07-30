#include "stdafx.h"
#include "..\Public\State_Player_2Gun_Skill_Special.h"
#include "GameInstance.h"
#include "Channel.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"


CState_Player_2Gun_Skill_Special::CState_Player_2Gun_Skill_Special(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_2Gun_Skill_Special::CState_Player_2Gun_Skill_Special(const CState_Player_2Gun_Skill_Special& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_2Gun_Skill_Special::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_JUMP_A);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_2GUN_ATTACK_A);
	m_ReserveStates.push_back(STATE_PLAYER_WALK);
	m_ReserveStates.push_back(STATE_PLAYER_IDLE);

	return S_OK;
}

HRESULT CState_Player_2Gun_Skill_Special::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_2Gun_Skill_Special::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 1.7f;

	m_iCurKeyFrame = 0;

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
}

void CState_Player_2Gun_Skill_Special::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	if (50 <= Get_CurKeyFrame())
	{
		if (!m_bCreateBullet)	//ºÒ¸´»ý¼º
		{
			_matrix OffsetMatrix = XMMatrixIdentity();
			OffsetMatrix.r[3] = XMVectorSet(0.f, 0.7f, 5.f, 1.f);
			_matrix TargetMatrix = OffsetMatrix * XMLoadFloat4x4(m_pState_Handler->Get_Owner()->Get_WorldMatrix());
			_float4	vTargetPos;
			XMStoreFloat4(&vTargetPos, TargetMatrix.r[3]);
			pGameInstance->Add_GameObject(pGameInstance->Get_CurLevelIndex(), ID_UINT(PLAYER, WEAPON_2GUN_BULLET_SPECIAL), ID_UINT(PLAYER, WEAPON_2GUN_BULLET_SPECIAL), &vTargetPos);
			m_bCreateBullet = true;
		}

		IMPL_ONCE(m_bShaked0,
			pGameInstance->Add_DirCA(_float2(-1.f, 1.f), _float4(0.f, 0.02f, 0.01f, 0.f), 0.75f, 0.75f);
			pGameInstance->Add_Camera_Shaking({ 1.f,1.f,0.f,0.f }, 0.032f, 0.8f, 0.4f););
	}

	if (50 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("2Gun_Skill_Special_2.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect0,
			EFFECT_POS("Player_Gun_Attack_D",
				m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(0.15f, 1.1f, 0.8f, 1.f),
				XMVectorSet(XMConvertToRadians(90.f), 0.f, 0.f, 1.f),
				0.8f,
				XMVectorSet(3.f, 3.f, 4.f, 1.f)
			)
		);
	}
	if (4 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("2Gun_Skill_Special_1.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect1,
			//EFFECT_EXCUTE("Player_Gun_Converge_Black", XMMatrixIdentity(), 1.05f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS, "Bip001-R-Finger0Nub");
		EFFECT_EXCUTE("Player_Gun_Converge_P", XMMatrixIdentity(), 1.05f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS, "Bip001-R-Finger0Nub");
		EFFECT_POS("Player_Gun_Skill_Special_A",
			m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			0.6f,
			XMVectorSet(1.5f, 1.5f, 1.5f, 1.f));
		);

		IMPL_ONCE(m_bShaked1,
			pGameInstance->Add_Camera_Shaking({ 1.f,0.f,0.f,0.f }, 0.015f, 0.4f, 0.6f);
		);
	}

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	if (KEY_PRESSING(VK_LBUTTON))
		m_fKeyPressTimeAcc += fTimeDelta;
	else
		m_fKeyPressTimeAcc = 0.f;

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_2Gun_Skill_Special::Exit(void* pArg)
{
	m_bCreateBullet = false;
	__super::Exit();
}

_bool CState_Player_2Gun_Skill_Special::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (KEY_DOWN(VK_LBUTTON))
	{
		return true;
	}

	return false;
}


CState_Player_2Gun_Skill_Special* CState_Player_2Gun_Skill_Special::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_2Gun_Skill_Special* pInstance = new CState_Player_2Gun_Skill_Special(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_2Gun_Skill_Special");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_2Gun_Skill_Special::Clone(void* pArg)
{
	CState_Player_2Gun_Skill_Special* pInstance = new CState_Player_2Gun_Skill_Special(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_2Gun_Skill_Special");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_2Gun_Skill_Special::Free()
{
}
