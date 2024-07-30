#include "stdafx.h"
#include "..\Public\State_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Ing.h"
#include "GameInstance.h"
#include "Enemy.h"
#include "Enemy_Arrow.h"
#include "Effect_Manager.h"

CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Ing::CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Ing(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Ing::CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Ing(const CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Ing& rhs)
	: CState_Enemy_Normal(rhs)
{
}

HRESULT CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Ing::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_DEAD_END);


	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_B);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_F);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_L);
	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_R);

	m_ReserveStates.push_back(STATE_ENEMY_MECHANICALCITY_GUNNER_ATTACK_SERIESSHOT_END);



	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_B] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_F] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_L] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_DAMAGE_R] = 1;


	m_ReserveProbabilitys[STATE_ENEMY_MECHANICALCITY_GUNNER_ATTACK_SERIESSHOT_END] = 1;


	m_fAttackLength_Min = 2.5f;
	m_fAttackLength_Max = 10.f;

	return S_OK;
}

HRESULT CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Ing::Initialize(void * pArg)
{
	return S_OK;
}

void CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Ing::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fAnimSpeed = 1.f;

	m_bOneShot = true;

}

void CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Ing::Tick(_float fTimeDelta)
{
	if (Is_Damaged())
		m_bEndCurState = true;

	if (Is_AnimFinished())
		m_bEndCurState = true;

	if (m_bOneShot)
	{
		if (0 < Get_CurrentKeyFrame())
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
			BulletDesc.fBulletSpeed = 60.f;
			BulletDesc.fBulletDeadTime = 5.f;
			pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_BULLET_ARROW), ID_UINT(ENEMY, ENEMY_BULLET, ENEMY_BULLET_ARROW), &BulletDesc);

			m_bOneShot = false;
		}
	}

	if (0 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect0, EFFECT_POS("Gunner_Shot", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 1.3f, 0.2f, 1.f),
			XMVectorSet(1.7f, 0.f, 0.f, 0.f), 1.f,
			XMVectorSet(0.3f, 0.3f, 0.3f, 0.f)));

		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Gunner_Shot.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.1f));
	}
		
	m_pOwnerTranformCom->YFixedLookAt(m_pState_Handler->Get_Player()->Get_Pos());

	CState_AI::Tick(fTimeDelta);
}

void CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Ing::Exit(void* pArg)
{
	for (int i = 0; i < HIT_END; i++)
		m_bHit[i] = false;

	RESET_ONCE(m_bEffect0);
}

_bool CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Ing::Check_Condition(CState_Handler* pHandler)
{
	if (Is_Damaged(pHandler))
		return false;

	return true;
}


CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Ing* CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Ing::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Ing* pInstance = new CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Ing(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Ing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Ing::Clone(void* pArg)
{
	CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Ing* pInstance = new CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Ing(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Ing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_MechanicalCity_Gunner_Attack_SeriesShot_Ing::Free()
{
	__super::Free();
}
