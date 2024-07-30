#include "stdafx.h"
#include "State_Handler.h"

#include "Unit.h"
#include "GameInstance.h"
#include "Collider.h"
#include "Sound_Manager.h"
#include "..\Public\Enemy_Thunder_Dragon.h"

#include "Effect_Manager.h"
#include "Imgui_Manager.h"

CEnemy_Thunder_FrostDragon::CEnemy_Thunder_FrostDragon()
	: CGameObject()
{
}

CEnemy_Thunder_FrostDragon::CEnemy_Thunder_FrostDragon(const CGameObject& rhs)
	: CGameObject(rhs)
{
}

HRESULT CEnemy_Thunder_FrostDragon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemy_Thunder_FrostDragon::Initialize(void* pArg)
{
	m_fFrustumRange = 3.f;

	if (FAILED(__super::Initialize(pArg)))
		__debugbreak();

	if (FAILED(SetUp_Components()))
		__debugbreak();

	if (nullptr != pArg)
	{
		memcpy(&m_vPos, pArg, sizeof(_float4));
	}
	m_bIsPhaseChanged = (_bool)m_vPos.w;
	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_ENEMY, this);
	pGameInstance->Add_LateTickGroup(LTG_ENEMY, this);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_vPos));

	if(!m_bIsPhaseChanged)
		m_fDamage = fRandom(510.f, 580.f);
	else
		m_fDamage = fRandom(640.f, 700.f);

	EFFECT_POS(m_bIsPhaseChanged ? "Boss_Dragon_Thunder_Red_Mark" : "Boss_Dragon_Thunder_Blue_Mark", nullptr, XMVectorSet(m_vPos.x, m_vPos.y, m_vPos.z, 1.f));

	return S_OK;
}

HRESULT CEnemy_Thunder_FrostDragon::Initialize_Pooling(void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_vPos, pArg, sizeof(_float4));
	}
	m_bIsPhaseChanged = (_bool)m_vPos.w;
	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_ENEMY, this);
	pGameInstance->Add_LateTickGroup(LTG_ENEMY, this);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_vPos));

	if (!m_bIsPhaseChanged)
		m_fDamage = fRandom(510.f, 580.f);
	else
		m_fDamage = fRandom(640.f, 700.f);

	m_pTransformCom->Reset_StraightAccTime();
	m_pCol_Sphere->Clear_Colliders();


	EFFECT_POS(m_bIsPhaseChanged ? "Boss_Dragon_Thunder_Red_Mark" : "Boss_Dragon_Thunder_Blue_Mark", nullptr, XMVectorSet(m_vPos.x, m_vPos.y, m_vPos.z, 1.f));

	m_fFrustumRange = 3.f;

	m_fAccTime = 0.f;

	RESET_ONCE(m_bEffect0);
	RESET_ONCE(m_bEffect1);
	RESET_ONCE(m_bSound0);

	return S_OK;
}

void CEnemy_Thunder_FrostDragon::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;

	USEGAMEINSTANCE;

	m_fAccTime += fTimeDelta;
	if (1.1f < m_fAccTime)
	{
		_float fAngle = fRandom(-180.f, 180.f);
		IMPL_ONCE(m_bEffect0, EFFECT_POS(m_bIsPhaseChanged ? "Boss_Dragon_Thunder_Red" : "Boss_Dragon_Thunder_Blue", m_pTransformCom->Get_WorldMatrix_ptr(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, XMConvertToRadians(fAngle), 0.f, 0.f)));
		IMPL_ONCE(m_bEffect1, EFFECT_POS(m_bIsPhaseChanged ? "Boss_Dragon_Thunder_Red" : "Boss_Dragon_Thunder_Blue", m_pTransformCom->Get_WorldMatrix_ptr(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, XMConvertToRadians(fAngle + 90.f), 0.f, 0.f)));
	
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

void CEnemy_Thunder_FrostDragon::LateTick(_float fTimeDelta)
{
	if (IsInFrustum())
	{
		GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);
	}

	if (1.1f < m_fAccTime)
		GAMEINSTANCE->Add_ColliderGroup(ID_UINT(COL_ENEMY_ATTACK), m_pCol_Sphere);
}

HRESULT CEnemy_Thunder_FrostDragon::Render()
{
#ifdef _DEBUG
	m_pCol_Sphere->Render();
#endif

	return S_OK;
}

HRESULT CEnemy_Thunder_FrostDragon::SetUp_Components()
{
	USEGAMEINSTANCE;

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(4.f, 0.f, 0.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK), (CComponent**)&m_pCol_Sphere, &ColliderDesc));
	m_pCol_Sphere->Set_Owner(this);

	return S_OK;
}

HRESULT CEnemy_Thunder_FrostDragon::SetUp_ShaderResource()
{
	return S_OK;
}

CEnemy_Thunder_FrostDragon* CEnemy_Thunder_FrostDragon::Create()
{
	CEnemy_Thunder_FrostDragon* pInstance = new CEnemy_Thunder_FrostDragon();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEnemy_Thunder_FrostDragon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnemy_Thunder_FrostDragon::Clone(void* pArg)
{
	CEnemy_Thunder_FrostDragon* pInstance = new CEnemy_Thunder_FrostDragon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEnemy_Thunder_FrostDragon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnemy_Thunder_FrostDragon::Free()
{
	__super::Free();
}
