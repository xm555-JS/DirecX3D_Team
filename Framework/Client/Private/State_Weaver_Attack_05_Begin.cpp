#include "stdafx.h"
#include "..\Public\State_Weaver_Attack_05_Begin.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "HIerarchyNode.h"
#include "Player.h"

#include "Unit.h"

#include "Imgui_Manager.h"
#include "Easing_Utillity.h"

#include "Effect_Manager.h"

CState_Weaver_Attack_05_Begin::CState_Weaver_Attack_05_Begin(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Weaver_Attack_05_Begin::CState_Weaver_Attack_05_Begin(const CState_Weaver_Attack_05_Begin& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Weaver_Attack_05_Begin::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_WEAVER_WEAK_BEGIN);
	m_ForceStates.push_back(STATE_BOSS_WEAVER_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_2);

	m_ReserveProbabilitys[STATE_BOSS_WEAVER_ATTACK_05_2] = 1;

	return S_OK;
}

HRESULT CState_Weaver_Attack_05_Begin::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Weaver_Attack_05_Begin::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fEasingTime = m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_Duration();
	m_fAccTime = 0.f;
	m_fAnimSpeed = 5.f;
	m_fMoveSpeed = 0.f;
	m_fDamage = fRandom(150.f, 250.f);

	m_fDir *= -1.f;

	XMStoreFloat4(&m_vStartLook, m_pState_Handler->Get_Owner()->Get_Transform()->Get_State(CTransform::STATE_LOOK));
	XMStoreFloat4(&m_vStartPos, m_pOwner->Get_Pos());
	m_vStartPos.w = 1.f;

	XMStoreFloat4(&m_vTargetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_pColCom = ((CCollider*)m_pOwner->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK, 6)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Weaver_Attack_05_Begin::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;


	// 동시에 생성
	if (38 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect1, EFFECT_EXCUTE("Boss_Weaver_Attack_02_KnifeAttack_Knife", XMMatrixIdentity(), 2.4f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone019"));
		IMPL_ONCE(m_bEffect2, EFFECT_EXCUTE("Boss_Weaver_Attack_02_KnifeAttack_Knife", XMMatrixIdentity(), 2.4f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone020"));
		IMPL_ONCE(m_bEffect3, EFFECT_EXCUTE("Boss_Weaver_Attack_02_KnifeAttack_Knife", XMMatrixIdentity(), 2.4f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone021"));
		IMPL_ONCE(m_bEffect4, EFFECT_EXCUTE("Boss_Weaver_Attack_02_KnifeAttack_Knife", XMMatrixIdentity(), 2.4f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone022"));
		IMPL_ONCE(m_bEffect5, EFFECT_EXCUTE("Boss_Weaver_Attack_02_KnifeAttack_Knife", XMMatrixIdentity(), 2.4f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone023"));
		IMPL_ONCE(m_bEffect6, EFFECT_EXCUTE("Boss_Weaver_Attack_02_KnifeAttack_Knife", XMMatrixIdentity(), 2.4f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL, "Bone024"));
	}
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
#if 0
	m_fTimeAcc_Effect1 += fTimeDelta;
	if (0.05f < m_fTimeAcc_Effect1)
	{
		m_fTimeAcc_Effect1 = 0;
		if (m_bToggle_Effect)
			EFFECT_EXCUTE("Run_P", XMMatrixScaling(DEBUGFLOAT, DEBUGFLOAT, DEBUGFLOAT), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL);
		else
			EFFECT_EXCUTE("Run_P_90", XMMatrixScaling(DEBUGFLOAT, DEBUGFLOAT, DEBUGFLOAT), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL);
		m_bToggle_Effect = !m_bToggle_Effect;
	}
#endif // 0




	_matrix ColMatrix = XMLoadFloat4x4(m_pState_Handler->Get_Owner()->Get_WorldMatrix());
	ColMatrix.r[3] -= XMVector3Normalize(ColMatrix.r[2]) * 2.f;
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale) * 7.f);
	m_pColCom->Update(ColMatrix);

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	Hit_Unit(m_pColCom, ELEMENTAL_ICE, m_fDamage, 80, 140, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit);
	
	if (80 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_fAccTime += fTimeDelta;
		m_fAnimSpeed = 5.f;

		m_fMoveSpeed = XMVectorGetX(CEasing_Utillity::SinIn(XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(10.f, 0.f, 0.f, 0.f), m_fAccTime, 1.f));
		//_vector vCurPos = CEasing_Utillity::SinIn(XMLoadFloat4(&m_vStartPos), XMLoadFloat4(&m_vTargetPos), m_fAccTime, DEBUGFLOAT4(0).w);
		//m_pOwnerTranformCom->Set_State(CTransform::STATE_TRANSLATION, vCurPos);
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Dash.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));

		m_pOwnerTranformCom->Go_Front(fTimeDelta, m_fMoveSpeed, m_pNaviCom);
	}
	else
	{
		m_fAccTime += fTimeDelta * m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_TickperSecond();

		_vector vLook = XMLoadFloat4(&m_vTargetPos) - m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION);

		_float	fCurTime = min(1.f, m_fAccTime / 79.f);

		_vector vFinalLook = XMVectorLerp(XMLoadFloat4(&m_vStartLook), vLook, fCurTime);

		m_pOwnerTranformCom->YFixedLook(vFinalLook);
	}

	STATE_TYPE	eNextState = CState_AI::Ready_NextState();
	if (STATE_END != eNextState)
	{
		m_pState_Handler->Change_State(eNextState, &m_bHit);
	}

	m_pTargetModel->Play_Animation(fTimeDelta * m_fAnimSpeed);
}

void CState_Weaver_Attack_05_Begin::Exit(void* pArg)
{
	RESET_ONCE(m_bEffect1);
	RESET_ONCE(m_bEffect2);
	RESET_ONCE(m_bEffect3);
	RESET_ONCE(m_bEffect4);
	RESET_ONCE(m_bEffect5);
	RESET_ONCE(m_bEffect6);
	m_bHit = false;
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
}

_bool CState_Weaver_Attack_05_Begin::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

void CState_Weaver_Attack_05_Begin::Excute_Particles(_uint _iEffectNum0, char* _szBoneName)
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

CState_Weaver_Attack_05_Begin* CState_Weaver_Attack_05_Begin::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Weaver_Attack_05_Begin* pInstance = new CState_Weaver_Attack_05_Begin(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Weaver_Attack_05_Begin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Weaver_Attack_05_Begin::Clone(void* pArg)
{
	CState_Weaver_Attack_05_Begin* pInstance = new CState_Weaver_Attack_05_Begin(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Weaver_Attack_05_Begin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Weaver_Attack_05_Begin::Free()
{
	__super::Free();
}
