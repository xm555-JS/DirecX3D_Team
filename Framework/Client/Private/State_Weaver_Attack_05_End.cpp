#include "stdafx.h"
#include "..\Public\State_Weaver_Attack_05_End.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Imgui_Manager.h"
#include "Easing_Utillity.h"

#include "Effect_Manager.h"
#include "HIerarchyNode.h"

CState_Weaver_Attack_05_End::CState_Weaver_Attack_05_End(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Weaver_Attack_05_End::CState_Weaver_Attack_05_End(const CState_Weaver_Attack_05_End& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Weaver_Attack_05_End::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_WEAVER_WEAK_BEGIN);
	m_ForceStates.push_back(STATE_BOSS_WEAVER_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_WEAVER_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_WEAVER_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Weaver_Attack_05_End::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Weaver_Attack_05_End::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAccTime = 0.f;
	m_fAnimSpeed = 5.f;
	m_pTargetModel->Set_ModelEasingRatio(1.f);
	m_fDamage = fRandom(150.f, 250.f);

	if (nullptr != pArg)
		memcpy(&m_bHit, pArg, sizeof(_bool));

	m_fDir *= -1.f;

	XMStoreFloat4(&m_vStartPos, m_pOwner->Get_Pos());
	m_vStartPos.w = 1.f;
	XMStoreFloat4(&m_vStartLook, m_pState_Handler->Get_Owner()->Get_Transform()->Get_State(CTransform::STATE_LOOK));

	m_pColCom = ((CCollider*)m_pOwner->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK, 6)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());
}

void CState_Weaver_Attack_05_End::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

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

	
	m_fAccTime += fTimeDelta;
		
	//_vector vCurLook = CEasing_Utillity::sinfOut(XMLoadFloat4(&m_vStartLook), (XMLoadFloat4(&m_vTargetPos) - m_pOwner->Get_Pos()), m_fAccTime, 1.f);
	//m_pOwnerTranformCom->YFixedLook(vCurLook);

	if (30 > m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		_float fMoveSpeed = XMVectorGetX(CEasing_Utillity::SinIn(XMVectorSet(20.f, 0.f, 0.f, 0.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), m_fAccTime, 0.5f));

		m_pOwnerTranformCom->Go_Front(fTimeDelta, fMoveSpeed, m_pNaviCom);
	}
	else
		m_fAnimSpeed = 3.f;

	CState_AI::Tick(fTimeDelta);
}

void CState_Weaver_Attack_05_End::Exit(void* pArg)
{
	m_bHit = false;
	m_fAnimSpeed = 2.f;
	m_pTargetModel->Set_ModelEasingRatio(0.1f);
	m_pColCom->Set_Scale(XMLoadFloat4(&m_vColScale));
}

_bool CState_Weaver_Attack_05_End::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

void CState_Weaver_Attack_05_End::Excute_Particles(_uint _iEffectNum0, char* _szBoneName)
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


CState_Weaver_Attack_05_End* CState_Weaver_Attack_05_End::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Weaver_Attack_05_End* pInstance = new CState_Weaver_Attack_05_End(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Weaver_Attack_05_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Weaver_Attack_05_End::Clone(void* pArg)
{
	CState_Weaver_Attack_05_End* pInstance = new CState_Weaver_Attack_05_End(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Weaver_Attack_05_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Weaver_Attack_05_End::Free()
{
	__super::Free();
}
