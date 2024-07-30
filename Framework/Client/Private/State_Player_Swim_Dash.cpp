#include "stdafx.h"
#include "..\Public\State_Player_Swim_Dash.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "HIerarchyNode.h"
#include "Player.h"
#include "Channel.h"
#include "Weapon.h"

#include "Effect_Manager.h"
#include "Imgui_Manager.h"

CState_Player_Swim_Dash::CState_Player_Swim_Dash(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Swim_Dash::CState_Player_Swim_Dash(const CState_Player_Swim_Dash& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Swim_Dash::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_RUN);
	m_ForceStates.push_back(STATE_PLAYER_WALK);
	m_ForceStates.push_back(STATE_PLAYER_HAND_IDLE);

	m_ReserveStates.push_back(STATE_PLAYER_SWIM_LOOP_A);
	m_ReserveStates.push_back(STATE_PLAYER_SWIM_IDLE);

	return S_OK;
}

HRESULT CState_Player_Swim_Dash::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Swim_Dash::Enter(void* pArg)
{
	__super::Enter(pArg);

	USEGAMEINSTANCE;
	pGameInstance->Add_RadialBlur(_float2(0.5f, 0.5f), 10.f, 0.5f, 0.5f, 1.f, 0.025f);

	_matrix pPlayerMatrix = XMLoadFloat4x4(m_pState_Handler->Get_Player()->Get_WorldMatrix());
	//EFFECT_EXCUTE("Test_Particle", pPlayerMatrix);

	m_bEndCurState = false;
	//m_fAnimSpeed = 2.5f;
	m_fAnimSpeed = 1.0f;
	m_fKeyPressTimeAcc = 0.f;

	CPlayer* pPlayer = (CPlayer*)m_pState_Handler->Get_Player();
	_float fDashGauge = ((CPlayer::PLAYERDESC*)pPlayer->Get_UnitInfo())->fDashGauge;
	((CPlayer::PLAYERDESC*)pPlayer->Get_UnitCurInfo())->fDashGauge -= fDashGauge / 3;

	CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
	pPlayer->Get_Transform()->Swim(pPlayer->Get_SwimHeight(), pPlayerNavi);

	m_pPlayer->Set_Target(nullptr);

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

void CState_Player_Swim_Dash::Tick(_float fTimeDelta)
{
	IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_Swim_Dash.mp3"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 1.f));

	USEGAMEINSTANCE;

	_float	fPlayerSpeed = ((CUnit*)m_pState_Handler->Get_Player())->Get_UnitInfo()->fSpeed;
	_float	fRunSpeed = fPlayerSpeed * 8.f;
	CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));

	m_pState_Handler->Get_Player()->Get_Transform()->Go_Front(fTimeDelta, fRunSpeed, pPlayerNavi);

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished() ||
		KEY_DOWN(VK_SPACE))
	{
		m_bEndCurState = true;
	}

	if (40 < Get_CurKeyFrame())
	{
		m_bEndCurState = true;
	}

	m_fTimeAcc_Effect0 += fTimeDelta;
	if (0.05f < m_fTimeAcc_Effect0)
	{
		m_fTimeAcc_Effect0 = 0.f;

		CHierarchyNode* pHirarchyNnode = static_cast<CModel*>((m_pState_Handler->Get_Owner()->Get_Component(ID_UINT(MODEL_DYNAMIC))))->Find_HierarcyNode("Bone_l_wp");
		_matrix matFollow = pHirarchyNnode->Get_CombinedMatrix() * pHirarchyNnode->Get_TransformMatrix() * XMLoadFloat4x4(m_pState_Handler->Get_Owner()->Get_WorldMatrix());
		_matrix OffsetMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(fRandom(-180.f, 180.f)));

		matFollow.r[0] = XMVector3Normalize(matFollow.r[0]);
		matFollow.r[1] = XMVector3Normalize(matFollow.r[1]);
		matFollow.r[2] = XMVector3Normalize(matFollow.r[2]);
		_matrix matFinalWorld = OffsetMatrix * matFollow;
		matFinalWorld.r[3] += XMVectorSet(fRandom(-0.05f, 0.05f), fRandom(-0.05f, 0.05f) + 0.3f, fRandom(-0.05f, 0.05f), 1.f);
		EFFECT_EXCUTE("Player_Swim_Loop_SoftX", matFinalWorld, 0.95f);

		OffsetMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(fRandom(-180.f, 180.f)));
		matFinalWorld = OffsetMatrix * matFollow;
		matFinalWorld.r[3] += XMVectorSet(fRandom(-0.05f, 0.05f), fRandom(-0.05f, 0.05f) + 0.3f, fRandom(-0.05f, 0.05f), 1.f);
		EFFECT_EXCUTE("Player_Swim_Loop_SoftX", matFinalWorld, 0.95f);
	}
	m_fTimeAcc_Effect1 += fTimeDelta;
	if (0.05f < m_fTimeAcc_Effect1)
	{
		m_fTimeAcc_Effect1 = 0.f;

		CHierarchyNode* pHirarchyNnode = static_cast<CModel*>((m_pState_Handler->Get_Owner()->Get_Component(ID_UINT(MODEL_DYNAMIC))))->Find_HierarcyNode("Bone_r_wp");
		_matrix matFollow = pHirarchyNnode->Get_CombinedMatrix() * pHirarchyNnode->Get_TransformMatrix() * XMLoadFloat4x4(m_pState_Handler->Get_Owner()->Get_WorldMatrix());
		_matrix OffsetMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(fRandom(-180.f, 180.f)));
		matFollow.r[0] = XMVector3Normalize(matFollow.r[0]);
		matFollow.r[1] = XMVector3Normalize(matFollow.r[1]);
		matFollow.r[2] = XMVector3Normalize(matFollow.r[2]);

		_matrix matFinalWorld = OffsetMatrix * matFollow;
		matFinalWorld.r[3] += XMVectorSet(fRandom(-0.05f, 0.05f), fRandom(-0.05f, 0.05f) + 0.3f, fRandom(-0.05f, 0.05f), 1.f);
		EFFECT_EXCUTE("Player_Swim_Loop_SoftX", matFinalWorld, 0.95f);

		OffsetMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(fRandom(-180.f, 180.f)));
		matFinalWorld = OffsetMatrix * matFollow;
		matFinalWorld.r[3] += XMVectorSet(fRandom(-0.05f, 0.05f), fRandom(-0.05f, 0.05f) + 0.3f, fRandom(-0.05f, 0.05f), 1.f);
		EFFECT_EXCUTE("Player_Swim_Loop_SoftX", matFinalWorld, 0.95f);
	}
	m_fTimeAcc_Effect2 += fTimeDelta;
	if (0.1f < m_fTimeAcc_Effect2)
	{
		m_fTimeAcc_Effect2 = 0.f;
		EFFECT_POS("Player_Swim_Loop2_SoftX", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 1.3f, -0.9f, 1.f),
			XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f,
			XMVectorSet(2.f, 1.f, 1.f, 0.f));

	}

	if (5 <= Get_CurKeyFrame() && 30 >= Get_CurKeyFrame())
	{
		m_fTimeAcc_Effect3 += fTimeDelta;
		if (0.3f < m_fTimeAcc_Effect3)
		{
			m_fTimeAcc_Effect3 = 0.f;
			EFFECT_POS("Player_Swim_Dash2", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(0.1f, 1.3f, 1.8f, 1.f),
				XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f,
				XMVectorSet(0.9f, 1.5f, 1.5f, 1.f)
			);
		}
	}

