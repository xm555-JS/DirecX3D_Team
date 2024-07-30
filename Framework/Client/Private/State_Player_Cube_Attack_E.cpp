#include "stdafx.h"
#include "..\Public\State_Player_Cube_Attack_E.h"
#include "GameInstance.h"
#include "Channel.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"
#include "Unit.h"
#include "Player.h"

#include "Imgui_Manager.h"

CState_Player_Cube_Attack_E::CState_Player_Cube_Attack_E(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player_Cube(pManager, eState)
{
}

CState_Player_Cube_Attack_E::CState_Player_Cube_Attack_E(const CState_Player_Cube_Attack_E& rhs)
	: CState_Player_Cube(rhs)
{
}

HRESULT CState_Player_Cube_Attack_E::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_CUBE_ATTACK_SKILL_NORMAL);
	m_ForceStates.push_back(STATE_PLAYER_JUMP_A);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_CUBE_ATTACK_BASE_CHARGE);
	m_ReserveStates.push_back(STATE_PLAYER_WALK);
	m_ReserveStates.push_back(STATE_PLAYER_IDLE);

	return S_OK;
}

HRESULT CState_Player_Cube_Attack_E::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Cube_Attack_E::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;
	m_fSoundTime = 0.f;
	m_iSoundChannel = 0;

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Player_Cube_Attack_E::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	_matrix ColMatrix;


	if (38 < Get_CurKeyFrame())
	{
		m_fSoundTime += fTimeDelta;			
		IMPL_ONCE(m_bSound[m_iSoundChannel], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Cube_Attack_E.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(),0.5f));

		if (0.1f < m_fSoundTime
			&& 2 > m_iSoundChannel)
		{
			m_fSoundTime = 0.f;
			++m_iSoundChannel;
			IMPL_ONCE(m_bSound[m_iSoundChannel], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Cube_Attack_E.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 1.f));
		}

		IMPL_ONCE(m_bEffect0, EFFECT_POS("Player_Cube_Attack4-1", m_pState_Handler->Get_Owner()->Get_WorldMatrix()));
		IMPL_ONCE(m_bEffect1, EFFECT_POS("Player_Cube_Ground", m_pState_Handler->Get_Owner()->Get_WorldMatrix()));
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

	Hit_Unit(m_pColCom, ELEMENTAL_ICE, fRandom(200, 220), 38, 70, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0, "Player_Hit_Cube");
	
	if (m_bHit0)
	{
		IMPL_ONCE(m_bSound[10], MGR(CSound_Manager)->PlaySurroundSound(TEXT("CubeHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bShaked0, pGameInstance->Add_Camera_Shaking(XMVectorSet(-1.f, 0.f, 0.f, 0.f), 0.043f, 0.2f, 0.2f));
	}
	
	if (m_bHit0 && !m_bHitCnt)
	{
		IMPL_ONCE(m_bSound[11], MGR(CSound_Manager)->PlaySurroundSound(TEXT("CubeHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
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
		90 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
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

void CState_Player_Cube_Attack_E::Exit(void* pArg)
{
	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	m_bHitCnt = false;
	__super::Exit();
}

_bool CState_Player_Cube_Attack_E::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	for (auto& iKey : *pHandler->Get_KeyInputList())
	{
		if (VK_LBUTTON == iKey)
			return true;
	}

	return false;
}


CState_Player_Cube_Attack_E* CState_Player_Cube_Attack_E::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Cube_Attack_E* pInstance = new CState_Player_Cube_Attack_E(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Cube_Attack_E");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Cube_Attack_E::Clone(void* pArg)
{
	CState_Player_Cube_Attack_E* pInstance = new CState_Player_Cube_Attack_E(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Cube_Attack_E");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Cube_Attack_E::Free()
{
}
