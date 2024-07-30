#include "stdafx.h"
#include "..\Public\State_Player_Run.h"
#include "GameInstance.h"
#include "Unit.h"
#include "Player.h"
#include "Weapon.h"

#include "Imgui_Manager.h"

CState_Player_Run::CState_Player_Run(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Run::CState_Player_Run(const CState_Player_Run& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Run::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_SKILL_NORMAL);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_SPECIAL);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK_BASE_CHARGE);
	m_ForceStates.push_back(STATE_PLAYER_ATTACK);
	m_ForceStates.push_back(STATE_PLAYER_DAMAGED_LOW);
	m_ForceStates.push_back(STATE_PLAYER_DAMAGED_MEDIUM);
	m_ForceStates.push_back(STATE_PLAYER_DAMAGED_HIGH_BEGIN);
	m_ForceStates.push_back(STATE_PLAYER_MOUNT_UP);
	m_ForceStates.push_back(STATE_PLAYER_GLIDE_UP);
	m_ForceStates.push_back(STATE_PLAYER_SKATE_CALL);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_JUMP_A);
	m_ReserveStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ReserveStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ReserveStates.push_back(STATE_PLAYER_HAND_IDLE);

	return S_OK;
}

HRESULT CState_Player_Run::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Run::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_pTargetModel->Set_AnimationGroup(ANIM_PLAYER_FEMALE, WEAPON_SWORD);
	m_fAnimSpeed = 2.f;
	m_bEndCurState = true;
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

void CState_Player_Run::Tick(_float fTimeDelta)
{
	Set_PlayerLook(fTimeDelta);

	m_fTimeAcc_Effect += fTimeDelta;

	if (0.05f < m_fTimeAcc_Effect)
	{
		m_fTimeAcc_Effect = 0;
		if (m_bToggle_Effect)
			EFFECT_POS("Run_P", m_pState_Handler->Get_Player()->Get_WorldMatrix());
		else
			EFFECT_POS("Run_P_90", m_pState_Handler->Get_Player()->Get_WorldMatrix(),
				XMVectorSet(0.f, 0.f, 0.f, 1.f),
				XMVectorSet(0.f, 0.f, XMConvertToRadians(-90.f), 0.f));
		m_bToggle_Effect = !m_bToggle_Effect;
	}

	USEGAMEINSTANCE;
	if (KEY_PRESSING('W') ||
		KEY_PRESSING('A') ||
		KEY_PRESSING('S') ||
		KEY_PRESSING('D'))
	{
		_float	fPlayerSpeed = ((CUnit*)m_pState_Handler->Get_Player())->Get_UnitInfo()->fSpeed;
		_float	fRunSpeed = fPlayerSpeed * 8.f;
		CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
		//if (KEY_PRESSING('W'))
		//	m_pState_Handler->Get_Player()->Get_Transform()->Go_Front(fTimeDelta, fRunSpeed, pPlayerNavi);

		//if (KEY_PRESSING('A'))
		//	m_pState_Handler->Get_Player()->Get_Transform()->Go_Left(fTimeDelta, fRunSpeed, pPlayerNavi);

		//if (KEY_PRESSING('S'))
		//	m_pState_Handler->Get_Player()->Get_Transform()->Go_Back(fTimeDelta, fRunSpeed, pPlayerNavi);

		//if (KEY_PRESSING('D'))
		//	m_pState_Handler->Get_Player()->Get_Transform()->Go_Right(fTimeDelta, fRunSpeed, pPlayerNavi);

		m_pState_Handler->Get_Player()->Get_Transform()->Go_Front(fTimeDelta, fRunSpeed, pPlayerNavi);
	}

	if (KEY_PRESSING(VK_LBUTTON))
		m_fKeyPressTimeAcc += fTimeDelta;
	else
		m_fKeyPressTimeAcc = 0.f;

	//if (13 <= Get_CurKeyFrame())
	//{
	//	IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySound(TEXT("Player_Walk.mp3"), CSound_Manager::CHANNEL_PLAYER, 1.f));
	//}
	//if (27 <= Get_CurKeyFrame())
	//{
	//	IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySound(TEXT("Player_Walk.mp3"), CSound_Manager::CHANNEL_PLAYER, 1.f));
	//}

	IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_Walk.mp3"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.8f));

	IMPL_ALARM(m_fSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_Walk.mp3"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.8f), 0.25f, fTimeDelta);

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Run::Exit(void* pArg)
{
	m_fKeyPressTimeAcc = 0.f;
	__super::Exit();
}

_bool CState_Player_Run::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (KEY_PRESSING('W') ||
		KEY_PRESSING('A') ||
		KEY_PRESSING('S') ||
		KEY_PRESSING('D'))
	{
		if (STATE_PLAYER_WALK == pHandler->Get_CurStateType() &&
			0.5f > pHandler->Get_TimeAcc())
			return false;

		_float4 vPos;
		XMStoreFloat4(&vPos, pHandler->Get_Player()->Get_Pos());
		if (vPos.y < static_cast<CPlayer*>(pHandler->Get_Player())->Get_SwimHeight() + 0.1f)
			return false;

		return true;
	}

	//if (STATE_PLAYER_ROLL_FORWORD == pHandler->Get_CurStateType())
	//{
	//	return true;
	//}
	return false;
}


CState_Player_Run* CState_Player_Run::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Run* pInstance = new CState_Player_Run(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Run::Clone(void* pArg)
{
	CState_Player_Run* pInstance = new CState_Player_Run(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Run::Free()
{
}