#if 0
	if (8 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect0, EFFECT_POS("Player_Swim_Dash", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 0.5f, 0.f, 1.f),
			XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f,
			XMVectorSet(1.f, 1.f, 1.f, 0.f)));
	}
	if (12 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect1, EFFECT_POS("Player_Swim_Dash", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 0.5f, 0.f, 1.f),
			XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f,
			XMVectorSet(1.f, 1.f, 1.f, 0.f)));
	}
	if (20 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect2, EFFECT_POS("Player_Swim_Dash", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 0.5f, 0.f, 1.f),
			XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f,
			XMVectorSet(1.f, 1.f, 1.f, 0.f)));
	}
	if (28 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect3, EFFECT_POS("Player_Swim", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 0.5f, 0.f, 1.f),
			XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f,
			XMVectorSet(1.f, 1.f, 1.f, 0.f)));
	}
#endif // 0


	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Swim_Dash::Exit(void* pArg)
{
	__super::Exit();
}

_bool CState_Player_Swim_Dash::Check_Condition(CState_Handler* pHandler)
{
	CPlayer* pPlayer = (CPlayer*)pHandler->Get_Player();
	_float fCurDashGauge = ((CPlayer::PLAYERDESC*)pPlayer->Get_UnitCurInfo())->fDashGauge;
	_float fDashGauge = ((CPlayer::PLAYERDESC*)pPlayer->Get_UnitInfo())->fDashGauge;

	USEGAMEINSTANCE;
	if ((KEY_DOWN(VK_SHIFT) || KEY_DOWN(VK_RBUTTON))
		&& fCurDashGauge > fDashGauge / 3)
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


CState_Player_Swim_Dash* CState_Player_Swim_Dash::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Swim_Dash* pInstance = new CState_Player_Swim_Dash(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Swim_Dash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Swim_Dash::Clone(void* pArg)
{
	CState_Player_Swim_Dash* pInstance = new CState_Player_Swim_Dash(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Swim_Dash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Swim_Dash::Free()
{
}
