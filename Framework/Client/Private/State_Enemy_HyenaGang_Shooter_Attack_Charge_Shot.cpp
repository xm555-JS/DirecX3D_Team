#include "stdafx.h"
#include "..\Public\State_Enemy_HyenaGang_Shooter_Attack_Charge_Shot.h"
#include "GameInstance.h"
#include "Enemy.h"
#include "HIerarchyNode.h"
#include "Enemy_Arrow.h"

CState_Enemy_HyenaGang_Shooter_Attack_Charge_Shot::CState_Enemy_HyenaGang_Shooter_Attack_Charge_Shot(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_HyenaGang_Shooter_Attack_Charge_Shot::CState_Enemy_HyenaGang_Shooter_Attack_Charge_Shot(const CState_Enemy_HyenaGang_Shooter_Attack_Charge_Shot& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_HyenaGang_Shooter_Attack_Charge_Shot::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_ENEMY_HYENAGANG_SHOOTER_DEAD_1);

	m_ReserveStates.push_back(STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_B);
	m_ReserveStates.push_back(STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_F);
	m_ReserveStates.push_back(STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_L);
	m_ReserveStates.push_back(STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_R);

	m_ReserveStates.push_back(STATE_ENEMY_HYENAGANG_SHOOTER_IDLE_BATTLE);
	m_ReserveStates.push_back(STATE_ENEMY_HYENAGANG_SHOOTER_IDLE_NORMAL);


	m_ReserveProbabilitys[STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_B] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_F] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_L] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_HYENAGANG_SHOOTER_DAMAGE_R] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_HYENAGANG_SHOOTER_IDLE_BATTLE] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_HYENAGANG_SHOOTER_IDLE_NORMAL] = 1;


	m_fAttackLength_Min = 2.5f;
	m_fAttackLength_Max = 10.f;


	return S_OK;
}

HRESULT CState_Enemy_HyenaGang_Shooter_Attack_Charge_Shot::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Enemy_HyenaGang_Shooter_Attack_Charge_Shot::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fAnimSpeed = 1.f;
	m_strRootBoneName = "Root-bone";

	m_bOneShot = true;

}

void CState_Enemy_HyenaGang_Shooter_Attack_Charge_Shot::Tick(_float fTimeDelta)
{

	if (Is_Damaged())
		m_bEndCurState = true;

	if (Is_AnimFinished())
		m_bEndCurState = true;

	if (m_bOneShot)
	{
		if (77 < Get_CurKeyFrame())
			IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Gang_Shot.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.1f));

		if (81 < Get_CurrentKeyFrame())
		{
			_vector vLookAt = m_pState_Handler->Get_Player()->Get_Pos();
			vLookAt = XMVectorSetY(vLookAt, XMVectorGetY(vLookAt) + 0.5f);
			_vector vPos = m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION);
			_vector vDir = m_pOwnerTranformCom->Get_State(CTransform::STATE_LOOK);

			USEGAMEINSTANCE;
			CEnemy_Arrow::BULLETDESC BulletDesc;
			XMStoreFloat4(&BulletDesc.vLookAt, vLookAt);
			vPos = vPos + vDir * 1.f;
			vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + 0.5f);
			XMStoreFloat4(&BulletDesc.vStartPos, vPos);
			//BulletDesc.fBulletSpeed = DEBUGFLOAT4(0).x;
			BulletDesc.fBulletSpeed = 15.f;
			BulletDesc.fBulletDeadTime = 5.f;
			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_BULLET_ARROW), ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_BULLET_ARROW), &BulletDesc);

			m_bOneShot = false;
		}
	}
	m_pOwnerTranformCom->YFixedLookAt(m_pState_Handler->Get_Player()->Get_Pos());


	CState_Enemy_Normal::Tick(fTimeDelta);

}

void CState_Enemy_HyenaGang_Shooter_Attack_Charge_Shot::Exit(void* pArg)
{

}

_bool CState_Enemy_HyenaGang_Shooter_Attack_Charge_Shot::Check_Condition(CState_Handler* pHandler)
{
	if (Is_Damaged(pHandler))
		return false;

	_vector vPlayerPos = pHandler->Get_Player()->Get_Pos();
	_vector vOwnerPos = pHandler->Get_Owner()->Get_Pos();

	_float fLength = XMVectorGetX((XMVector3Length(vPlayerPos - vOwnerPos)));

	if (m_fAttackLength_Min < fLength && m_fAttackLength_Max > fLength)
		return true;

	return false;
}


CState_Enemy_HyenaGang_Shooter_Attack_Charge_Shot* CState_Enemy_HyenaGang_Shooter_Attack_Charge_Shot::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_HyenaGang_Shooter_Attack_Charge_Shot* pInstance = new CState_Enemy_HyenaGang_Shooter_Attack_Charge_Shot(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_HyenaGang_Shooter_Attack_Charge_Shot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_HyenaGang_Shooter_Attack_Charge_Shot::Clone(void* pArg)
{
	CState_Enemy_HyenaGang_Shooter_Attack_Charge_Shot* pInstance = new CState_Enemy_HyenaGang_Shooter_Attack_Charge_Shot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_HyenaGang_Shooter_Attack_Charge_Shot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_HyenaGang_Shooter_Attack_Charge_Shot::Free()
{
	__super::Free();
}
