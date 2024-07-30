#include "stdafx.h"
#include "..\Public\State_Player_Cube_Attack_Skill_Normal.h"
#include "GameInstance.h"
#include "Channel.h"
#include "Bullet_Player_Cube.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "GameObject.h"
#include "Player.h"
#include "Weapon_ShieldAxe.h"

#include "Imgui_Manager.h"

CState_Player_Cube_Attack_Skill_Normal::CState_Player_Cube_Attack_Skill_Normal(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Cube_Attack_Skill_Normal::CState_Player_Cube_Attack_Skill_Normal(const CState_Player_Cube_Attack_Skill_Normal& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Cube_Attack_Skill_Normal::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_JUMP_A);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_SHIELD_ATTACK_A);
	m_ReserveStates.push_back(STATE_PLAYER_WALK);
	m_ReserveStates.push_back(STATE_PLAYER_IDLE);

	return S_OK;
}

HRESULT CState_Player_Cube_Attack_Skill_Normal::Initialize(void* pArg)
{

	return S_OK;
}

void CState_Player_Cube_Attack_Skill_Normal::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fKeyPressTimeAcc = 0.f;
	m_fAnimSpeed = 2.f;

	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(true);
	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
	EFFECT_EXCUTE("Player_Cube_Attack_Charge_P", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS, "Bone_l_wp");
}

void CState_Player_Cube_Attack_Skill_Normal::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (45 > Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Charge1.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 0.5f));
		IMPL_ALARM(m_fSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Charge2.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 0.5f), 0.5f, fTimeDelta);
	}

	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 6.f);

	Hit_Unit(m_pColCom, ELEMENTAL_ICE, fRandom(500.f, 600.f), 50, 65, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0, "Player_Hit_Cube");

	if (m_bHit0)
	{
		IMPL_ONCE(m_bSound[10], MGR(CSound_Manager)->PlaySurroundSound(TEXT("CubeHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge0, m_pPlayer->Set_SkillGauge(fRandom(500.f, 600.f)));
	}

	if (45 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Cube_Attack_Skill_Normal.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect0, EFFECT_POS("Player_Cube_Ground", m_pState_Handler->Get_Owner()->Get_WorldMatrix()));
		IMPL_ONCE(m_bEffect1, EFFECT_POS("Player_Cube_Skill_A_P", m_pState_Handler->Get_Owner()->Get_WorldMatrix()));
		IMPL_ONCE(m_bEffect2, EFFECT_EXCUTE("Player_Cube_Skill_B", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Player(), CEffect_Manager::FOLLOW_POS), "Bip001-Pelvis");

		m_pPlayer->Set_Shield(0.5f);

		IMPL_ONCE(m_bShaked0,
			pGameInstance->Add_Camera_Shaking(XMVectorSet(-1.f, 0.f, 0.f, 0.f), 0.04f, 0.2f, 0.2f);
			pGameInstance->Add_DirCA(_float2(-1.f, 1.f), _float4(0.008f, 0.f, 0.004f, 0.f), 0.4f, 0.4f);
		);
	}

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	if (!m_bShoot && 50 < Get_CurKeyFrame())
	{
		m_bShoot = true;
		list<CGameObject*>* CubeBullets = (GAMEINSTANCE->Get_Objects(LEVEL_GAMEPLAY, ID_UINT(PLAYER, WEAPON_CUBE_BULLET)));
		if (nullptr != CubeBullets && 0 != CubeBullets->size())
		{
			for (auto& iter : *CubeBullets)
			{
				((CBullet_Player_Cube*)iter)->Set_Shoot();
			}
		}
	}

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Cube_Attack_Skill_Normal::Exit(void* pArg)
{
	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(false);

	m_bShoot = false;

	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	__super::Exit();
}

_bool CState_Player_Cube_Attack_Skill_Normal::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (KEY_DOWN('1'))
	{
		return true;
	}

	return false;
}


CState_Player_Cube_Attack_Skill_Normal* CState_Player_Cube_Attack_Skill_Normal::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Cube_Attack_Skill_Normal* pInstance = new CState_Player_Cube_Attack_Skill_Normal(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Cube_Attack_Skill_Normal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Cube_Attack_Skill_Normal::Clone(void* pArg)
{
	CState_Player_Cube_Attack_Skill_Normal* pInstance = new CState_Player_Cube_Attack_Skill_Normal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Cube_Attack_Skill_Normal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Cube_Attack_Skill_Normal::Free()
{
}
