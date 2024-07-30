#include "stdafx.h"
#include "..\Public\State_Weaver_Attack_08.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"
#include "Enemy_Boss_Weaver.h"

#include "Unit.h"

#include "Easing_Utillity.h"
#include "Imgui_Manager.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"

CState_Weaver_Attack_08::CState_Weaver_Attack_08(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Weaver_Attack_08::CState_Weaver_Attack_08(const CState_Weaver_Attack_08& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Weaver_Attack_08::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_WEAVER_WEAK_BEGIN);
	m_ForceStates.push_back(STATE_BOSS_WEAVER_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_WEAVER_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_WEAVER_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Weaver_Attack_08::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Weaver_Attack_08::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAccTime = 0.f;
	m_fAnimSpeed = 1.2f;

	((CEnemy_Boss_Weaver*)m_pOwner)->Set_RenderKnife(false);

	IMPL_ONCE(m_bEffect0, EFFECT_EXCUTE("Boss_Weaver_Attack_08_IceBall_B21",
		XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(fRandom(-180.f, 180.f))),
		1.1f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS, "Bone021"));
	IMPL_ONCE(m_bEffect1, EFFECT_EXCUTE("Boss_Weaver_Attack_08_IceBall_B23",
		XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(fRandom(-180.f, 180.f))),
		1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS, "Bone023"));
	IMPL_ONCE(m_bEffect2, EFFECT_EXCUTE("Boss_Weaver_Attack_08_IceBall_B19",
		XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(fRandom(-180.f, 180.f))),
		1.15f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS, "Bone019"));
	IMPL_ONCE(m_bEffect3, EFFECT_EXCUTE("Boss_Weaver_Attack_08_IceBall_B24",
		XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(fRandom(-180.f, 180.f))),
		1.15f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS, "Bone024"));
	IMPL_ONCE(m_bEffect4, EFFECT_EXCUTE("Boss_Weaver_Attack_08_IceBall_B22",
		XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(fRandom(-180.f, 180.f))),
		1.15f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS, "Bone022"));
	IMPL_ONCE(m_bEffect5, EFFECT_EXCUTE("Boss_Weaver_Attack_08_IceBall_B20",
		XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(fRandom(-180.f, 180.f))),
		1.1f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS, "Bone020"));

	for (size_t i = 0; i < 6; ++i)
		m_pColCom[i] = ((CCollider*)m_pOwner->Get_Component(ID_UINT(COLLIDER, COLLIDER_AABB, COL_ENEMY_ATTACK, i)));
}

void CState_Weaver_Attack_08::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	IMPL_ONCE(m_bSound[12], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_08_Begin.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.3f));

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

	Excute_IceMeshEffect(fTimeDelta, 66, "Bone021", m_bEffect6);
	Excute_IceMeshEffect(fTimeDelta, 106, "Bone023", m_bEffect7);
	Excute_IceMeshEffect(fTimeDelta, 160, "Bone019", m_bEffect8);
	Excute_IceMeshEffect(fTimeDelta, 198, "Bone024", m_bEffect9);
	Excute_IceMeshEffect(fTimeDelta, 236, "Bone022", m_bEffect10);
	Excute_IceMeshEffect(fTimeDelta, 306, "Bone020", m_bEffect11);

	XMStoreFloat4(&m_vTargetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	m_pOwnerTranformCom->LookAt(XMLoadFloat4(&m_vTargetPos));

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	/////////////////////////////////
	// 순서 - 뼈번호 - 키프레임 구간 //
	/////////////////////////////////
	//  1 -	  21 -    58~66   : (0프레임부터 마지막프레임까지 시간 : 2.2초)
	//  2 -	  23 -    102~106 : (0프레임부터 마지막프레임까지 시간 : 3.21초)
	//  3 -	  19 -    150~160 : (0프레임부터 마지막프레임까지 시간 : 5.33초)
	//  4 -	  24 -    192~198 : (0프레임부터 마지막프레임까지 시간 : 6.6초)
	//  5 -	  22 -    231~236 : (0프레임부터 마지막프레임까지 시간 : 7.87초)
	//  6 -	  20 -    300~306 : (0프레임부터 마지막프레임까지 시간 : 10.2초)
	if(58 < Get_CurKeyFrame() && 66 > Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_08.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
	}
	else if(66 <= Get_CurKeyFrame())		
		IMPL_ONCE(m_bSound[6], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_08_Boom.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));

	if (102 < Get_CurKeyFrame() && 106 > Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_08.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
	}
	else if (106 <= Get_CurKeyFrame())
		IMPL_ONCE(m_bSound[7], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_08_Boom.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));

	if (150 < Get_CurKeyFrame() && 160 > Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[2], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_08.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
	}
	else if (160 <= Get_CurKeyFrame())
		IMPL_ONCE(m_bSound[8], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_08_Boom.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));

	if (192 < Get_CurKeyFrame() && 198 > Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[3], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_08.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
	}
	else if (198 <= Get_CurKeyFrame())
		IMPL_ONCE(m_bSound[9], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_08_Boom.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));

	if (231 < Get_CurKeyFrame() && 236 > Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[4], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_08.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
	}
	else if (236 <= Get_CurKeyFrame())
		IMPL_ONCE(m_bSound[10], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_08_Boom.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));

	if (300 < Get_CurKeyFrame() && 306 > Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[5], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_08.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));
	}
	else if (306 <= Get_CurKeyFrame())
		IMPL_ONCE(m_bSound[11], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_08_Boom.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));

	Hit_Unit(m_pColCom[0], ELEMENTAL_ICE, fRandom(550.f, 650.f), 150, 160, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[0]);
	Hit_Unit(m_pColCom[1], ELEMENTAL_ICE, fRandom(550.f, 650.f), 300, 306, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[1]);
	Hit_Unit(m_pColCom[2], ELEMENTAL_ICE, fRandom(550.f, 650.f), 58, 66, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[2]);
	Hit_Unit(m_pColCom[3], ELEMENTAL_ICE, fRandom(550.f, 650.f), 231, 236, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[3]);
	Hit_Unit(m_pColCom[4], ELEMENTAL_ICE, fRandom(550.f, 650.f), 102, 106, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[4]);
	Hit_Unit(m_pColCom[5], ELEMENTAL_ICE, fRandom(550.f, 650.f), 192, 198, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[5]);

	if (315 <= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bRenderKnife, ((CEnemy_Boss_Weaver*)m_pOwner)->Set_RenderKnife(true));
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Weaver_Attack_08::Exit(void* pArg)
{
	RESET_ONCE(m_bEffect0);
	RESET_ONCE(m_bEffect1);
	RESET_ONCE(m_bEffect2);
	RESET_ONCE(m_bEffect3);
	RESET_ONCE(m_bEffect4);
	RESET_ONCE(m_bEffect5);
	RESET_ONCE(m_bEffect6);
	RESET_ONCE(m_bEffect7);
	RESET_ONCE(m_bEffect8);
	RESET_ONCE(m_bEffect9);
	RESET_ONCE(m_bEffect10);
	RESET_ONCE(m_bEffect11);
	RESET_ONCE(m_bRenderKnife);

	for (size_t i = 0; i < 6; ++i)
		m_bHit[i] = false;

	((CEnemy_Boss_Weaver*)m_pOwner)->Set_RenderKnife(true);
}

