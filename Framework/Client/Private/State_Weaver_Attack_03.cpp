#include "stdafx.h"
#include "..\Public\State_Weaver_Attack_03.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

#include "Effect_Manager.h"
#include "Imgui_Manager.h"
#include "Model.h"
#include "HIerarchyNode.h"

CState_Weaver_Attack_03::CState_Weaver_Attack_03(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Weaver_Attack_03::CState_Weaver_Attack_03(const CState_Weaver_Attack_03& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Weaver_Attack_03::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_WEAVER_WEAK_BEGIN);
	m_ForceStates.push_back(STATE_BOSS_WEAVER_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_WEAVER_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_WEAVER_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Weaver_Attack_03::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Weaver_Attack_03::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAccTime = 0.f;

	m_fDamage = fRandom(450.f, 550.f);
	for (size_t i = 0; i < 6; ++i)
		m_pColCom[i] = ((CCollider*)m_pOwner->Get_Component(ID_UINT(COLLIDER, COLLIDER_AABB, COL_ENEMY_ATTACK, i)));

	EFFECT_EXCUTE("Boss_Weaver_Attack_03_KnifeThrow_Knife", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone019");
	EFFECT_EXCUTE("Boss_Weaver_Attack_03_KnifeThrow_Knife", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone020");
	EFFECT_EXCUTE("Boss_Weaver_Attack_03_KnifeThrow_Knife", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone021");
	EFFECT_EXCUTE("Boss_Weaver_Attack_03_KnifeThrow_Knife", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone022");
	EFFECT_EXCUTE("Boss_Weaver_Attack_03_KnifeThrow_Knife", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone023");
	EFFECT_EXCUTE("Boss_Weaver_Attack_03_KnifeThrow_Knife", XMMatrixIdentity(), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone024");
}

void CState_Weaver_Attack_03::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_fAccTime += fTimeDelta;

	// Effect
	m_fTimeAcc_Effect0 += fTimeDelta;
	if (0.02f < m_fTimeAcc_Effect0)
	{
		m_fTimeAcc_Effect0 = 0.f;
		Excute_Particles(m_iEffectNum0, "Bone019");
		Excute_Particles(m_iEffectNum0, "Bone020");
		Excute_Particles(m_iEffectNum0, "Bone021");
		Excute_Particles(m_iEffectNum0, "Bone022");
		Excute_Particles(m_iEffectNum0, "Bone023");
		Excute_Particles(m_iEffectNum0, "Bone024");

		m_iEffectNum0 += 1;
		if (3 < m_iEffectNum0)
			m_iEffectNum0 = 0;
	}

	Excute_IceMeshEffect(fTimeDelta, 72, 80, "Bone024", m_bEffect0);
	Excute_IceMeshEffect(fTimeDelta, 83, 90, "Bone023", m_bEffect1);
	Excute_IceMeshEffect(fTimeDelta, 93, 102, "Bone022", m_bEffect2);
	Excute_IceMeshEffect(fTimeDelta, 103, 108, "Bone021", m_bEffect3);
	Excute_IceMeshEffect(fTimeDelta, 119, 126, "Bone020", m_bEffect4);
	Excute_IceMeshEffect(fTimeDelta, 124, 130, "Bone019", m_bEffect5);
	// End Effect

	if (50 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_03.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));		
	}
	if (60 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_03.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
	}
	if (70 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[2], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_03.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
	}
	if (80 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[3], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_03.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
	}
	if (90 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[4], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_03.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
	}
	if (100 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[5], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_03.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
	}
	if (110 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[6], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_03.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
	}

	XMStoreFloat4(&m_vTargetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	m_pOwnerTranformCom->LookAt(XMLoadFloat4(&m_vTargetPos));


	for (size_t i = 0; i < 6; ++i)
		Hit_Unit(m_pColCom[i], ELEMENTAL_ICE, m_fDamage, 70, 130, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[i]);

	for (size_t i = 0; i < 6; ++i)
	{
		if (m_bHit[i])
		{
			IMPL_ONCE(m_bSound[i + 7], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_03_1.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
		}
	}


	CState_AI::Tick(fTimeDelta);
}

void CState_Weaver_Attack_03::Exit(void* pArg)
{
	for (size_t i = 0; i < 6; ++i)
		m_bHit[i] = false;

	m_fTimeAcc_Effect0 = 0.f;
	m_fTimeAcc_Effect1 = 0.f;

	RESET_ONCE(m_bEffect0);
	RESET_ONCE(m_bEffect1);
	RESET_ONCE(m_bEffect2);
	RESET_ONCE(m_bEffect3);
	RESET_ONCE(m_bEffect4);
	RESET_ONCE(m_bEffect5);
}

_bool CState_Weaver_Attack_03::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	_float4 vTargetPos;
	XMStoreFloat4(&vTargetPos, pHandler->Get_Player()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_float fDistance = XMVectorGetX(XMVector3Length(pHandler->Get_Owner()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION) - XMLoadFloat4(&vTargetPos)));

	//if (fDistance > 9.f && fDistance < 15.f )
	return true;

	//return false;
}

void CState_Weaver_Attack_03::Excute_Particles(_uint _iEffectNum0, char* _szBoneName)
{
	CHierarchyNode* pHirarchyNnode = static_cast<CModel*>((m_pState_Handler->Get_Owner()->Get_Component(ID_UINT(MODEL_DYNAMIC))))->Find_HierarcyNode(_szBoneName);
	_matrix matFollow = pHirarchyNnode->Get_CombinedMatrix() * pHirarchyNnode->Get_TransformMatrix() * XMLoadFloat4x4(m_pState_Handler->Get_Owner()->Get_WorldMatrix());
	matFollow.r[0] = XMVector3Normalize(matFollow.r[0]) * 3.f;
	matFollow.r[1] = XMVector3Normalize(matFollow.r[1]) * 3.f;
	matFollow.r[2] = XMVector3Normalize(matFollow.r[2]) * 3.f;

	if (0 == _iEffectNum0)
	{
		EFFECT_EXCUTE("Player_Cube_Object_P1", matFollow);
	}
	else if (1 == _iEffectNum0)
	{
		EFFECT_EXCUTE("Player_Cube_Object_P2", matFollow);
	}
	else if (2 == _iEffectNum0)
	{
		EFFECT_EXCUTE("Player_Cube_Object_P3", matFollow);
	}
	else if (3 == _iEffectNum0)
	{
		EFFECT_EXCUTE("Player_Cube_Object_P4", matFollow);
	}
}

void CState_Weaver_Attack_03::Excute_IceMeshEffect(_float _fTimeDelta, _uint _KeyFrameMin, _uint _KeyFrameMax, char* _szBoneName, _bool& _bEffect)
{
	if (_KeyFrameMin <= Get_CurKeyFrame() && _KeyFrameMax >= Get_CurKeyFrame())
	{
		// ¾óÀ½ ±ËÀû	
		m_fTimeAcc_Effect1 += _fTimeDelta;
		if (0.02f < m_fTimeAcc_Effect1)
		{
			m_fTimeAcc_Effect1 = 0.f;

			CHierarchyNode* pHirarchyNnode = static_cast<CModel*>((m_pState_Handler->Get_Owner()->Get_Component(ID_UINT(MODEL_DYNAMIC))))->Find_HierarcyNode(_szBoneName);
			_matrix matFollow = pHirarchyNnode->Get_CombinedMatrix() * pHirarchyNnode->Get_TransformMatrix() * XMLoadFloat4x4(m_pState_Handler->Get_Owner()->Get_WorldMatrix());
			matFollow.r[3] = XMVectorSetY(matFollow.r[3], XMVectorGetY(m_pState_Handler->Get_Owner()->Get_Pos()));
			EFFECT_EXCUTE(true == Random(0, 1) ? "Boss_Weaver_Attack_03_KnifeThrow_GroundIce_A" : "Boss_Weaver_Attack_03_KnifeThrow_GroundIce_B",
				XMMatrixScaling(1.5f, 1.5f, 1.5f) *
				XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(fRandom(-180.f, 180.f))) *
				XMMatrixTranslation(XMVectorGetX(matFollow.r[3]), XMVectorGetY(matFollow.r[3]), XMVectorGetZ(matFollow.r[3]))
			);
		}
	}
	// ¶¥¿¡ ¹ÚÈ÷´Â ¾óÀ½±âµÕ
	if (_KeyFrameMax + 1 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(_bEffect,
			CHierarchyNode * pHirarchyNnode = static_cast<CModel*>((m_pState_Handler->Get_Owner()->Get_Component(ID_UINT(MODEL_DYNAMIC))))->Find_HierarcyNode(_szBoneName);
		_matrix matFollow = pHirarchyNnode->Get_CombinedMatrix() * pHirarchyNnode->Get_TransformMatrix() * XMLoadFloat4x4(m_pState_Handler->Get_Owner()->Get_WorldMatrix());
		matFollow.r[0] = XMVector3Normalize(matFollow.r[0]);
		matFollow.r[1] = XMVector3Normalize(matFollow.r[1]);
		matFollow.r[2] = XMVector3Normalize(matFollow.r[2]);
		EFFECT_EXCUTE("Boss_Weaver_Attack_03_KnifeThrow_GroundKnife_C", XMMatrixScaling(1.5f, 1.5f, 1.5f) * matFollow, 1.1f);
		)
	}
}

CState_Weaver_Attack_03* CState_Weaver_Attack_03::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Weaver_Attack_03* pInstance = new CState_Weaver_Attack_03(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Weaver_Attack_03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Weaver_Attack_03::Clone(void* pArg)
{
	CState_Weaver_Attack_03* pInstance = new CState_Weaver_Attack_03(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Weaver_Attack_03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Weaver_Attack_03::Free()
{
	__super::Free();
}
