#include "stdafx.h"
#include "..\Public\State_Player_Shield_Roll_Forword.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "HIerarchyNode.h"
#include "Player.h"
#include "Channel.h"
#include "Weapon.h"

#include "Effect_Manager.h"


CState_Player_Shield_Roll_Forword::CState_Player_Shield_Roll_Forword(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Shield_Roll_Forword::CState_Player_Shield_Roll_Forword(const CState_Player_Shield_Roll_Forword& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Shield_Roll_Forword::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_ATTACK_ROLL);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_NORMAL);
	m_ForceStates.push_back(STATE_PLAYER_SKILL_SPECIAL);
	m_ForceStates.push_back(STATE_PLAYER_JUMP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ReserveStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ReserveStates.push_back(STATE_PLAYER_ROLL_FORWORD_END);
	m_ReserveStates.push_back(STATE_PLAYER_RUN);
	m_ReserveStates.push_back(STATE_PLAYER_WALK);
	m_ReserveStates.push_back(STATE_PLAYER_IDLE);

	return S_OK;
}

HRESULT CState_Player_Shield_Roll_Forword::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Shield_Roll_Forword::Enter(void* pArg)
{
	__super::Enter(pArg);
	_matrix pPlayerMatrix = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix());
	//EFFECT_EXCUTE("Test_Particle", pPlayerMatrix);

	m_bEndCurState = false;
	//m_fAnimSpeed = 2.5f;
	m_fAnimSpeed = 4.0f;
	m_fKeyPressTimeAcc = 0.f;

	CPlayer* pPlayer = (CPlayer*)m_pState_Handler->Get_Player();
	_float fDashGauge = ((CPlayer::PLAYERDESC*)pPlayer->Get_UnitInfo())->fDashGauge;
	((CPlayer::PLAYERDESC*)pPlayer->Get_UnitCurInfo())->fDashGauge -= fDashGauge / 3;
	((CPlayer*)m_pState_Handler->Get_Player())->Set_Target(nullptr);
}

void CState_Player_Shield_Roll_Forword::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

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

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished() ||
		KEY_DOWN(VK_SPACE))
	{
		//_matrix	Combined = m_pTargetModel->Find_HierarcyNode("Bip001")->Get_CombinedMatrix();
		//_matrix TransformMat = m_pTargetModel->Find_HierarcyNode("Bip001")->Get_TransformMatrix();
		//_matrix playerMat = pPlayerTrasform->Get_WorldMatrix();
		//CTransform* pPlayerTrasform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
		//pPlayerTrasform->Reset_RootBoneDiff();

		//_matrix CombinedMatrix = Combined * TransformMat * playerMat;
		//m_pTargetModel->Set_ModelEasingRatio(0.f);
		m_bEndCurState = true;
	}

	if (40 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_bEndCurState = true;
	}



	//CTransform* pPlayerTrasform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	//_matrix CombinedMatrix = m_pTargetModel->Find_HierarcyNode("Root_Bone")->Get_CombinedMatrix() * m_pTargetModel->Find_HierarcyNode("Root_Bone")->Get_TransformMatrix() * pPlayerTrasform->Get_WorldMatrix();

	//COUT_VECTOR(CombinedMatrix.r[0]);
	//COUT_VECTOR(CombinedMatrix.r[1]);
	//COUT_VECTOR(CombinedMatrix.r[2]);
	//COUT_VECTOR(CombinedMatrix.r[3]);
	//cout << endl;

	//Effect
	if (3 <= Get_CurKeyFrame() && 40 >= Get_CurKeyFrame())
	{
		if (!(Get_CurKeyFrame() % 4))
		{
			IMPL_ONCE(m_bEffect0, EFFECT_POS("Dash_P", m_pState_Handler->Get_Owner()->Get_WorldMatrix()));
			RESET_ONCE(m_bEffect0);
		}

		if (!(Get_CurKeyFrame() % 8))
		{
			IMPL_ONCE(m_bEffect1, EFFECT_POS("Dash_M", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(fRandom(-0.3, 0.3), 0.f, 0.f, 1.f)));
			RESET_ONCE(m_bEffect1);
		}
	}

	Set_PlayerLook(fTimeDelta);
	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Shield_Roll_Forword::Exit(void* pArg)
{
	m_fKeyPressTimeAcc = 0.f;
	__super::Exit();
}

_bool CState_Player_Shield_Roll_Forword::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if ((KEY_DOWN(VK_SHIFT) || KEY_DOWN(VK_RBUTTON))
		&& Enable_Use_Dash(pHandler) && ((CPlayer*)pHandler->Get_Player())->Get_EnableKeyInput())
	{
		if (KEY_PRESSING('W') ||
			KEY_PRESSING('A') ||
			KEY_PRESSING('S') ||
			KEY_PRESSING('D'))
		{
			return true;
		}
	}

	return false;
}


CState_Player_Shield_Roll_Forword* CState_Player_Shield_Roll_Forword::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Shield_Roll_Forword* pInstance = new CState_Player_Shield_Roll_Forword(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Shield_Roll_Forword");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Shield_Roll_Forword::Clone(void* pArg)
{
	CState_Player_Shield_Roll_Forword* pInstance = new CState_Player_Shield_Roll_Forword(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Shield_Roll_Forword");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Shield_Roll_Forword::Free()
{
}
