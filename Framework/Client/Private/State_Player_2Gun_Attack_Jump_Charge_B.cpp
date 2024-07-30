#include "stdafx.h"
#include "..\Public\State_Player_2Gun_Attack_Jump_Charge_B.h"
#include "GameInstance.h"
#include "Channel.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"


CState_Player_2Gun_Attack_Jump_Charge_B::CState_Player_2Gun_Attack_Jump_Charge_B(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_2Gun_Attack_Jump_Charge_B::CState_Player_2Gun_Attack_Jump_Charge_B(const CState_Player_2Gun_Attack_Jump_Charge_B& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_2Gun_Attack_Jump_Charge_B::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_2GUN_ATTACK_JUMP_CHARGE_C);
	//m_ReserveStates.push_back(STATE_PLAYER_WALK);
	//m_ReserveStates.push_back(STATE_PLAYER_IDLE);

	return S_OK;
}

HRESULT CState_Player_2Gun_Attack_Jump_Charge_B::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_2Gun_Attack_Jump_Charge_B::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.5f;

	m_iCurKeyFrame = 0;

	m_bEndCurState = true;

	m_fPlayerPosY = XMVectorGetY(m_pState_Handler->Get_Owner()->Get_Transform()->Get_WorldMatrix().r[3]);
	//m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	IMPL_ONCE(m_bSound[0],m_eSoundChannel[2] = MGR(CSound_Manager)->PlaySurroundSound(TEXT("2Gun_Attack_Jump_Charge_B.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));

	EFFECT_POS("Player_Gun_Attack_Air_B", m_pState_Handler->Get_Owner()->Get_WorldMatrix());
	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Player_2Gun_Attack_Jump_Charge_B::Tick(_float fTimeDelta)
{
	if (1.f < m_fPlayerPosY - XMVectorGetY(m_pState_Handler->Get_Owner()->Get_Transform()->Get_WorldMatrix().r[3]))
	{
		EFFECT_POS("Player_Gun_Attack_Air_B", m_pState_Handler->Get_Owner()->Get_WorldMatrix());
		m_fPlayerPosY = XMVectorGetY(m_pState_Handler->Get_Owner()->Get_Transform()->Get_WorldMatrix().r[3]);
	}
	USEGAMEINSTANCE;

	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, fRandom(230.f,270.f), 35, 40, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit);

	_matrix ColMatrix = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix());
	ColMatrix.r[3] -= XMVector3Normalize(ColMatrix.r[2]);
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 4.f);
	m_pColCom->Update(ColMatrix);
	/*if(nullptr != m_pColCom)
		m_pColCom->Set_ColliderState(true);*/

	/*if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}*/

	/*if (KEY_PRESSING('W') ||
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


	if (m_pState_Handler->Get_KeyInput() &&
		25 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_bEndCurState = true;
	}

	if (KEY_PRESSING(VK_LBUTTON))
		m_fKeyPressTimeAcc += fTimeDelta;*/


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

void CState_Player_2Gun_Attack_Jump_Charge_B::Exit(void* pArg)
{
	MGR(CSound_Manager)->StopSound(m_eSoundChannel[2]);
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	//CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	//pPlayerTransform->Set_YFixState(false);

	m_bHit = false;

	__super::Exit();
}

_bool CState_Player_2Gun_Attack_Jump_Charge_B::Check_Condition(CState_Handler* pHandler)
{

	return true;
}


CState_Player_2Gun_Attack_Jump_Charge_B* CState_Player_2Gun_Attack_Jump_Charge_B::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_2Gun_Attack_Jump_Charge_B* pInstance = new CState_Player_2Gun_Attack_Jump_Charge_B(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_2Gun_Attack_Jump_Charge_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_2Gun_Attack_Jump_Charge_B::Clone(void* pArg)
{
	CState_Player_2Gun_Attack_Jump_Charge_B* pInstance = new CState_Player_2Gun_Attack_Jump_Charge_B(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_2Gun_Attack_Jump_Charge_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_2Gun_Attack_Jump_Charge_B::Free()
{
}
