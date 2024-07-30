#include "stdafx.h"
#include "..\Public\State_Robella_Attack3.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"
#include "Effect_Manager.h"
#include "Imgui_Manager.h"

CState_Robella_Attack3::CState_Robella_Attack3(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Robella_Attack3::CState_Robella_Attack3(const CState_Robella_Attack3& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Robella_Attack3::Initialize_Prototype()
{
	m_ReserveStates.push_back(STATE_BOSS_ROBELLA_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_ROBELLA_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Robella_Attack3::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Robella_Attack3::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_pColCom = ((CCollider*)m_pOwner->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Robella_Attack3::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	_matrix ColMatrix = XMLoadFloat4x4(m_pState_Handler->Get_Owner()->Get_WorldMatrix());
	ColMatrix.r[3] -= XMVector3Normalize(ColMatrix.r[2]);
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 15.f);
	m_pColCom->Update(ColMatrix);

	// 해당 클래스의 애니메이션이 끝나면 Idle_state로 넘어감
	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
	{
		m_bEndCurState = true;
	}

	//65~95~120~145~160
	Hit_Unit(m_pColCom, ELEMENTAL_FIRE, m_fDamage, 65, 95, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[0]);
	Hit_Unit(m_pColCom, ELEMENTAL_FIRE, m_fDamage, 96, 120, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[1]);
	Hit_Unit(m_pColCom, ELEMENTAL_FIRE, m_fDamage, 121, 145, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[2]);

	if (70 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect0, 
			EFFECT_POS("Robella_Rotate", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, -9.4f, 0.f, 0.f), 1.f,
			XMVectorSet(1.f, 1.f, 1.f, 0.f)
			)
		);

		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Robella_Turn.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
	}

	if (93 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect1,
			EFFECT_POS("Robella_Rotate", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(0.f, 0.f, 0.f, 1.f),
				XMVectorSet(0.f, -9.4f, 0.f, 0.f), 1.f,
				XMVectorSet(1.f, 1.f, 1.f, 0.f))
		);

		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Robella_Turn.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
	}

	if (116 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect2,
			EFFECT_POS("Robella_Rotate", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(0.f, 0.f, 0.f, 1.f),
				XMVectorSet(0.f, -9.4f, 0.f, 0.f), 1.f,
				XMVectorSet(1.f, 1.f, 1.f, 0.f))
		);

		IMPL_ONCE(m_bSound[2], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Robella_Turn.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
	}

	if (135 < Get_CurKeyFrame())
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect3,
			EFFECT_POS("Robella_Rotate_Finish", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
				XMVectorSet(0.f, -1.f, 0.f, 1.f),
				XMVectorSet(0.f, -9.4f, 0.f, 0.f), 1.f,
				XMVectorSet(1.f, 1.f, 1.f, 0.f))  
		);

		IMPL_ONCE(m_bSound[3], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Robella_Turn_End.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.3f));
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Robella_Attack3::Exit(void* pArg)
{
	RESET_ONCE(m_bEffect0);
	RESET_ONCE(m_bEffect1);
	RESET_ONCE(m_bEffect2);
	RESET_ONCE(m_bEffect3);

	m_bHit[0] = false;
	m_bHit[1] = false;
	m_bHit[2] = false;
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
}

_bool CState_Robella_Attack3::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

CState_Robella_Attack3* CState_Robella_Attack3::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Robella_Attack3* pInstance = new CState_Robella_Attack3(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Robella_Attack3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Robella_Attack3::Clone(void* pArg)
{
	CState_Robella_Attack3* pInstance = new CState_Robella_Attack3(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Robella_Attack3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Robella_Attack3::Free()
{
	__super::Free();
}
