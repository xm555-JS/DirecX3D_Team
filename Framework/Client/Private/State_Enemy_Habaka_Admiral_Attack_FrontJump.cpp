#include "stdafx.h"
#include "..\Public\State_Enemy_Habaka_Admiral_Attack_FrontJump.h"
#include "GameInstance.h"
#include "Enemy.h"
#include "Camera.h"
CState_Enemy_Habaka_Admiral_Attack_FrontJump::CState_Enemy_Habaka_Admiral_Attack_FrontJump(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Enemy_Normal(pManager, eState)
{
}

CState_Enemy_Habaka_Admiral_Attack_FrontJump::CState_Enemy_Habaka_Admiral_Attack_FrontJump(const CState_Enemy_Habaka_Admiral_Attack_FrontJump& rhs)
	: CState_Enemy_Normal(rhs)
{
	m_fAttackLength_Min = rhs.m_fAttackLength_Min;
	m_fAttackLength_Max = rhs.m_fAttackLength_Max;
}

HRESULT CState_Enemy_Habaka_Admiral_Attack_FrontJump::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DEAD_1);


	//m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_B);
	//m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_L);
	//m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_R);
	//m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_F);

	m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_IDLE_BATTLE);
	m_ReserveStates.push_back(STATE_ENEMY_HABAKA_ADMIRAL_IDLE_NORMAL);

	//m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_B] = 1;
	//m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_L] = 1;
	//m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_R] = 1;
	//m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_DAMAGE_F] = 1;

	m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_IDLE_BATTLE] = 1;
	m_ReserveProbabilitys[STATE_ENEMY_HABAKA_ADMIRAL_IDLE_NORMAL] = 1;

	m_fAttackLength_Min = 5.f;
	m_fAttackLength_Max = 7.5f;

	return S_OK;
}

HRESULT CState_Enemy_Habaka_Admiral_Attack_FrontJump::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Enemy_Habaka_Admiral_Attack_FrontJump::Enter(void* pArg)
{
	__super::Enter(pArg);

	m_fAnimSpeed = 1.f;

	m_strRootBoneName = "Root_bone";

	m_fDamage = fRandom(800.f, 1000.f);

	m_pColCom[COLLIDER_OWNER] = (CCollider*)((CEnemy*)m_pState_Handler->Get_Owner())->Get_Component(ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK));
	m_vStartColScale = m_pColCom[COLLIDER_OWNER]->Get_Scale();
	m_pNaviCom = (CNavigation*)m_pOwner->Get_Component(ID_UINT(NAVIGATION));
}

void CState_Enemy_Habaka_Admiral_Attack_FrontJump::Tick(_float fTimeDelta)
{
	if (50 < Get_CurrentKeyFrame())
		m_pColCom[COLLIDER_OWNER]->Set_Scale(XMVectorSet(10.f, 10.f, 10.f, 1.f));
	if (75 < Get_CurrentKeyFrame())
		m_pColCom[COLLIDER_OWNER]->Set_Scale(m_vStartColScale);



	Hit_Unit(m_pColCom[COLLIDER_OWNER], ELEMENTAL_MELEE, m_fDamage, 51, 75, m_pTargetModel->Get_Animation()[m_pState_Handler->Get_AnimationIdx(m_eState)], &m_bHit[HIT_1]);


	if (Is_AnimFinished())
		m_bEndCurState = true;

	if (0 < Get_CurKeyFrame())
	{
		IMPL_ONCE(m_bEffect0, EFFECT_POS("TimeIllusion_Ready", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			//XMVectorSet(0.f, 0.f, 2.f, 1.f),
			-XMVector3Normalize(GAMEINSTANCE->Get_CurrentCamera()->Get_Transform()->Get_WorldMatrix().r[2]),
			XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f,
			XMVectorSet(1.5f, 1.5f, 1.5f, 0.f)));
	}

	if (0 < Get_CurKeyFrame())
		IMPL_ONCE(m_bSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Habaka_Jump.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT,m_pOwner->Get_Pos(), 0.1f));

	if (43 < Get_CurKeyFrame())
	{
		// ���� ȸ�� ������
		IMPL_ONCE(m_bEffect1, EFFECT_POS("Habaka_Jump_Attack", m_pState_Handler->Get_Owner()->Get_WorldMatrix(),
			XMVectorSet(0.f, 0.f, -7.f, 1.f),
			XMVectorSet(0.f, -9.4f, 0.f, 0.f), 1.f,
			XMVectorSet(1.f, 1.f, 1.f, 0.f)));
	}

	if (50 < Get_CurKeyFrame())
		IMPL_ONCE(m_bSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Habaka_Chop.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, m_pOwner->Get_Pos(), 0.1f));

	CState_Enemy_Normal::Tick(fTimeDelta);

}

void CState_Enemy_Habaka_Admiral_Attack_FrontJump::Exit(void* pArg)
{
	for (int i = 0; i < HIT_END; i++)
		m_bHit[i] = false;

	RESET_ONCE(m_bEffect0);
	RESET_ONCE(m_bEffect1);
}

_bool CState_Enemy_Habaka_Admiral_Attack_FrontJump::Check_Condition(CState_Handler* pHandler)
{
	_vector vPlayerPos = pHandler->Get_Player()->Get_Pos();
	_vector vOwnerPos = pHandler->Get_Owner()->Get_Pos();

	_float fLength = XMVectorGetX((XMVector3Length(vPlayerPos - vOwnerPos)));

	if (m_fAttackLength_Max > fLength && m_fAttackLength_Min < fLength)//�÷��̾���� �Ÿ��� m_fAttackLength_Max�۰� m_fAttackLength_Min���� ũ�� ���ü��ִ�
		return true;

	return false;
}


CState_Enemy_Habaka_Admiral_Attack_FrontJump* CState_Enemy_Habaka_Admiral_Attack_FrontJump::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Enemy_Habaka_Admiral_Attack_FrontJump* pInstance = new CState_Enemy_Habaka_Admiral_Attack_FrontJump(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Enemy_Habaka_Admiral_Attack_FrontJump");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Enemy_Habaka_Admiral_Attack_FrontJump::Clone(void* pArg)
{
	CState_Enemy_Habaka_Admiral_Attack_FrontJump* pInstance = new CState_Enemy_Habaka_Admiral_Attack_FrontJump(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Enemy_Habaka_Admiral_Attack_FrontJump");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Enemy_Habaka_Admiral_Attack_FrontJump::Free()
{
	__super::Free();
}