_bool CState_Weaver_Attack_08::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	//CUnit* pOwner = ((CUnit*)pHandler->Get_Owner());
	//_float4 vTargetPos;
	//XMStoreFloat4(&vTargetPos, pHandler->Get_Player()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//
	//_float fDistance = XMVectorGetX(XMVector3Length(pHandler->Get_Owner()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION) - XMLoadFloat4(&vTargetPos)));
	//
	//if (fDistance < 5.f && (pOwner->Get_UnitInfo()->fHP) * 0.7f < pOwner->Get_UnitCurInfo()->fHP)
	//	return false;

	return true;
}

void CState_Weaver_Attack_08::Excute_Particles(_uint _iEffectNum0, char* _szBoneName)
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

void CState_Weaver_Attack_08::Excute_IceMeshEffect(_float _fTimeDelta, _uint _KeyFrameMax, char* _szBoneName, _bool& _bEffect)
{
	if (_KeyFrameMax <= Get_CurKeyFrame())
	{
		IMPL_ONCE(_bEffect,
			CHierarchyNode * pHirarchyNnode = static_cast<CModel*>((m_pState_Handler->Get_Owner()->Get_Component(ID_UINT(MODEL_DYNAMIC))))->Find_HierarcyNode(_szBoneName);
		_matrix matFollow = pHirarchyNnode->Get_CombinedMatrix() * pHirarchyNnode->Get_TransformMatrix() * XMLoadFloat4x4(m_pState_Handler->Get_Owner()->Get_WorldMatrix());
		EFFECT_EXCUTE("Boss_Weaver_Attack_08_IceBall_Explosion3",
			XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(fRandom(-180.f, 180.f))) *
			XMMatrixTranslation(XMVectorGetX(matFollow.r[3]), XMVectorGetY(matFollow.r[3]), XMVectorGetZ(matFollow.r[3]))
		);
		EFFECT_EXCUTE("Boss_Weaver_Attack_08_IceBall_Explosion_P",
			XMMatrixTranslation(XMVectorGetX(matFollow.r[3]), XMVectorGetY(matFollow.r[3]), XMVectorGetZ(matFollow.r[3]))
		);
		)
	}
}

CState_Weaver_Attack_08* CState_Weaver_Attack_08::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Weaver_Attack_08* pInstance = new CState_Weaver_Attack_08(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Weaver_Attack_08");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Weaver_Attack_08::Clone(void* pArg)
{
	CState_Weaver_Attack_08* pInstance = new CState_Weaver_Attack_08(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Weaver_Attack_08");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Weaver_Attack_08::Free()
{
	__super::Free();
}
