#include "stdafx.h"
#include "..\Public\State_Dragon_Attack3.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "HIerarchyNode.h"
#include "Player.h"
#include "Enemy_Boss_Dragon.h"

#include "Effect_Manager.h"
#include "Channel.h"

#include "Imgui_Manager.h"

CState_Dragon_Attack3::CState_Dragon_Attack3(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Dragon_Attack3::CState_Dragon_Attack3(const CState_Dragon_Attack3& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Dragon_Attack3::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_DRAGON_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_DRAGON_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_DRAGON_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Dragon_Attack3::Initialize(void* pArg)
{
	m_bOnceSkill = true;
	return S_OK;
}

void CState_Dragon_Attack3::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fDamage = fRandom(900.f, 1100.f);

	m_pColCom = ((CCollider*)m_pOwner->Get_Component(ID_UINT(COLLIDER, COLLIDER_OBB, COL_ENEMY_ATTACK)));
}

void CState_Dragon_Attack3::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	m_fAnimSpeed = 1.f;

	CHierarchyNode* pHierarchyNode = m_pTargetModel->Find_HierarcyNode("tongue_01");
	_matrix HierarchyMatrix = (pHierarchyNode->Get_OffsetMatrix() * pHierarchyNode->Get_CombinedMatrix()
		* pHierarchyNode->Get_TransformMatrix() * m_pOwnerTranformCom->Get_WorldMatrix());

	_matrix OffsetMatrix;

	//30
	if (53.f < Get_CurKeyFrame() && 140.f >= Get_CurKeyFrame())
	{
		_float4 vPos;
		_vector vOut;

		CHierarchyNode* pHierarchyNode = m_pTargetModel->Find_HierarcyNode("tongue_01");
		_matrix HierarchyMatrix = (pHierarchyNode->Get_OffsetMatrix() * pHierarchyNode->Get_CombinedMatrix()
			* pHierarchyNode->Get_TransformMatrix() * m_pOwnerTranformCom->Get_WorldMatrix());

		pGameInstance->Picking(152.f, HierarchyMatrix.r[3], HierarchyMatrix.r[0], &vOut);
		XMStoreFloat4(&vPos, vOut);
		OffsetMatrix = XMMatrixTranslation(vPos.x, 152.f, vPos.z);

		_float4x4 matOffset;
		XMStoreFloat4x4(&matOffset, OffsetMatrix);
		m_fDigTime += fTimeDelta;
		if (0.01f < m_fDigTime)
		{
			EFFECT_POS("Boss_Dragon_Laser_Dig_Red", &matOffset, XMVectorSet(0.f, -0.5f, 0.f, 0.1f), XMVectorSet(0.f, XMConvertToRadians(fRandom(-180.f, 180.f)), 0.f, 0.f), 1.f, XMVectorSet(3.f, 3.f, 3.f, 0.f));
			m_fDigTime = 0.f;
		}
	}


	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, m_fDamage, 51, 180, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit);

	OffsetMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(-90.f))
		* XMMatrixTranslation(1.5f, 0.f, 0.f);

	IMPL_ONCE(m_bOnce, EFFECT_EXCUTE("Boss_Dragon_Laser_2Phase", OffsetMatrix, 1.f, m_pOwner, CEffect_Manager::FOLLOW_POS_RUL, "tongue_01"));

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	// »ç¿îµå
	IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Laser_Ready.wav"),
		CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));

	if (20 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[2], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Laser_Charge.wav"),
			CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
	}

	if (43 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Laser_Shot.wav"),
			CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Dragon_Attack3::Exit(void* pArg)
{
	m_pTargetModel->Set_ModelEasingRatio(0.2f);
	m_bHit = false;
	RESET_ONCE(m_bOnce);
}

_bool CState_Dragon_Attack3::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	CGameObject* pOwner = pHandler->Get_Owner();
	CGameObject* pTarget = ((CEnemy*)pHandler->Get_Owner())->Get_Target();

	_float4 vTargetPos;
	XMStoreFloat4(&vTargetPos, pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	vTargetPos.y = XMVectorGetY(pOwner->Get_Pos());

	_float fDistance = XMVectorGetX(XMVector3Length(pOwner->Get_Pos() - XMLoadFloat4(&vTargetPos)));

	if (19.f < fDistance)
		return false;

	return true;
}

CState_Dragon_Attack3* CState_Dragon_Attack3::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Dragon_Attack3* pInstance = new CState_Dragon_Attack3(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Dragon_Attack3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Dragon_Attack3::Clone(void* pArg)
{
	CState_Dragon_Attack3* pInstance = new CState_Dragon_Attack3(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Dragon_Attack3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Dragon_Attack3::Free()
{
	__super::Free();
}
