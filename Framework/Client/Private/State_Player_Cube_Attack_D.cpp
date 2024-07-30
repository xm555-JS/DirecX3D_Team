#include "stdafx.h"
#include "..\Public\State_Player_Cube_Attack_D.h"
#include "GameInstance.h"
#include "Channel.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"
#include "Unit.h"
#include "Player.h"

#include "Imgui_Manager.h"

CState_Player_Cube_Attack_D::CState_Player_Cube_Attack_D(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player_Cube(pManager, eState)
{
}

CState_Player_Cube_Attack_D::CState_Player_Cube_Attack_D(const CState_Player_Cube_Attack_D& rhs)
	: CState_Player_Cube(rhs)
{
}

HRESULT CState_Player_Cube_Attack_D::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_CUBE_ATTACK_SKILL_NORMAL);
	m_ForceStates.push_back(STATE_PLAYER_JUMP_A);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_CUBE_ATTACK_BASE_CHARGE);
	m_ReserveStates.push_back(STATE_PLAYER_CUBE_ATTACK_E);
	m_ReserveStates.push_back(STATE_PLAYER_WALK);
	m_ReserveStates.push_back(STATE_PLAYER_IDLE);

	return S_OK;
}

HRESULT CState_Player_Cube_Attack_D::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Cube_Attack_D::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;
	m_iSoundChannel = 0;

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Player_Cube_Attack_D::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	_matrix ColMatrix;

	if (11 < Get_CurKeyFrame() && 20 > Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect0, EFFECT_POS(Random(0, 1) == 1 ? "Player_Cube_Attack1_Up-1" : "Player_Cube_Attack2_Up-1", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 0.f, 3.5f, 1.f)));
		IMPL_ONCE(m_bEffect1, EFFECT_POS("Player_Cube_Ground", m_pState_Handler->Get_Owner()->Get_WorldMatrix()));

		m_fTimeAcc += fTimeDelta;
		if (0.03f < m_fTimeAcc)
		{
			IMPL_ONCE(m_bSound[m_iSoundChannel], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Cube_Attack_D.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 0.5f));

			EFFECT_POS("Player_Cube_Attack1_Up_Middle-1", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(fRandom(-3.f, 3.f), 0.f, fRandom(2.5f, 6.f), 1.f)); //, XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f, XMVectorSet(0.6f, 0.6f, 0.6f, 0.f));
			EFFECT_POS("Player_Cube_Attack2_Up_Middle-1", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(fRandom(-3.f, 3.f), 0.f, fRandom(2.5f, 6.f), 1.f)); //, XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f, XMVectorSet(0.6f, 0.6f, 0.6f, 0.f));
			m_fTimeAcc = 0.f;
			++m_iSoundChannel;
		}

	}


	if (nullptr != ((CUnit*)m_pState_Handler->Get_Player())->Get_Target())
		ColMatrix = XMLoadFloat4x4(((CUnit*)m_pState_Handler->Get_Player())->Get_Target()->Get_WorldMatrix());
	else
	{
		ColMatrix = XMLoadFloat4x4(((CUnit*)m_pState_Handler->Get_Player())->Get_WorldMatrix());
		ColMatrix.r[3] += XMVector3Normalize(ColMatrix.r[2]) * 2.f;
	}
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 5.f);
	m_pColCom->Update(ColMatrix);

	Hit_Unit(m_pColCom, ELEMENTAL_ICE, fRandom(200, 220), 12, 20, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0, "Player_Hit_Cube");
	
	if (m_bHit0)
	{
		IMPL_ONCE(m_bSound[10], MGR(CSound_Manager)->PlaySurroundSound(TEXT("CubeHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bShaked0, pGameInstance->Add_Camera_Shaking(XMVectorSet(1.f, 1.f, 0.f, 0.f), 0.035f, 0.2f, 0.2f));
	}

	
	if (m_bHit0 && !m_bHitCnt)
	{
		IMPL_ONCE(m_bGauge0, m_pPlayer->Set_SkillGauge(fRandom(200, 220)));
		CreateCubeBullet();
		m_bHitCnt = true;
	}

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	if (KEY_PRESSING('W') ||
		KEY_PRESSING('S') ||
		KEY_PRESSING('A') ||
		KEY_PRESSING('D'))
	{
		m_pState_Handler->Set_KeyInput(true);
	}

	if (KEY_DOWN(VK_LBUTTON))
	{
		m_pState_Handler->Set_KeyInput(true);
		m_pState_Handler->Add_KeyInput(VK_LBUTTON);
	}

	if (KEY_DOWN(VK_SPACE))
	{
		m_pState_Handler->Set_KeyInput(true);
		m_pState_Handler->Add_KeyInput(VK_SPACE);
	}


	if ((m_pState_Handler->Get_KeyInput() || 0.15f < m_fKeyPressTimeAcc) &&
		20 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_bEndCurState = true;
	}

	if (KEY_PRESSING(VK_LBUTTON))
		m_fKeyPressTimeAcc += fTimeDelta;
	else
		m_fKeyPressTimeAcc = 0.f;
		
	Set_PlayerLook(fTimeDelta);
	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Cube_Attack_D::Exit(void* pArg)
{
	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	m_bHitCnt = false;
	__super::Exit();
}

_bool CState_Player_Cube_Attack_D::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	for (auto& iKey : *pHandler->Get_KeyInputList())
	{
		if (VK_LBUTTON == iKey)
			return true;
	}

	return false;
}


CState_Player_Cube_Attack_D* CState_Player_Cube_Attack_D::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Cube_Attack_D* pInstance = new CState_Player_Cube_Attack_D(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Cube_Attack_D");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Cube_Attack_D::Clone(void* pArg)
{
	CState_Player_Cube_Attack_D* pInstance = new CState_Player_Cube_Attack_D(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Cube_Attack_D");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Cube_Attack_D::Free()
{
}
