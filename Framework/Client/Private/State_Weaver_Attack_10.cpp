#include "stdafx.h"
#include "..\Public\State_Weaver_Attack_10.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"
#include "Enemy_Boss_Weaver.h"

#include "Unit.h"

#include "Easing_Utillity.h"
#include "Effect_Base.h"
#include "HIerarchyNode.h"

#include "Imgui_Manager.h"

CState_Weaver_Attack_10::CState_Weaver_Attack_10(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Weaver_Attack_10::CState_Weaver_Attack_10(const CState_Weaver_Attack_10& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Weaver_Attack_10::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_WEAVER_WEAK_BEGIN);
	m_ForceStates.push_back(STATE_BOSS_WEAVER_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_WEAVER_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_WEAVER_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Weaver_Attack_10::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Weaver_Attack_10::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAccTime = 0.f;

	EFFECT_POS("Boss_Weaver_Attack_10_PhaseChange", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 0.8f, -0.1f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 0.8f);
}

void CState_Weaver_Attack_10::Tick(_float fTimeDelta)
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

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	if (0 == ((CEnemy_Boss_Weaver*)m_pOwner)->Get_Phase())
		((CEnemy_Boss_Weaver*)m_pOwner)->Set_Phase(1);
	else if (1 == ((CEnemy_Boss_Weaver*)m_pOwner)->Get_Phase())
		((CEnemy_Boss_Weaver*)m_pOwner)->Set_Phase(2);

	CState_AI::Tick(fTimeDelta);
}

void CState_Weaver_Attack_10::Exit(void* pArg)
{
	m_fTimeAcc_Effect0 = 0;
	m_iEffectNum0 = 0;
}

void CState_Weaver_Attack_10::Excute_Particles(_uint _iEffectNum0, char* _szBoneName)
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

_bool CState_Weaver_Attack_10::Check_Condition(CState_Handler* pHandler)
{
	CEnemy_Boss_Weaver* pOwner = ((CEnemy_Boss_Weaver*)pHandler->Get_Owner());
	if ((pOwner->Get_UnitInfo()->fHP) * 0.7f > pOwner->Get_UnitCurInfo()->fHP && 0 == pOwner->Get_Phase())
		return true;
	if ((pOwner->Get_UnitInfo()->fHP) * 0.4f > pOwner->Get_UnitCurInfo()->fHP && 1 == pOwner->Get_Phase())
		return true;

	return false;
}

CState_Weaver_Attack_10* CState_Weaver_Attack_10::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Weaver_Attack_10* pInstance = new CState_Weaver_Attack_10(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Weaver_Attack_10");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Weaver_Attack_10::Clone(void* pArg)
{
	CState_Weaver_Attack_10* pInstance = new CState_Weaver_Attack_10(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Weaver_Attack_10");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Weaver_Attack_10::Free()
{
	__super::Free();
}
