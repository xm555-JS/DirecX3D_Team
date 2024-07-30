#include "stdafx.h"
#include "..\Public\State_Player_Swim_Loop_B.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "HIerarchyNode.h"
#include "Player.h"
#include "Channel.h"
#include "Weapon.h"

#include "Effect_Manager.h"


CState_Player_Swim_Loop_B::CState_Player_Swim_Loop_B(CState_Manager* pManager, STATE_TYPE eState)
	: CState_Player(pManager, eState)
{
}

CState_Player_Swim_Loop_B::CState_Player_Swim_Loop_B(const CState_Player_Swim_Loop_B& rhs)
	: CState_Player(rhs)
{
}

HRESULT CState_Player_Swim_Loop_B::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_PLAYER_SWIM_DASH);
	m_ForceStates.push_back(STATE_PLAYER_SWIM_IDLE);
	m_ForceStates.push_back(STATE_PLAYER_SKATE_CALL);

	m_ForceStates.push_back(STATE_PLAYER_RUN);
	m_ForceStates.push_back(STATE_PLAYER_WALK);
	m_ForceStates.push_back(STATE_PLAYER_HAND_IDLE);

	//m_ReserveStates.push_back(STATE_PLAYER_SWIM_LOOP_B);

	return S_OK;
}

HRESULT CState_Player_Swim_Loop_B::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Player_Swim_Loop_B::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_bEndCurState = true;
	m_fAnimSpeed = 1.0f;
	m_fKeyPressTimeAcc = 0.f;
	m_iCurKeyFrame = 0;

	CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));
	m_pState_Handler->Get_Player()->Get_Transform()->Swim(static_cast<CPlayer*>(m_pState_Handler->Get_Player())->Get_SwimHeight(), pPlayerNavi);

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

	m_pPlayer->Get_Mobility_Glider()->Set_IsDissolve(true);
	m_pPlayer->Get_Mobility_Glider()->Set_IsUsing(false);
	m_pPlayer->Get_Mobility_Glider()->Set_IsGlow(true);

	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsDissolve(true);
	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsUsing(false);
	m_pPlayer->Get_Mobility_MotorCycle()->Set_IsGlow(true);


}

void CState_Player_Swim_Loop_B::Tick(_float fTimeDelta)
{
	Use_StaminaGauge(0.001f);
	Set_PlayerLook(fTimeDelta);

	USEGAMEINSTANCE;
	if (KEY_PRESSING('W') ||
		KEY_PRESSING('A') ||
		KEY_PRESSING('S') ||
		KEY_PRESSING('D'))
	{
		_float	fPlayerSpeed = ((CUnit*)m_pState_Handler->Get_Player())->Get_UnitInfo()->fSpeed;
		_float	fRunSpeed = fPlayerSpeed * 4.f;
		CNavigation* pPlayerNavi = (CNavigation*)m_pState_Handler->Get_Player()->Get_Component(ID_UINT(NAVIGATION));

		m_pState_Handler->Get_Player()->Get_Transform()->Go_Front(fTimeDelta, fRunSpeed, pPlayerNavi);
	}

	//if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished() ||
	//	KEY_DOWN(VK_SPACE))
	//{
	//	m_bEndCurState = true;
	//}

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
	IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_Swim.mp3"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 1.f));

	IMPL_ALARM(m_fSound[0],
		m_eSoundChannel[2] = MGR(CSound_Manager)->PlaySurroundSound(TEXT("Player_Swim.mp3"), CSound_Manager::CHANNEL_PLAYER, m_pState_Handler->Get_Owner()->Get_Pos(), 1.f)
		, 0.7f, fTimeDelta);

	

	CState_Player::Tick(fTimeDelta);
}

void CState_Player_Swim_Loop_B::Exit(void* pArg)
{

	MGR(CSound_Manager)->StopSound(m_eSoundChannel[2]);

	RESET_ONCE(m_bEffect6);
	RESET_ONCE(m_bEffect7);
	RESET_ONCE(m_bEffect8);
	RESET_ONCE(m_bEffect9);

	__super::Exit();
}

_bool CState_Player_Swim_Loop_B::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	if (KEY_PRESSING('W') ||
		KEY_PRESSING('A') ||
		KEY_PRESSING('S') ||
		KEY_PRESSING('D'))
	{
		_float4 vPos;
		XMStoreFloat4(&vPos, pHandler->Get_Player()->Get_Pos());
		if (vPos.y < static_cast<CPlayer*>(pHandler->Get_Player())->Get_SwimHeight() + 0.1f)
			return true;
	}

	return false;
}


CState_Player_Swim_Loop_B* CState_Player_Swim_Loop_B::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Player_Swim_Loop_B* pInstance = new CState_Player_Swim_Loop_B(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Player_Swim_Loop_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Player_Swim_Loop_B::Clone(void* pArg)
{
	CState_Player_Swim_Loop_B* pInstance = new CState_Player_Swim_Loop_B(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Player_Swim_Loop_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Player_Swim_Loop_B::Free()
{
}
