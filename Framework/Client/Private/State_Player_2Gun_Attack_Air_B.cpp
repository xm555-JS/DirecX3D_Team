#include "stdafx.h"
#include "..\Public\State_Player_2Gun_Attack_Air_B.h"
#include "GameInstance.h"
#include "Channel.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "Navigation.h"
#include "Unit.h"


CState_Player_2Gun_Attack_Air_B::CState_Player_2Gun_Attack_Air_B(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_2Gun_Attack_Air_B::CState_Player_2Gun_Attack_Air_B(const CState_Player_2Gun_Attack_Air_B& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_2Gun_Attack_Air_B::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_SKILL_NORMAL_AIR);
	m_ReserveStates.push_back(STATE_PLAYER_SKILL_SPECIAL_AIR);
	m_ReserveStates.push_back(STATE_PLAYER_2GUN_ATTACK_JUMP_CHARGE_A);
	m_ReserveStates.push_back(STATE_PLAYER_2GUN_ATTACK_AIR_B);
	m_ReserveStates.push_back(STATE_PLAYER_2GUN_ATTACK_AIR_C);

	return S_OK;
}

HRESULT CState_Player_2Gun_Attack_Air_B::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_2Gun_Attack_Air_B::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.2f;

	m_iCurKeyFrame = 0;

	for (auto& elem : m_bEffect)
		elem = true;

	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(true);

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));


	Use_StaminaGauge(0.1f);
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Player_2Gun_Attack_Air_B::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	_matrix ColMatrix = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix());
	ColMatrix.r[3] -= XMVector3Normalize(ColMatrix.r[2]) + XMVector3Normalize(ColMatrix.r[1]);
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 15.f);
	m_pColCom->Update(ColMatrix);

	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(30, 60), 0, 2, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[0], "Player_Gun_Hit");
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(30, 60), 3, 5, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[1], "Player_Gun_Hit");
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(30, 60), 6, 8, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[2], "Player_Gun_Hit");
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(30, 60), 9, 11, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[3], "Player_Gun_Hit");
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(30, 60), 12, 14, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[4], "Player_Gun_Hit");
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(30, 60), 15, 17, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[5], "Player_Gun_Hit");
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(30, 60), 18, 20, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[6], "Player_Gun_Hit");
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(30, 60), 21, 23, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[7], "Player_Gun_Hit");
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(30, 60), 24, 26, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[8], "Player_Gun_Hit");
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(30, 60), 27, 29, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[9], "Player_Gun_Hit");
	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(30, 60), 30, 33, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[10], "Player_Gun_Hit");

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
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

	Set_PlayerLook(fTimeDelta);

	if (KEY_PRESSING('W') ||
		KEY_PRESSING('A') ||
		KEY_PRESSING('S') ||
		KEY_PRESSING('D'))
	{
		_float	fPlayerSpeed = ((CUnit*)m_pState_Handler->Get_Player())->Get_UnitInfo()->fSpeed;
		_float	fRunSpeed = fPlayerSpeed * 8.f;
		CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
		CTransform* pPlayerTransform = m_pState_Handler->Get_Player()->Get_Transform();

		pPlayerTransform->Go_Front(fTimeDelta, fRunSpeed, pPlayerNavi);
	}

	if (KEY_PRESSING(VK_LBUTTON))
		m_fKeyPressTimeAcc += fTimeDelta;
	else
		m_fKeyPressTimeAcc = 0.f;

	for(size_t i = 0; i < 10; ++i)
	{
		if (i * 4 < Get_CurKeyFrame())
		{
			if (0 == i % 2)
			{
				IMPL_ONCE(m_bSound[i], MGR(CSound_Manager)->PlaySurroundSound(TEXT("2Gun_Attack_A_1.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
			}
			else
			{
				IMPL_ONCE(m_bSound[i], MGR(CSound_Manager)->PlaySurroundSound(TEXT("2Gun_Attack_A_2.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
			}
		}
	}

	for (size_t i = 0; i < 20; ++i)
	{
		_matrix RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRandom(-3.14f, 3.14f)) * XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), fRandom(-1.f, 1.f)) * XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), fRandom(-1.f, 1.f));
		if (i * 2 + 2 < Get_CurKeyFrame())
		{
			IMPL_ONCE(m_bEffect[i],
				EFFECT_POS_ROT("Player_Gun_Attack_A_5",
					m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
					XMVectorSet(fRandom(-0.5f, 0.5f), 3.f, fRandom(-0.5f, 0.5f), 1.f),
					RotationMatrix,
					1.f,
					XMVectorSet(fRandom(1.f, 2.f), fRandom(1.5f, 2.5f), fRandom(1.f, 2.f), 0.f),
					XMVectorSet(fRandom(3.14f - 1.25f, 3.14f + 1.25f), fRandom(-3.15f, 3.15f), fRandom(3.14f - 1.25f, 3.14f + 1.25f), 0.f));
			);
		}
		if (i * 2 + 1 < Get_CurKeyFrame())
		{
			IMPL_ONCE(m_bEffect[i],
				EFFECT_POS_ROT("Player_Gun_Attack_B_2",
					m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
					XMVectorSet(fRandom(-0.5f, 0.5f), 3.f, fRandom(-0.5f, 0.5f), 1.f),
					RotationMatrix,
					1.f,
					XMVectorSet(fRandom(1.f, 2.f), fRandom(1.5f, 2.5f), fRandom(1.f, 2.f), 0.f),
					XMVectorSet(fRandom(3.14f - 1.25f, 3.14f + 1.25f), fRandom(-3.15f, 3.15f), fRandom(3.14f - 1.25f, 3.14f + 1.25f), 0.f));
			);
		}
	}

	/*if (m_pState_Handler->Get_KeyInput() &&
		m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}*/



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

	Set_PlayerLook(fTimeDelta);
	CState_Player::Tick(fTimeDelta);
}

void CState_Player_2Gun_Attack_Air_B::Exit(void* pArg)
{
	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(false);
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));

	for (auto& elem : m_bEffect)
		elem = false;

	for (_uint i = 0; i < 11; ++i)
		m_bHit[i] = false;

	__super::Exit();
}

_bool CState_Player_2Gun_Attack_Air_B::Check_Condition(CState_Handler* pHandler)
{
	/*for (auto& iKey : *pHandler->Get_KeyInputList())
	{
		if (VK_LBUTTON == iKey)
			return true;
	}
	return false;*/
	if (STATE_PLAYER_2GUN_ATTACK_AIR_A == pHandler->Get_CurStateType())
	{
		return true;
	}
	else if (STATE_PLAYER_2GUN_ATTACK_AIR_B == pHandler->Get_CurStateType())
	{
		for (auto& iKey : *pHandler->Get_KeyInputList())
		{
			if (VK_LBUTTON == iKey && Enable_Use_Stamina(pHandler))
				return true;
		}
	}

	return false;
}


CState_Player_2Gun_Attack_Air_B* CState_Player_2Gun_Attack_Air_B::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_2Gun_Attack_Air_B* pInstance = new CState_Player_2Gun_Attack_Air_B(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_2Gun_Attack_Air_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_2Gun_Attack_Air_B::Clone(void* pArg)
{
	CState_Player_2Gun_Attack_Air_B* pInstance = new CState_Player_2Gun_Attack_Air_B(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_2Gun_Attack_Air_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_2Gun_Attack_Air_B::Free()
{
}
