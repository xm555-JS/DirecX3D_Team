#include "stdafx.h"
#include "State_Handler.h"

#include "Unit.h"
#include "GameInstance.h"
#include "Collider.h"
#include "Sound_Manager.h"
#include "..\Public\Enemy_Weaver_ShockWave.h"

#include "Effect_Manager.h"
#include "Imgui_Manager.h"

CEnemy_Weaver_ShockWave::CEnemy_Weaver_ShockWave()
	: CGameObject()
{
}

CEnemy_Weaver_ShockWave::CEnemy_Weaver_ShockWave(const CGameObject& rhs)
	: CGameObject(rhs)
{
}

HRESULT CEnemy_Weaver_ShockWave::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemy_Weaver_ShockWave::Initialize(void* pArg)
{
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

	m_fDamage = fRandom(640.f, 700.f);

	m_fAccTime = 1.f;

	//EFFECT_POS(m_bIsPhaseChanged ? "Boss_Dragon_Thunder_Red_Mark" : "Boss_Dragon_Thunder_Blue_Mark", nullptr, XMVectorSet(m_vPos.x, m_vPos.y, m_vPos.z, 1.f));

	return S_OK;
}

HRESULT CEnemy_Weaver_ShockWave::Initialize_Pooling(void* pArg)
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

	m_fDamage = fRandom(640.f, 700.f);
	//EFFECT_POS(m_bIsPhaseChanged ? "Boss_Dragon_Thunder_Red_Mark" : "Boss_Dragon_Thunder_Blue_Mark", nullptr, XMVectorSet(m_vPos.x, m_vPos.y, m_vPos.z, 1.f));

	return S_OK;
}

void CEnemy_Weaver_ShockWave::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;

	USEGAMEINSTANCE;

	m_fAccTime += fTimeDelta;
	m_fDamagedTime += fTimeDelta;
	if (1.1f < m_fAccTime)
	{
		_float fAngle = fRandom(-180.f, 180.f);

		_vector vScale0 = m_pCol_Sphere->Get_Scale();
		_vector vScale1 = m_pCol_Safe->Get_Scale();

		m_pCol_Sphere->Set_Scale(vScale0 * m_fAccTime * 2.f);
		m_pCol_Safe->Set_Scale(vScale1 * m_fAccTime * 2.f);

		//이펙트 알아서 바꾸세요
		//IMPL_ONCE(m_bEffect0, EFFECT_POS(m_bIsPhaseChanged ? "Boss_Dragon_Thunder_Red" : "Boss_Dragon_Thunder_Blue", m_pTransformCom->Get_WorldMatrix_ptr(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, XMConvertToRadians(fAngle), 0.f, 0.f)));
		//IMPL_ONCE(m_bEffect1, EFFECT_POS(m_bIsPhaseChanged ? "Boss_Dragon_Thunder_Red_Sprite" : "Boss_Dragon_Thunder_Blue", m_pTransformCom->Get_WorldMatrix_ptr(), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, XMConvertToRadians(fAngle + 90.f), 0.f, 0.f)));
	}


	if(0.5f < m_fDamagedTime)
	{
		for (auto& Attackediter : *m_pCol_Sphere->Get_StayColliders())
		{
			for (auto& Safeiter : *m_pCol_Safe->Get_StayColliders())
			{
				if(Attackediter != Safeiter)
					((CUnit*)Attackediter->Get_Owner())->Damaged(ELEMENTAL_ICE, m_fDamage);
			}
		}
		m_fDamagedTime = 0.f;
	}

	m_pCol_Sphere->Update(m_pTransformCom->Get_WorldMatrix());
	m_pCol_Safe->Update(m_pTransformCom->Get_WorldMatrix());
}

void CEnemy_Weaver_ShockWave::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);
	if (1.1f < m_fAccTime)
	{
		GAMEINSTANCE->Add_ColliderGroup(ID_UINT(COL_ENEMY_ATTACK), m_pCol_Sphere);
		GAMEINSTANCE->Add_ColliderGroup(ID_UINT(COL_ENEMY_SAFERANGE), m_pCol_Safe);
	}
}

HRESULT CEnemy_Weaver_ShockWave::Render()
{
#ifdef _DEBUG
	m_pCol_Sphere->Render();
	m_pCol_Safe->Render();
#endif

	return S_OK;
}

HRESULT CEnemy_Weaver_ShockWave::SetUp_Components()
{
	USEGAMEINSTANCE;

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(6.f, 0.f, 0.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK), (CComponent**)&m_pCol_Sphere, &ColliderDesc));
	m_pCol_Sphere->Set_Owner(this);

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(4.f, 0.f, 0.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_SAFERANGE), (CComponent**)&m_pCol_Safe, &ColliderDesc));
	m_pCol_Safe->Set_Owner(this);

	return S_OK;
}

HRESULT CEnemy_Weaver_ShockWave::SetUp_ShaderResource()
{
	return S_OK;
}

CEnemy_Weaver_ShockWave* CEnemy_Weaver_ShockWave::Create()
{
	CEnemy_Weaver_ShockWave* pInstance = new CEnemy_Weaver_ShockWave();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEnemy_Weaver_ShockWave");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnemy_Weaver_ShockWave::Clone(void* pArg)
{
	CEnemy_Weaver_ShockWave* pInstance = new CEnemy_Weaver_ShockWave(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEnemy_Weaver_ShockWave");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnemy_Weaver_ShockWave::Free()
{
	__super::Free();
}
