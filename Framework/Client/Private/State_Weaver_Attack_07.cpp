#include "stdafx.h"
#include "..\Public\State_Weaver_Attack_07.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

#include "Effect_Manager.h"
#include "Imgui_Manager.h"
#include "HIerarchyNode.h"

CState_Weaver_Attack_07::CState_Weaver_Attack_07(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Weaver_Attack_07::CState_Weaver_Attack_07(const CState_Weaver_Attack_07& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Weaver_Attack_07::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_WEAVER_WEAK_BEGIN);
	m_ForceStates.push_back(STATE_BOSS_WEAVER_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_WEAVER_IDLE);

	m_ReserveProbabilitys[STATE_BOSS_WEAVER_IDLE] = 1;

	return S_OK;
}

HRESULT CState_Weaver_Attack_07::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Weaver_Attack_07::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAccTime = 0.f;

	m_fDamage = fRandom(150.f, 250.f);
	m_pColCom = ((CCollider*)m_pOwner->Get_Component(ID_UINT(COLLIDER, COLLIDER_OBB, COL_ENEMY_ATTACK)));
	XMStoreFloat4(&m_vColScale, m_pColCom->Get_Scale());

	XMStoreFloat4(&m_vStartLook, m_pState_Handler->Get_Owner()->Get_Transform()->Get_State(CTransform::STATE_LOOK));
}

