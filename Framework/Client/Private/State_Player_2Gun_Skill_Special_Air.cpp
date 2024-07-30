#include "stdafx.h"
#include "..\Public\State_Player_2Gun_Skill_Special_Air.h"
#include "GameInstance.h"
#include "Channel.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"


CState_Player_2Gun_Skill_Special_Air::CState_Player_2Gun_Skill_Special_Air(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_2Gun_Skill_Special_Air::CState_Player_2Gun_Skill_Special_Air(const CState_Player_2Gun_Skill_Special_Air& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_2Gun_Skill_Special_Air::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_JUMP_FALL);

	return S_OK;
}

HRESULT CState_Player_2Gun_Skill_Special_Air::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_2Gun_Skill_Special_Air::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;

	m_iCurKeyFrame = 0;

	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(true);

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
}

void CState_Player_2Gun_Skill_Special_Air::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

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
		IMPL_ONCE(m_bShaked0,
			pGameInstance->Add_DirCA(_float2(-1.f, 1.f), _float4(0.f, 0.02f, 0.01f, 0.f), 0.75f, 0.75f);
			pGameInstance->Add_Camera_Shaking({ 1.f,1.f,0.f,0.f }, 0.032f, 0.8f, 0.4f););
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
			pGameInstance->Add_Camera_Shaking({ 1.f,0.f,0.f,0.f }, 0.015f, 0.4f, 0.6f));
	}
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

void CState_Player_2Gun_Skill_Special_Air::Exit(void* pArg)
{
	m_bCreateBullet = false;

	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(false);

	__super::Exit();
}

_bool CState_Player_2Gun_Skill_Special_Air::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (KEY_DOWN(VK_LBUTTON))
	{
		return true;
	}

	return false;
}


CState_Player_2Gun_Skill_Special_Air* CState_Player_2Gun_Skill_Special_Air::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_2Gun_Skill_Special_Air* pInstance = new CState_Player_2Gun_Skill_Special_Air(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_2Gun_Skill_Special_Air");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_2Gun_Skill_Special_Air::Clone(void* pArg)
{
	CState_Player_2Gun_Skill_Special_Air* pInstance = new CState_Player_2Gun_Skill_Special_Air(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_2Gun_Skill_Special_Air");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_2Gun_Skill_Special_Air::Free()
{
}
