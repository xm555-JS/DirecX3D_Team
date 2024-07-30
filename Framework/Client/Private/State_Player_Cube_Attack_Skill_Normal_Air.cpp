#include "stdafx.h"
#include "..\Public\State_Player_Cube_Attack_Skill_Normal_Air.h"
#include "GameInstance.h"
#include "Navigation.h"
#include "GameObject.h"
#include "Player.h"
#include "Bullet_Player_Cube.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"
#include "Channel.h"

CState_Player_Cube_Attack_Skill_Normal_Air::CState_Player_Cube_Attack_Skill_Normal_Air(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Cube_Attack_Skill_Normal_Air::CState_Player_Cube_Attack_Skill_Normal_Air(const CState_Player_Cube_Attack_Skill_Normal_Air& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Cube_Attack_Skill_Normal_Air::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_JUMP_B);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_FORWORD_AIR);
	m_ForceStates.push_back(STATE_PLAYER_ROLL_BACK_AIR);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	m_ReserveStates.push_back(STATE_PLAYER_JUMP_FALL);//
	m_ReserveStates.push_back(STATE_PLAYER_JUMP_END);//	,

	return S_OK;
}

HRESULT CState_Player_Cube_Attack_Skill_Normal_Air::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Player_Cube_Attack_Skill_Normal_Air::Enter(void* pArg)
{
	__super::Enter(pArg);
	USEGAMEINSTANCE;
	m_fAnimSpeed = 1.5f;
	m_fKeyPressTimeAcc = 0.f;

	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(true);

	if (45 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect0, EFFECT_POS("Player_Cube_Ground", m_pState_Handler->Get_Owner()->Get_WorldMatrix()));
		IMPL_ONCE(m_bEffect1, EFFECT_POS("Player_Cube_Skill_A_P", m_pState_Handler->Get_Owner()->Get_WorldMatrix()));
		IMPL_ONCE(m_bEffect2, EFFECT_EXCUTE("Player_Cube_Skill_B", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Player(), CEffect_Manager::FOLLOW_POS), "Bip001-Pelvis");

		m_pPlayer->Set_Shield(0.5f);
	}

	CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
	m_pState_Handler->Get_Player()->Get_Transform()->Jump(8.5f, pPlayerNavi);

	m_pColCom = ((CCollider*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
	EFFECT_EXCUTE("Player_Cube_Attack_Charge_P", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS, "Bone_l_wp");
}

void CState_Player_Cube_Attack_Skill_Normal_Air::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 6.f);

	Hit_Unit(m_pColCom, ELEMENTAL_ICE, fRandom(700.f, 800.f), 55, 70, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit0, "Player_Hit_Cube");

	if (m_bHit0)
	{
		IMPL_ONCE(m_bSound[10], MGR(CSound_Manager)->PlaySurroundSound(TEXT("CubeHit_A.wav"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 0.5f));
		IMPL_ONCE(m_bGauge0, m_pPlayer->Set_SkillGauge(fRandom(700.f, 800.f)));
	}

	if (45 > Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Charge1.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 0.5f));
		IMPL_ALARM(m_fSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Charge2.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 0.5f), 0.5f, fTimeDelta);
	}

	if (45 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Cube_Attack_Skill_Normal.wav"), CSound_Manager::CHANNEL_PLAYER_EFFECT, m_pPlayer->Get_Pos(), 0.5f));

		IMPL_ONCE(m_bEffect0, EFFECT_POS("Player_Cube_Ground", m_pState_Handler->Get_Owner()->Get_WorldMatrix()));
		IMPL_ONCE(m_bEffect1, EFFECT_POS("Player_Cube_Skill_A_P", m_pState_Handler->Get_Owner()->Get_WorldMatrix()));
		IMPL_ONCE(m_bEffect2, EFFECT_EXCUTE("Player_Cube_Skill_B", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Player(), CEffect_Manager::FOLLOW_POS), "Bip001-Pelvis");

		IMPL_ONCE(m_bShaked0,
			pGameInstance->Add_Camera_Shaking(XMVectorSet(-1.f, 0.f, 0.f, 0.f), 0.048f, 0.5f, 0.5f);
			pGameInstance->Add_DirCA(_float2(-1.f, 1.f), _float4(0.008f, 0.f, 0.004f, 0.f), 0.4f, 0.4f);
		);
	}

	//Hit_Unit(m_pColCom, ELEMENTAL_FIRE, 5.f, 7, 10, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit, "Player_Sword_Hit_Degree90");
	//m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 3.f);

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	if (!m_bShoot && 55 < Get_CurKeyFrame())
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

void CState_Player_Cube_Attack_Skill_Normal_Air::Exit(void* pArg)
{
	CTransform* pPlayerTransform = (CTransform*)m_pState_Handler->Get_Player()->Get_Component(TRANSFORM);
	pPlayerTransform->Set_YFixState(false);
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
	m_bShoot = false;
	__super::Exit();
}

_bool CState_Player_Cube_Attack_Skill_Normal_Air::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	if (KEY_DOWN('1') && Enable_Use_Stamina(pHandler))
	{
		return true;
	}

	return false;
}


CState_Player_Cube_Attack_Skill_Normal_Air* CState_Player_Cube_Attack_Skill_Normal_Air::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Cube_Attack_Skill_Normal_Air* pInstance = new CState_Player_Cube_Attack_Skill_Normal_Air(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Cube_Attack_Skill_Normal_Air");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Cube_Attack_Skill_Normal_Air::Clone(void* pArg)
{
	CState_Player_Cube_Attack_Skill_Normal_Air* pInstance = new CState_Player_Cube_Attack_Skill_Normal_Air(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Cube_Attack_Skill_Normal_Air");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Cube_Attack_Skill_Normal_Air::Free()
{
}