void CState_Weaver_Attack_07::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	for(_uint i = 0; i < 21; ++i)
		Hit_Unit(m_pColCom, ELEMENTAL_ICE, fRandom(150.f, 250.f), 175 + i * 10, 184 + i * 10, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[i]);

	m_fTimeAcc_Effect0 += fTimeDelta;
	m_fTimeAcc_Effect1 += fTimeDelta;


	m_fTimeAcc_Effect2 += fTimeDelta;
	if (0.02f < m_fTimeAcc_Effect2)
	{
		m_fTimeAcc_Effect2 = 0.f;
		Excute_Particles(m_iEffectNum1, "Bone019");
		Excute_Particles(m_iEffectNum1, "Bone020");
		Excute_Particles(m_iEffectNum1, "Bone021");
		Excute_Particles(m_iEffectNum1, "Bone022");
		Excute_Particles(m_iEffectNum1, "Bone023");
		Excute_Particles(m_iEffectNum1, "Bone024");

		m_iEffectNum1 += 1;
		if (3 < m_iEffectNum1)
			m_iEffectNum1 = 0;
	}

	if (65 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_07_Begin.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));


		if (155 > Get_CurKeyFrame())
		{
			IMPL_ALARM(m_fSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_07_Loop_3.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.2f), 0.3f, fTimeDelta);
			IMPL_ALARM(m_fSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_07_Loop_4.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.2f), 0.5f, fTimeDelta);
		}
	}

	//155~400까지 레이저 6갈래 155에 바로생성
	if (155 < Get_CurKeyFrame() && 390 >= Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_07_Begin_Laser.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f));

		IMPL_ALARM(m_fSound[2], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_07_Loop_1.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.5f), 0.3f, fTimeDelta);
		IMPL_ALARM(m_fSound[3], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Attack_07_Loop_2.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.3f), 0.3f, fTimeDelta);

		IMPL_ONCE(m_bEffect1, EFFECT_POS("Boss_Weaver_Attack_02_KnifeAttack_Sphere", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 5.7f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 0.9f));
		
		IMPL_ONCE(m_bEffect0, EFFECT_EXCUTE("Boss_Weaver_Laser_A", XMMatrixScaling(1.2f, 1.2f, 1.2f) * XMMatrixTranslation(0.f, 2.8, 4.f), 1.f, m_pState_Handler->Get_Owner(), CEffect_Manager::FOLLOW_POS_RUL));

		if (0.2f < m_fTimeAcc_Effect0)
		{
			m_fTimeAcc_Effect0 = 0;
			if (m_bToggle_Effect)
			{
				EFFECT_POS("Boss_Weaver_Laser_C1_P", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 3.f, 4.f, 1.f));
			}
			else
			{
				EFFECT_POS("Boss_Weaver_Laser_C1_P", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
					XMVectorSet(0.f, 3.f, 4.f, 1.f),
					XMVectorSet(0.f, 0.f, XMConvertToRadians(-90.f), 0.f));
			}
			m_bToggle_Effect = !m_bToggle_Effect;

			EFFECT_POS("Boss_Weaver_Laser_C2_P", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, 3.f, 4.f, 1.f));
		}
		if (0.05f < m_fTimeAcc_Effect1)
		{
			m_fTimeAcc_Effect1 = 0.f;
			if (0 == m_iEffectNum0)
			{
				EFFECT_POS("Boss_Weaver_Laser_B1", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, fRandom(-1, 1) + 3.f, fRandom(-1, 1) + 6.5F, 1.f));
				EFFECT_POS("Boss_Weaver_Laser_B3", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, fRandom(-1, 1) + 3.f, fRandom(-1, 1) + 6.5F, 1.f));
			}
			else if (1 == m_iEffectNum0)
			{
				EFFECT_POS("Boss_Weaver_Laser_B2", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, fRandom(-1, 1) + 3.f, fRandom(-1, 1) + 6.5F, 1.f));
				EFFECT_POS("Boss_Weaver_Laser_B3", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, fRandom(-1, 1) + 3.f, fRandom(-1, 1) + 6.5F, 1.f));
			}
			else if (2 == m_iEffectNum0)
			{
				EFFECT_POS("Boss_Weaver_Laser_B1", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, fRandom(-1, 1) + 3.f, fRandom(-1, 1) + 6.5F, 1.f));
				EFFECT_POS("Boss_Weaver_Laser_B2", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(0.f, fRandom(-1, 1) + 3.f, fRandom(-1, 1) + 6.5F, 1.f));
			}
			m_iEffectNum0 += 1;
			if (2 < m_iEffectNum0)
				m_iEffectNum0 = 0;
		}

		_float4 vTargetPos;
		XMStoreFloat4(&vTargetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

		_vector vCurLook = m_pOwnerTranformCom->Get_State(CTransform::STATE_LOOK);
		_vector vLook = XMLoadFloat4(&vTargetPos) - m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION);

		_float	fCurTime = min(1.f, (Get_CurKeyFrame() - 155) / 400.f);

		_vector vFinalLook = XMVectorLerp(XMLoadFloat4(&m_vStartLook), vLook, fCurTime * fCurTime);
		_float fDgree = XMConvertToDegrees(XMVectorGetX(XMVector3AngleBetweenVectors(vCurLook, vLook)));
		
		if (0 > XMVectorGetY(XMVector3Cross(vCurLook, vLook)))
			m_pOwnerTranformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(fDgree  * (-fTimeDelta) * 0.5f));
		else
			m_pOwnerTranformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(fDgree * fTimeDelta * 0.5f));
	}

	CState_AI::Tick(fTimeDelta);
}

void CState_Weaver_Attack_07::Exit(void* pArg)
{
	RESET_ONCE(m_bEffect0);
	RESET_ONCE(m_bEffect1);

	m_fTimeAcc_Effect0 = 0.f;
	m_fTimeAcc_Effect1 = 0.f;
	m_fTimeAcc_Effect2 = 0.f;
	m_iEffectNum0 = 0.f;
	m_iEffectNum1 = 0.f;
	m_bToggle_Effect = false;
	for (_uint i = 0; i < 21; ++i)
		m_bHit[i] = false;
}

_bool CState_Weaver_Attack_07::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	return true;
}

void CState_Weaver_Attack_07::Excute_Particles(_uint _iEffectNum0, char* _szBoneName)
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

CState_Weaver_Attack_07* CState_Weaver_Attack_07::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Weaver_Attack_07* pInstance = new CState_Weaver_Attack_07(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Weaver_Attack_07");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Weaver_Attack_07::Clone(void* pArg)
{
	CState_Weaver_Attack_07* pInstance = new CState_Weaver_Attack_07(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Weaver_Attack_07");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Weaver_Attack_07::Free()
{
	__super::Free();
}
