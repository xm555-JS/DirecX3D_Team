#include "stdafx.h"
#include "..\Public\State_Player_Damaged_High_Begin.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Player.h"
#include "Weapon.h"

#include "Effect_Manager.h"

///collider
#include "Collider.h"
#include "Imgui_Manager.h"


CState_Player_Damaged_High_Begin::CState_Player_Damaged_High_Begin(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Damaged_High_Begin::CState_Player_Damaged_High_Begin(const CState_Player_Damaged_High_Begin& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Damaged_High_Begin::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_PLAYER_DAMAGED_HIGH_END);

	return S_OK;
}

HRESULT CState_Player_Damaged_High_Begin::Initialize(void* pArg)
{
	USEGAMEINSTANCE;

	return S_OK;
}

void CState_Player_Damaged_High_Begin::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_bEndCurState = false;
	m_fAnimSpeed = 2.f;
	IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_Damaged_Hight.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

	USEGAMEINSTANCE;
	CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
	m_pState_Handler->Get_Player()->Get_Transform()->Jump(5.5f, pPlayerNavi);
	m_pPlayer->Get_Transform()->Set_Skate(false);
	((CPlayer*)m_pState_Handler->Get_Player())->Set_Target(nullptr);

	m_pPlayer->Get_Mobility_Glider()->Set_IsDissolve(true);
	m_pPlayer->Get_Mobility_Glider()->Set_IsUsing(false);
	m_pPlayer->Get_Mobility_Glider()->Set_IsGlow(true);

	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsDissolve(true);
	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsUsing(false);
	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsGlow(true);
}

void CState_Player_Damaged_High_Begin::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	IMPL_ONCE(m_bShaked0,
		pGameInstance->Add_Camera_Shaking(XMVectorSet(1.f, 2.f, 0.f, 0.f), 0.033f, 0.3f, 0.25f);
		pGameInstance->Add_DirCA(_float2(-2.f, -1.f), {0.023f,0.0f,0.0115f,0.f}, 0.6f, 0.6f););

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	_float	fPlayerSpeed = ((CUnit*)m_pState_Handler->Get_Player())->Get_UnitInfo()->fSpeed;
	_float	fRunSpeed = fPlayerSpeed * 4.f;
	CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
	CTransform* pPlayerTransform = m_pState_Handler->Get_Player()->Get_Transform();

	pPlayerTransform->Go_Back(fTimeDelta, fRunSpeed, pPlayerNavi);

	IMPL_ONCE(m_bEffect0, EFFECT_POS("Player_Damaged", m_pState_Handler->Get_Owner()->Get_WorldMatrix()));

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Damaged_High_Begin::Exit(void* pArg)
{
	__super::Exit();
}

_bool CState_Player_Damaged_High_Begin::Check_Condition(CState_Handler* pHandler)
{
	return	((CPlayer*)pHandler->Get_Player())->Get_Damaged(2);
}


CState_Player_Damaged_High_Begin* CState_Player_Damaged_High_Begin::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Damaged_High_Begin* pInstance = new CState_Player_Damaged_High_Begin(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Damaged_High_Begin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Damaged_High_Begin::Clone(void* pArg)
{
	CState_Player_Damaged_High_Begin* pInstance = new CState_Player_Damaged_High_Begin(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Damaged_High_Begin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Damaged_High_Begin::Free()
{
}
