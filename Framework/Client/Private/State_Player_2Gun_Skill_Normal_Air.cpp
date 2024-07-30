#include "stdafx.h"
#include "..\Public\State_Player_2Gun_Skill_Normal_Air.h"
#include "GameInstance.h"
#include "Channel.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"


CState_Player_2Gun_Skill_Normal_Air::CState_Player_2Gun_Skill_Normal_Air(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_2Gun_Skill_Normal_Air::CState_Player_2Gun_Skill_Normal_Air(const CState_Player_2Gun_Skill_Normal_Air& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_2Gun_Skill_Normal_Air::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_JUMP_FALL);

	return S_OK;
}

HRESULT CState_Player_2Gun_Skill_Normal_Air::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_2Gun_Skill_Normal_Air::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;

	m_iCurKeyFrame = 0;

	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(true);

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
}

void CState_Player_2Gun_Skill_Normal_Air::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (2 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("2Gun_Skill_Normal.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect0,
			EFFECT_POS("Player_Gun_Skill_Turn",
				m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(0.f, 0.f, 0.f, 1.f),
				XMVectorSet(0.f, 0.f, 0.f, 0.f),
				0.9f
			)
		);
		IMPL_ONCE(m_bShaked0, pGameInstance->Add_Camera_Shaking(XMVectorSet(0.f, -1.f, 0.f, 0.f), 0.02f, 1.f, 0.5f));
	}
	if (30 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect1,
			EFFECT_POS("Player_Gun_Converge_Black",
				m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(0.f, 0.7f, 0.7f, 1.f),
				XMVectorSet(0.f, 0.f, 0.f, 0.f),
				2.6f
			);
		EFFECT_POS("Player_Gun_Converge_P",
			m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 0.7f, 0.7f, 1.f),
			XMVectorSet(0.f, 0.f, 0.f, 0.f),
			2.6f
		);
		);
	}
	if (45 <= Get_CurKeyFrame())
	{
		if (!m_bCreateBullet)	//ºÒ¸´»ý¼º
		{
			_matrix OffsetMatrix = XMMatrixIdentity();
			OffsetMatrix.r[3] = XMVectorSet(0.f, 0.7f, 5.f, 1.f);
			_matrix TargetMatrix = OffsetMatrix * XMLoadFloat4x4(m_pState_Handler->Get_Owner()->Get_WorldMatrix());
			_float4	vTargetPos;
			XMStoreFloat4(&vTargetPos, TargetMatrix.r[3]);
			pGameInstance->Add_GameObject(pGameInstance->Get_CurLevelIndex(), ID_UINT(PLAYER, WEAPON_2GUN_BULLET_NORMAL), ID_UINT(PLAYER, WEAPON_2GUN_BULLET_NORMAL), &vTargetPos);
			m_bCreateBullet = true;
		}
		IMPL_ONCE(m_bShaked1,
			pGameInstance->Add_Camera_Shaking(XMVectorSet(1.f, -1.f, 0.f, 0.f), 0.045f, 0.5f, 0.25f);
			pGameInstance->Add_DirCA(_float2(-1.f, 1.f), _float4(0.005f, 0.f, 0.0025f, 0.f), 0.5f, 0.5f);
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


	/*_uint iCurKeyFrame = m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame();
	if (m_iCurKeyFrame != iCurKeyFrame)
	{
		m_iCurKeyFrame = iCurKeyFrame;
		if (6 == m_iCurKeyFrame)
		{
			_matrix matPosition = m_pTargetModel->Find_HierarcyNode("Root_Bone")->Get_CombinedMatrix() * XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix());

			matPosition.r[0] = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix()).r[0];
			matPosition.r[1] = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix()).r[1];
			matPosition.r[2] = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix()).r[2];
			EFFECT_EXCUTE("Attack_A_M", matPosition);
		}
	}*/


	CState_Player::Tick(fTimeDelta);
}

void CState_Player_2Gun_Skill_Normal_Air::Exit(void* pArg)
{
	m_bCreateBullet = false;

	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(false);

	__super::Exit();
}

_bool CState_Player_2Gun_Skill_Normal_Air::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (KEY_DOWN('1'))
	{
		return true;
	}

	return false;
}


CState_Player_2Gun_Skill_Normal_Air* CState_Player_2Gun_Skill_Normal_Air::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_2Gun_Skill_Normal_Air* pInstance = new CState_Player_2Gun_Skill_Normal_Air(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_2Gun_Skill_Normal_Air");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_2Gun_Skill_Normal_Air::Clone(void* pArg)
{
	CState_Player_2Gun_Skill_Normal_Air* pInstance = new CState_Player_2Gun_Skill_Normal_Air(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_2Gun_Skill_Normal_Air");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_2Gun_Skill_Normal_Air::Free()
{
}
