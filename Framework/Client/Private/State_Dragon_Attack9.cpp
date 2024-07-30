#include "stdafx.h"
#include "..\Public\State_Dragon_Attack9.h"
#include "GameInstance.h"

#include "Channel.h"
#include "Model.h"
#include "Animation.h"
#include "HIerarchyNode.h"

#include "Enemy_Boss_Dragon.h"
#include "Bullet_Normal.h"
#include "Player.h"

#include "Imgui_Manager.h"

CState_Dragon_Attack9::CState_Dragon_Attack9(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Dragon_Attack9::CState_Dragon_Attack9(const CState_Dragon_Attack9& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Dragon_Attack9::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_DRAGON_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_DRAGON_ATTACK_05);

	m_ReserveProbabilitys[STATE_BOSS_DRAGON_ATTACK_05] = 1;

	return S_OK;
}

HRESULT CState_Dragon_Attack9::Initialize(void* pArg)
{
	m_bOnceSkill = true;
	return S_OK;
}

void CState_Dragon_Attack9::Enter(void* pArg)
{
	__super::Enter(pArg);

	_float4 vPos;
	XMStoreFloat4(&vPos, m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION));

	m_fGoalY = vPos.y - 152.f;
	m_fDamage = fRandom(810.f, 990.f);

	XMStoreFloat4(&m_vStartLook, m_pOwnerTranformCom->Get_State(CTransform::STATE_LOOK));
	m_pColCom = ((CCollider*)m_pOwner->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK)));
	m_bChangePhase = true;
}

void CState_Dragon_Attack9::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, m_fDamage, 655, 686, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit);

	if(303.f < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame() && !m_bAnimSkip)
	{
		m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Set_KeyFrame(586);
		m_bAnimSkip = true;
	}

	if (35 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame()
		&& 290 > m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_fAccTime += fTimeDelta;

		if (0.3f < m_fAccTime)
		{
			for (_uint i = 0; i < 1; ++i)
			{
				CBullet_Normal::BULLETDESC BulletDesc;
				BulletDesc.pTarget = m_pTarget;

				_float fRanSpeed = fRandom(5.f, 10.f);
				BulletDesc.fBulletSpeed = fRanSpeed;
				_matrix		RotationMatrix = XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(i * 10.f));
				_matrix		WorldMatrix = RotationMatrix * m_pOwnerTranformCom->Get_WorldMatrix();

				WorldMatrix.r[0] = XMVector3Normalize(WorldMatrix.r[0]);
				WorldMatrix.r[1] = XMVector3Normalize(WorldMatrix.r[1]);
				WorldMatrix.r[2] = XMVector3Normalize(WorldMatrix.r[2]);
				WorldMatrix.r[3] += XMVector3Normalize(WorldMatrix.r[2]);

				XMStoreFloat4x4(&BulletDesc.WorldMatrix, WorldMatrix);
				pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_BULLET_FROSTDRAGON), ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_BULLET_FROSTDRAGON), &BulletDesc);
			
				IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySound(TEXT("Missile.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, 0.2f));
				RESET_ONCE(m_bSound[0]);
			}

			m_fAccTime = 0.f;
		}
	}

	//Look
	if (620 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame()
		&& 655 > m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_fAnimSpeed = 1.f;

		_float fTickperSec = fTimeDelta * m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_TickperSecond();	//1프레임당 시간
		m_fEasingTime += fTickperSec;


		_float4 vTargetPos;
		XMStoreFloat4(&vTargetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

		//_vector vCurLook = m_pOwnerTranformCom->Get_State(CTransform::STATE_LOOK);
		_vector vLook = XMLoadFloat4(&vTargetPos) - m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION);

		_float	fCurTime = min(1.f, m_fEasingTime / 34.f);

		if (fCurTime == 1.f)
		{
			int a = 0;
		}

		//vCurLook
		_vector vFinalLook = XMVectorLerp(XMLoadFloat4(&m_vStartLook), vLook, fCurTime);

		m_pOwnerTranformCom->YFixedLook(vFinalLook);
	}

	//하강
	if (613 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame()
		&& 655 > m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_fAnimSpeed = 1.f;
		_float fTickperSec = fTimeDelta * m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_TickperSecond();	//1프레임당 시간

		m_pOwner->Get_Transform()->Go_Down(fTickperSec, m_fGoalY / 42.f);
	}

	//684 ~ 5 까지 떨어짐//670까지 올라감
	if (686 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_bEndCurState = true;
	}

	STATE_TYPE	eNextState = CState_AI::Ready_NextState();
	if (STATE_END != eNextState)
	{
		m_pOwnerTranformCom->Reset_RootBoneDiff();

		m_pState_Handler->Change_State(eNextState);

		m_pTargetModel->Play_Animation(fTimeDelta * m_fAnimSpeed, "ROOT", &m_RootBoneMatrix);
		m_pTargetModel->Set_ModelEasingRatio(0.2f);

		m_pTargetModel->Set_RootEasing(true);

	}
	else
	{
		m_pTargetModel->Play_Animation(fTimeDelta * m_fAnimSpeed, "ROOT", &m_RootBoneMatrix);
	}

	CHierarchyNode* pHierarchyNode = m_pTargetModel->Find_HierarcyNode("ROOT");

	XMStoreFloat4x4(&m_RootBoneMatrix, pHierarchyNode->Get_RealCombinedMatrix() * XMLoadFloat4x4(&m_pTargetModel->Get_TransformMatrix()));

	m_pOwner->Get_Transform()->Set_RootBoneMatrix(m_RootBoneMatrix);

	if (660 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[4], MGR(CSound_Manager)->PlaySound(TEXT("Earthquake.mp3"), CSound_Manager::CHANNEL_ENEMY_EFFECT, 1.f));
	}

	if (670 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[3], MGR(CSound_Manager)->PlaySound(TEXT("Roar.wav"), CSound_Manager::CHANNEL_ENEMY, 1.f));
	}


	if (590 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bSound[2], MGR(CSound_Manager)->PlaySound(TEXT("Fly.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, 1.f));
	}
}

void CState_Dragon_Attack9::Exit(void* pArg)
{
	m_pOwner->Get_Transform();
	m_fAnimSpeed = 1.f;
	m_fEasingTime = 0.f;
	m_bHit = false;
	m_bAnimSkip = false;

	m_pTargetModel->Set_RootEasing(false);
}

_bool CState_Dragon_Attack9::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;
	return true;
}

CState_Dragon_Attack9* CState_Dragon_Attack9::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Dragon_Attack9* pInstance = new CState_Dragon_Attack9(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Dragon_Attack9");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Dragon_Attack9::Clone(void* pArg)
{
	CState_Dragon_Attack9* pInstance = new CState_Dragon_Attack9(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Dragon_Attack9");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Dragon_Attack9::Free()
{
	__super::Free();
}
