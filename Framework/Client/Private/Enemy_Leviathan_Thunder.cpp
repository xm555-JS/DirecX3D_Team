#include "stdafx.h"
#include "State_Handler.h"

#include "Unit.h"
#include "GameInstance.h"
#include "Collider.h"
#include "Sound_Manager.h"
#include "..\Public\Enemy_Leviathan_Thunder.h"

#include "Effect_Base.h"
#include "Imgui_Manager.h"

CEnemy_Leviathan_Thunder::CEnemy_Leviathan_Thunder()
	: CBullet_Normal()
{
}

CEnemy_Leviathan_Thunder::CEnemy_Leviathan_Thunder(const CBullet_Normal& rhs)
	: CBullet_Normal(rhs)
{
}

HRESULT CEnemy_Leviathan_Thunder::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemy_Leviathan_Thunder::Initialize(void* pArg)
{
	m_fFrustumRange = 3.f;

	if (FAILED(__super::Initialize(pArg)))
		__debugbreak();

	if (FAILED(SetUp_Components()))
		__debugbreak();

	if (nullptr != pArg)
	{
		memcpy(&m_BulletDesc, pArg, sizeof(BULLETDESC));
	}
	m_bIsPhaseChanged = (_bool)m_BulletDesc.TargetPos.w;

	m_vPos = m_BulletDesc.TargetPos;
	m_vPos.w = 1.f;
	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_ENEMY, this);
	pGameInstance->Add_LateTickGroup(LTG_ENEMY, this);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_vPos));

	m_fDamage = fRandom(640.f, 2040.f);

	EFFECT_POS(m_bIsPhaseChanged ? "Boss_Dragon_Thunder_Red_Mark" : "Boss_Dragon_Thunder_Blue_Mark", nullptr, XMVectorSet(m_vPos.x, m_vPos.y, m_vPos.z, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f, XMVectorSet(5.f, 5.f, 5.f, 0.f));

	RESET_ONCE(m_bSound0);

	return S_OK;
}

HRESULT CEnemy_Leviathan_Thunder::Initialize_Pooling(void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_BulletDesc, pArg, sizeof(BULLETDESC));
	}
	m_bIsPhaseChanged = (_bool)m_BulletDesc.TargetPos.w;

	m_vPos = m_BulletDesc.TargetPos;
	m_vPos.w = 1.f;
	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_ENEMY, this);
	pGameInstance->Add_LateTickGroup(LTG_ENEMY, this);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_vPos));

	m_pCol_Sphere->Clear_Colliders();

	m_fDamage = fRandom(640.f, 2040.f);

	m_pTransformCom->Reset_StraightAccTime();

	EFFECT_POS(m_bIsPhaseChanged ? "Boss_Dragon_Thunder_Red_Mark" : "Boss_Dragon_Thunder_Blue_Mark", nullptr, XMVectorSet(m_vPos.x, m_vPos.y, m_vPos.z, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f, XMVectorSet(5.f, 5.f, 5.f, 0.f));

	m_fFrustumRange = 3.f;

	m_fAccTime = 0.f;

	RESET_ONCE(m_bEffect0);
	RESET_ONCE(m_bEffect1);
	RESET_ONCE(m_bSound0);

	return S_OK;
}

void CEnemy_Leviathan_Thunder::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;

	USEGAMEINSTANCE;

	m_fAccTime += fTimeDelta;
	if (1.1f < m_fAccTime)
	{
		_float fAngle = fRandom(-180.f, 180.f);
		IMPL_ONCE(m_bEffect0, EFFECT_POS(m_bIsPhaseChanged ? "Boss_Dragon_Thunder_Red" : "Boss_Dragon_Thunder_Blue", m_pTransformCom->Get_WorldMatrix_ptr(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, XMConvertToRadians(fAngle), 0.f, 0.f), 1.f, XMVectorSet(5.f, 5.f, 5.f,0.f)));
		IMPL_ONCE(m_bEffect1, EFFECT_POS(m_bIsPhaseChanged ? "Boss_Dragon_Thunder_Red" : "Boss_Dragon_Thunder_Blue", m_pTransformCom->Get_WorldMatrix_ptr(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, XMConvertToRadians(fAngle + 90.f), 0.f, 0.f), 1.f, XMVectorSet(5.f, 5.f, 5.f, 0.f)));

		IMPL_ONCE(m_bSound0, MGR(CSound_Manager)->PlaySurroundSound(TEXT("Thunder.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT,
			m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION), 0.3f));

		m_pCol_Sphere->Update(m_pTransformCom->Get_WorldMatrix());

		for (auto& iter : *m_pCol_Sphere->Get_EnterColliders())
		{
			((CUnit*)iter->Get_Owner())->Damaged(ELEMENTAL_LIGHTNING, m_fDamage);
		}
	}
	if (2.1f < m_fAccTime)
	{
		pGameInstance->Add_PoolingObject(this);
		return;
	}

}

void CEnemy_Leviathan_Thunder::LateTick(_float fTimeDelta)
{
	if (IsInFrustum())
	{
		GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);
	}

	GAMEINSTANCE->Add_ColliderGroup(ID_UINT(COL_ENEMY_ATTACK), m_pCol_Sphere);
}

HRESULT CEnemy_Leviathan_Thunder::Render()
{
#ifdef _DEBUG
	m_pCol_Sphere->Render();
#endif

	return S_OK;
}

HRESULT CEnemy_Leviathan_Thunder::SetUp_Components()
{
	USEGAMEINSTANCE;

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(15.f, 0.f, 0.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK), (CComponent**)&m_pCol_Sphere, &ColliderDesc));
	m_pCol_Sphere->Set_Owner(this);

	return S_OK;
}

HRESULT CEnemy_Leviathan_Thunder::SetUp_ShaderResource()
{
	return S_OK;
}

CEnemy_Leviathan_Thunder* CEnemy_Leviathan_Thunder::Create()
{
	CEnemy_Leviathan_Thunder* pInstance = new CEnemy_Leviathan_Thunder();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEnemy_Leviathan_Thunder");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnemy_Leviathan_Thunder::Clone(void* pArg)
{
	CEnemy_Leviathan_Thunder* pInstance = new CEnemy_Leviathan_Thunder(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEnemy_Leviathan_Thunder");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnemy_Leviathan_Thunder::Free()
{
	__super::Free();
}
