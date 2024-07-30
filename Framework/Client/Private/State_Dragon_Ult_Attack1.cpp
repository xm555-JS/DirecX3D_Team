#include "stdafx.h"
#include "..\Public\State_Dragon_Ult_Attack1.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Player.h"
#include "Channel.h"
#include "Enemy_Boss_Dragon.h"
#include "Bullet_Normal.h"
#include "HIerarchyNode.h"

#include "Imgui_Manager.h"

CState_Dragon_Ult_Attack1::CState_Dragon_Ult_Attack1(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Dragon_Ult_Attack1::CState_Dragon_Ult_Attack1(const CState_Dragon_Ult_Attack1& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Dragon_Ult_Attack1::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_DRAGON_DEAD);

	m_ReserveStates.push_back(STATE_BOSS_DRAGON_FALL_LAND);

	m_ReserveProbabilitys[STATE_BOSS_DRAGON_FALL_LAND] = 1;

	return S_OK;
}

HRESULT CState_Dragon_Ult_Attack1::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Dragon_Ult_Attack1::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fAnimSpeed = 1.f;

	_float4 vPos;
	XMStoreFloat4(&vPos, m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION));
	
	m_fGoalY = vPos.y - 100.f;
	m_fGoalY += 4.f;
	m_fDamage = fRandom(900.f, 1100.f);

	m_pColCom = ((CCollider*)m_pOwner->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK)));
	m_bChangePhase = true;
}

void CState_Dragon_Ult_Attack1::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	Hit_Unit(m_pColCom, ELEMENTAL_LIGHTNING, m_fDamage, 655, 800, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit);

	if (303.f < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame() && !m_bAnimSkip)
	{
		m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Set_KeyFrame(586);
		m_bAnimSkip = true;
	}

	if (35 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame()
		&& 590 > m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
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

	if (m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_IsFinished())
		m_bEndCurState = true;

	//상승
	if (28 > m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame()
		&& 10 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_fAnimSpeed = 1.2f;
		_float fTickperSec = fTimeDelta * m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_TickperSecond();	//1프레임당 시간

		m_pOwner->Get_Transform()->Go_Up(fTickperSec, 4.f / 18.f);

		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySound(TEXT("Fly.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, 1.f));
	}

	//선회
	if (590 > m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame()
		&& 35 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_fAnimSpeed = 1.2f;
	}

	//Look
	if (620 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame()
		&& 655 > m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_fAnimSpeed = 1.f;

		_float fTickperSec = fTimeDelta * m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_TickperSecond();	//1프레임당 시간
		m_fEasingTime += fTickperSec;

		XMStoreFloat4(&m_vTargetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

		//_vector vCurLook = m_pOwnerTranformCom->Get_State(CTransform::STATE_LOOK);
		_vector vLook = XMLoadFloat4(&m_vTargetPos) - m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION);
		m_vDistance = XMVectorGetX(XMVector3Length(vLook)) + 1.5f;

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
	if (670 < m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame()
		&& 690 >= m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		m_fAnimSpeed = 1.2f;
		m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Set_RootBoneName("ROOT");

		_float fTickperSec = fTimeDelta * m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Get_TickperSecond();

		m_pOwnerTranformCom->Chase(XMLoadFloat4(&m_vTargetPos), fTickperSec, m_vDistance / 20.f);
	}

	STATE_TYPE	eNextState = CState::Ready_NextState();
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

	//하강이전
	if (670 > m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->GetChannel()[0]->Get_NumCurrentKeyFrame())
	{
		CHierarchyNode* pHierarchyNode = m_pTargetModel->Find_HierarcyNode("ROOT");
		XMStoreFloat4x4(&m_RootBoneMatrix, pHierarchyNode->Get_RealCombinedMatrix() * XMLoadFloat4x4(&m_pTargetModel->Get_TransformMatrix()));

		m_pOwner->Get_Transform()->Set_RootBoneMatrix(m_RootBoneMatrix);
	}
}

void CState_Dragon_Ult_Attack1::Exit(void* pArg)
{
	m_pOwner->Get_Transform();
	m_fAnimSpeed = 1.f;
	m_bHit = false;
	m_bAnimSkip = false;
	m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)]->Set_RootBoneName("");
}

_bool CState_Dragon_Ult_Attack1::Check_Condition(CState_Handler* pHandler)
{
	USEGAMEINSTANCE;

	CEnemy_Boss_FrostDragon* pOwner = (CEnemy_Boss_FrostDragon*)pHandler->Get_Owner();

	m_pColCom = ((CCollider*)pOwner->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACKRANGE)));

	if (pOwner->Get_UnitCurInfo()->fHP < (pOwner->Get_UnitInfo()->fHP) * 0.7f
		&& m_pColCom->Get_StayColliders()->size())
	{
		m_bChangePhase = true;
		return true;
	}

	return false;
}

CState_Dragon_Ult_Attack1* CState_Dragon_Ult_Attack1::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Dragon_Ult_Attack1* pInstance = new CState_Dragon_Ult_Attack1(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Dragon_Ult_Attack1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Dragon_Ult_Attack1::Clone(void* pArg)
{
	CState_Dragon_Ult_Attack1* pInstance = new CState_Dragon_Ult_Attack1(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Dragon_Ult_Attack1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Dragon_Ult_Attack1::Free()
{
	__super::Free();
}
