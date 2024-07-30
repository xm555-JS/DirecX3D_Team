#include "stdafx.h"
#include "..\Public\State_Weaver_Idle.h"
#include "GameInstance.h"
#include "Model.h"

#include "Animation.h"
#include "Channel.h"
#include "Player.h"

#include "Unit.h"

#include "Easing_Utillity.h"

CState_Weaver_Idle::CState_Weaver_Idle(CState_Manager* pManager, STATE_TYPE eState)
	:CState_Boss(pManager, eState)
{
}

CState_Weaver_Idle::CState_Weaver_Idle(const CState_Weaver_Idle& rhs)
	: CState_Boss(rhs)
{
}

HRESULT CState_Weaver_Idle::Initialize_Prototype()
{
	m_ForceStates.push_back(STATE_BOSS_WEAVER_WEAK_BEGIN);
	m_ForceStates.push_back(STATE_BOSS_WEAVER_DEAD);

	m_ForceStates.push_back(STATE_BOSS_WEAVER_ATTACK_10);				//������ �Ѿ


	m_ForceStates.push_back(STATE_BOSS_WEAVER_TURN_L);
	m_ForceStates.push_back(STATE_BOSS_WEAVER_TURN_R);

	//m_ForceStates.push_back(STATE_BOSS_WEAVER_WALK_B);
	//m_ForceStates.push_back(STATE_BOSS_WEAVER_WALK_F);

	//�⺻���ݷ� ����
	m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//�뽬
	m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_01);			//��Ÿ - �¿�� �ѹ��� ����
	
	m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//�뽬
	m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_09);			//�����
	
	m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//�뽬
	m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_02);			//Į���� - Į�� �Դٸ� ���ٸ�
	
	m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//�뽬
	m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_03);			//Į �߻�1 �ֹ�ó�� ���� �����Ÿ��� �׳� ���������� �������� - Į�� �޸� �ݶ��̴��� ����	//Į �߻�2 ���ݸ�� �����⸸ �ϰ� ����Ʈ�� ó���� ���� - �Ҹ� ��ü�� �� ����. ����Ʈ���ϰ� ū�ݶ��̴��ε� ����.
	
	m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//�뽬
	m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_07);			//������

	//m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_11_BEGIN);		//������ε� ���� ��� �𸣰���
	//m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_12);

	return S_OK;
}

HRESULT CState_Weaver_Idle::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Weaver_Idle::Enter(void* pArg)
{
	__super::Enter(pArg);
	m_fEasingTime = 2.f;
	m_fAccTime = 0.f;
	m_fStateTime = fRandom(1.f, 1.5f);
	m_fAnimSpeed = 1.f;

	//2������
	if (((CUnit*)m_pOwner)->Get_UnitInfo()->fHP * 0.7f > ((CUnit*)m_pOwner)->Get_UnitCurInfo()->fHP
		&& STATE_BOSS_WEAVER_ATTACK_05_1 != m_pState_Handler->Get_PreStateType()
		&& 0 == m_iPhase)
	{
		m_ReserveStates.clear();

		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//�뽬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_08);

		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//�뽬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_04);			//�㸮���� }

		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//�뽬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_02);			//Į���� - Į�� �Դٸ� ���ٸ�

		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//�뽬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_03);			//Į �߻�1 �ֹ�ó�� ���� �����Ÿ��� �׳� ���������� �������� - Į�� �޸� �ݶ��̴��� ����	//Į �߻�2 ���ݸ�� �����⸸ �ϰ� ����Ʈ�� ó���� ���� - �Ҹ� ��ü�� �� ����. ����Ʈ���ϰ� ū�ݶ��̴��ε� ����.

		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//�뽬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_07);			//������

		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//�뽬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_01);			//��Ÿ - �¿�� �ѹ��� ����

		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//�뽬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_09);			//�����

		m_iPhase = 1;
	}

	if (((CUnit*)m_pOwner)->Get_UnitInfo()->fHP * 0.4f > ((CUnit*)m_pOwner)->Get_UnitCurInfo()->fHP
		&& STATE_BOSS_WEAVER_ATTACK_05_1 != m_pState_Handler->Get_PreStateType()
		&& 1 == m_iPhase)
	{
		m_ReserveStates.clear();

		//3������ �߰�//ü�� 40���� ����
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//�뽬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_06_BEGIN);	//���� â }

		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//�뽬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_08);

		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//�뽬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_07);			//������

		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//�뽬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_03);			//Į �߻�1 �ֹ�ó�� ���� �����Ÿ��� �׳� ���������� �������� - Į�� �޸� �ݶ��̴��� ����	//Į �߻�2 ���ݸ�� �����⸸ �ϰ� ����Ʈ�� ó���� ���� - �Ҹ� ��ü�� �� ����. ����Ʈ���ϰ� ū�ݶ��̴��ε� ����.

		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//�뽬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_04);			//�㸮���� }

		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//�뽬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_01);			//��Ÿ - �¿�� �ѹ��� ����

		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//�뽬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_09);			//�����

		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_05_1);		//�뽬
		m_ReserveStates.push_back(STATE_BOSS_WEAVER_ATTACK_02);			//Į���� - Į�� �Դٸ� ���ٸ�

		m_iPhase = 2;
	}

	XMStoreFloat4(&m_vStartLook, m_pOwnerTranformCom->Get_State(CTransform::STATE_LOOK));
}

void CState_Weaver_Idle::Tick(_float fTimeDelta)
{
	USEGAMEINSTANCE;

	m_fAccTime += fTimeDelta;

	_float4 vTargetPos;
	XMStoreFloat4(&vTargetPos, m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vCurLook = m_pOwnerTranformCom->Get_State(CTransform::STATE_LOOK);
	_vector vLook = XMLoadFloat4(&vTargetPos) - m_pOwnerTranformCom->Get_State(CTransform::STATE_TRANSLATION);

	_float	fCurTime = min(1.f, m_fAccTime / m_fEasingTime);

	//vCurLook
	_vector vFinalLook = XMVectorLerp(XMLoadFloat4(&m_vStartLook), vLook, fCurTime);

	m_pOwnerTranformCom->YFixedLook(vFinalLook);

	if (m_fStateTime > m_fAccTime)
		m_bEndCurState = true;

	CState_Boss::Tick(fTimeDelta);
}

void CState_Weaver_Idle::Exit(void* pArg)
{
}

_bool CState_Weaver_Idle::Check_Condition(CState_Handler* pHandler)
{
	return true;
}

CState_Weaver_Idle* CState_Weaver_Idle::Create(CState_Manager* pManager, STATE_TYPE eState)
{
	CState_Weaver_Idle* pInstance = new CState_Weaver_Idle(pManager, eState);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Weaver_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Weaver_Idle::Clone(void* pArg)
{
	CState_Weaver_Idle* pInstance = new CState_Weaver_Idle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Weaver_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Weaver_Idle::Free()
{
	__super::Free();
}
