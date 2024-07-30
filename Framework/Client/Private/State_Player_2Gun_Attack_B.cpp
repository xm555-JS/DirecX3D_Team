#include "stdafx.h"
#include "..\Public\State_Player_2Gun_Attack_B.h"
#include "GameInstance.h"
#include "Channel.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"
#include "Player.h"


CState_Player_2Gun_Attack_B::CState_Player_2Gun_Attack_B(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_2Gun_Attack_B::CState_Player_2Gun_Attack_B(const CState_Player_2Gun_Attack_B& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_2Gun_Attack_B::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_JUMP_A);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_SKILL_NORMAL);
	m_ReserveStates.push_back(STATE_PLAYER_SKILL_SPECIAL);
	m_ReserveStates.push_back(STATE_PLAYER_2GUN_ATTACK_BASE_CHARGE_BEGIN);
	m_ReserveStates.push_back(STATE_PLAYER_2GUN_ATTACK_C);
	m_ReserveStates.push_back(STATE_PLAYER_WALK);
	m_ReserveStates.push_back(STATE_PLAYER_IDLE);

	return S_OK;
}

HRESULT CState_Player_2Gun_Attack_B::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_2Gun_Attack_B::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;

	m_iCurKeyFrame = 0;

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());

	
}

void CState_Player_2Gun_Attack_B::Tick(_float fTimeDelta)
{
	

	USEGAMEINSTANCE;

	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(140.f, 170.f), 15, 40, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0, "Player_Gun_Hit");

	if (m_bHit0)
	{
		IMPL_ONCE(m_bSound[11], MGR(CSound_Manager)->PlaySurroundSound(TEXT("2GunHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge0, m_pPlayer->Set_SkillGauge(fRandom(140.f, 170.f)));
		IMPL_ONCE(m_bShaked0, pGameInstance->Add_Camera_Shaking(XMVectorSet(1.f, 1.f, 0.f, 0.f), 0.025f, 0.15f, 0.15f));
	}

	_matrix ColMatrix = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix());
	ColMatrix.r[3] += XMVector3Normalize(ColMatrix.r[2]) * 2.f;
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 6.f);
	m_pColCom->Update(ColMatrix);

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

	//35
	if ((m_pState_Handler->Get_KeyInput() || 0.15f < m_fKeyPressTimeAcc) &&
		25 < Get_CurKeyFrame())
	{
		m_bEndCurState = true;
	}

	if (KEY_PRESSING(VK_LBUTTON))
		m_fKeyPressTimeAcc += fTimeDelta;
	else
		m_fKeyPressTimeAcc = 0.f;

	if (16 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("2Gun_Attack_B.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(),0.5f));

		IMPL_ONCE(m_bEffect0,
			//Create_EFFECT_POS("Player_Gun_Attack", DEBUGVECTOR(0), DEBUGVECTOR(1), DEBUGVECTOR(2));
			for (size_t i = 0; i < 10; ++i)
			{
				_matrix RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRandom(-3.14f, 3.14f)) * XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), 1.571f);
				EFFECT_POS_ROT("Player_Gun_Attack_B_2", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 1.2f, 1.1f, 1.f), RotationMatrix, 1.f, XMVectorSet(1.5f, 1.5f, 1.5f, 0.f), XMVectorSet(fRandom(-0.3f, 0.3f), fRandom(-1.3f, 1.3f), 0.f, 0.f));
			}

			for (size_t i = 0; i < 7; ++i)
				EFFECT_POS("Player_Gun_Attack_C", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
					XMVectorSet(0.f, 1.2f, 1.1f, 1.f), XMVectorSet(1.571f, 0.f, 0.f, 0.f), 1.f, XMVectorSet(1.f, 1.f, 1.f, 0.f), XMVectorSet(fRandom(-0.3f, 0.3f), fRandom(-1.3f, 1.3f), 0.f, 0.f));
			);
	}
	

	//_uint iCurKeyFrame = m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame();
	//if (m_iCurKeyFrame != iCurKeyFrame)
	//{
	//	m_iCurKeyFrame = iCurKeyFrame;
	//	if (6 == m_iCurKeyFrame)
	//	{
	//		_matrix matPosition = m_pTargetModel->Find_HierarcyNode("Root_Bone")->Get_CombinedMatrix() * XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix());

	//		matPosition.r[0] = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix()).r[0];
	//		matPosition.r[1] = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix()).r[1];
	//		matPosition.r[2] = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix()).r[2];
	//		EFFECT_EXCUTE("Attack_A_M", matPosition);
	//	}
	//}

	Set_PlayerLook(fTimeDelta);
	CState_Player::Tick(fTimeDelta);
}

void CState_Player_2Gun_Attack_B::Exit(void* pArg)
{

	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	__super::Exit();
}

_bool CState_Player_2Gun_Attack_B::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	for (auto& iKey : *pHandler->Get_KeyInputList())
	{
		if (VK_LBUTTON == iKey)
			return true;
	}

	return false;
}


CState_Player_2Gun_Attack_B* CState_Player_2Gun_Attack_B::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_2Gun_Attack_B* pInstance = new CState_Player_2Gun_Attack_B(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_2Gun_Attack_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_2Gun_Attack_B::Clone(void* pArg)
{
	CState_Player_2Gun_Attack_B* pInstance = new CState_Player_2Gun_Attack_B(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_2Gun_Attack_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_2Gun_Attack_B::Free()
{
}
