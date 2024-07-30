#include "stdafx.h"
#include "..\Public\State_Player_Eat.h"
#include "GameInstance.h"
#include "Player.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Item_Manager.h"
#include "Weapon.h"

#include "Effect_Manager.h"

///collider
#include "Collider.h"

//#include "Imgui_Manager.h"

CState_Player_Eat::CState_Player_Eat(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Eat::CState_Player_Eat(const CState_Player_Eat& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Eat::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_ATTACK_BASE_CHARGE);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_NORMAL);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK);
	m_ForceStates.push_back(STATE_PLAYER_JUMP_A);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ForceStates.push_back(STATE_PLAYER_WALK);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_HAND_IDLE);

	return S_OK;
}

HRESULT CState_Player_Eat::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Eat::Enter(void* pArg)
{
	MGR(CSound_Manager)->PlaySound(TEXT("Player_Eat.mp3"), CSound_Manager::CHANNEL_PLAYER, 1.f);

	__super::Enter(pArg);
	//m_pTargetModel->Set_AnimationGroup(ANIM_PLAYER_FEMALE, WEAPON_SWORD);
	//m_bEndCurState = true;
	m_fAnimSpeed = 1.5f;
	Load_UI(TEXT("Food_Proceeding"));
	((CPlayer*)m_pState_Handler->Get_Player())->Set_Target(nullptr);

	if (nullptr != ((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponL())
	{
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponL()->Set_IsDissolve(true);
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponL()->Set_IsUsing(false);
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponL()->Set_IsGlow(true);
	}

	if (nullptr != ((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponR())
	{
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponR()->Set_IsDissolve(true);
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponR()->Set_IsUsing(false);
		((CPlayer*)m_pState_Handler->Get_Player())->Get_CurWeaponR()->Set_IsGlow(true);
	}
}

void CState_Player_Eat::Tick(_float fTimeDelta)
{
	m_fTimeAcc_Effect0 += fTimeDelta;
	m_fTimeAcc_Effect1 += fTimeDelta;

	if (50 <= Get_CurKeyFrame() && 80 >= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect0, EFFECT_POS("Heal_M",
			m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, -0.5f, 0.f, 1.f),
			XMVectorSet(0.f, 0.f, 0.f, 0.f),
			0.6f,
			XMVectorSet(1.5f, 1.5f, 1.5f, 0.f)
		)
		);
		if (0.03f < m_fTimeAcc_Effect0)
		{
			m_fTimeAcc_Effect0 = 0.f;
			EFFECT_POS("Heal_P1", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(0.f, -0.4f, 0.f, 1.f),
				XMVectorSet(0.f, 0.f, 0.f, 0.f),
				1.f,
				XMVectorSet(1.5f, 1.5f, 1.5f, 1.f)
			);
		}
	}
	if (80 <= Get_CurKeyFrame() && 90 >= Get_CurKeyFrame())
	{
		if (0.05f < m_fTimeAcc_Effect1)
		{
			m_fTimeAcc_Effect1 = 0.f;
			EFFECT_POS("Heal_P2",
				m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(0.f, 0.2f, 0.f, 1.f),
				XMVectorSet(0.f, 0.f, 0.f, 1.f),
				1.f,
				XMVectorSet(1.5f, 1.5f, 1.5f, 1.f)
			);
		}
	}



	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		((CPlayer*)m_pState_Handler->Get_Player())->Healing();

		m_bEndCurState = true;
	}

	Set_PlayerLook(fTimeDelta);

#if 0
	if (88 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect0, EFFECT_POS("Player_Heal", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 0.7f, 0.f, 1.f),
			XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f,
			XMVectorSet(0.5f, 0.5f, 0.5f, 0.f)));
	}
#endif // 0

	//if (100 < Get_CurKeyFrame())
	//{
	//	// 포스 회전 스케일
	//	IMPL_ONCE(m_bEffect1, EFFECT_POS("Player_Heal_Remain", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
	//		XMVectorSet(DEBUGFLOAT4(0).x, DEBUGFLOAT4(0).y, DEBUGFLOAT4(0).z, 1.f),
	//		XMVectorSet(DEBUGFLOAT4(1).x, DEBUGFLOAT4(1).y, DEBUGFLOAT4(1).z, 0.f), DEBUGFLOAT4(3).x,
	//		XMVectorSet(DEBUGFLOAT4(2).x, DEBUGFLOAT4(2).y, DEBUGFLOAT4(2).z, 0.f)));
	//}

	CState_Player::Tick(fTimeDelta);
		}

void CState_Player_Eat::Exit(void* pArg)
{
	MGR(CSound_Manager)->PlaySound(TEXT("Player_Eat_Hp.mp3"), CSound_Manager::CHANNEL_PLAYER, 1.f);
	__super::Exit();
}

_bool CState_Player_Eat::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

#ifdef _DEBUG
	if (KEY_DOWN(VK_F2))
	{
		return true;
	}
#endif
	
	if (KEY_DOWN(VK_F2) &&
		ITEM_END != CItem_Manager::Get_Instance()->Get_CurItem() &&
		((CPlayer*)pHandler->Get_Player())->Get_EnableKeyInput())
	{
		return true;
	}

	return false;
}


CState_Player_Eat* CState_Player_Eat::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Eat* pInstance = new CState_Player_Eat(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Eat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Eat::Clone(void* pArg)
{
	CState_Player_Eat* pInstance = new CState_Player_Eat(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Eat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Eat::Free()
{
}
