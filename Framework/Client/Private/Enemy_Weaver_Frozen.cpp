#include "stdafx.h"
#include "State_Handler.h"

#include "Unit.h"
#include "GameInstance.h"
#include "Collider.h"
#include "Sound_Manager.h"
#include "..\Public\Enemy_Weaver_Frozen.h"

#include "Effect_Manager.h"
#include "Imgui_Manager.h"

CEnemy_Weaver_Frozen::CEnemy_Weaver_Frozen()
	: CGameObject()
{
}

CEnemy_Weaver_Frozen::CEnemy_Weaver_Frozen(const CGameObject& rhs)
	: CGameObject(rhs)
{
}

HRESULT CEnemy_Weaver_Frozen::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemy_Weaver_Frozen::Initialize(void* pArg)
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

	m_fDamage = fRandom(840.f, 900.f);

	if (!m_bIsPhaseChanged)
	{
		//EFFECT_POS("Boss_Weaver_Attack_06_Spear_Ground", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(fRandom(-fRange, fRange), 0.f, fRandom(-fRange, fRange), 1.f), XMVectorSet(0.f, XMConvertToRadians(fRandom(-180.f, 180.f)), 0.f, 0.f), 1, XMVectorSet(DEBUGFLOAT, DEBUGFLOAT, DEBUGFLOAT, 1.f));
		EFFECT_POS("Boss_Weaver_Attack_06_Spear_Spear1", Get_WorldMatrix(),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, XMConvertToRadians(fRandom(-45.f, 45.f)), 0.f, 0.f)
		);
	}
	else
	{
		EFFECT_POS("Boss_Weaver_Attack_06_Spear_Spear2", Get_WorldMatrix(),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, XMConvertToRadians(fRandom(-45.f, 45.f)), 0.f, 0.f)
		);
	}

	return S_OK;
}

HRESULT CEnemy_Weaver_Frozen::Initialize_Pooling(void* pArg)
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
	m_pCol_Sphere->Clear_Colliders();

	m_fAccTime = 0.f;
	m_bSound = true;

	m_fDamage = fRandom(640.f, 700.f);

	if (!m_bIsPhaseChanged)
	{
		//EFFECT_POS("Boss_Weaver_Attack_06_Spear_Ground", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(fRandom(-fRange, fRange), 0.f, fRandom(-fRange, fRange), 1.f), XMVectorSet(0.f, XMConvertToRadians(fRandom(-180.f, 180.f)), 0.f, 0.f), 1, XMVectorSet(DEBUGFLOAT, DEBUGFLOAT, DEBUGFLOAT, 1.f));
		EFFECT_POS("Boss_Weaver_Attack_06_Spear_Spear1", Get_WorldMatrix(),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, XMConvertToRadians(fRandom(-45.f, 45.f)), 0.f, 0.f)
		);
	}
	else
	{
		EFFECT_POS("Boss_Weaver_Attack_06_Spear_Spear2", Get_WorldMatrix(),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, XMConvertToRadians(fRandom(-45.f, 45.f)), 0.f, 0.f)
		);
	}


	return S_OK;
}

void CEnemy_Weaver_Frozen::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;

	USEGAMEINSTANCE;
	IMPL_ONCE(m_bSound, MGR(CSound_Manager)->PlaySurroundSound(TEXT("IceField.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, Get_Pos(), 0.5f));

	m_fAccTime += fTimeDelta;

	_float fRange = 10;

	if (1.06f < m_fAccTime)
	{
		pGameInstance->Add_PoolingObject(this);
		return;
	}

	if(0.3f < m_fAccTime && 0.6f > m_fAccTime)
	{
		m_pCol_Sphere->Update(m_pTransformCom->Get_WorldMatrix());
		for (auto& iter : *m_pCol_Sphere->Get_EnterColliders())
		{
			((CUnit*)iter->Get_Owner())->Damaged(ELEMENTAL_ICE, m_fDamage);
		}
	}

}

void CEnemy_Weaver_Frozen::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);

	GAMEINSTANCE->Add_ColliderGroup(ID_UINT(COL_ENEMY_ATTACK), m_pCol_Sphere);
}

HRESULT CEnemy_Weaver_Frozen::Render()
{
#ifdef _DEBUG
	m_pCol_Sphere->Render();
#endif

	return S_OK;
}

HRESULT CEnemy_Weaver_Frozen::SetUp_Components()
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

HRESULT CEnemy_Weaver_Frozen::SetUp_ShaderResource()
{
	return S_OK;
}

CEnemy_Weaver_Frozen* CEnemy_Weaver_Frozen::Create()
{
	CEnemy_Weaver_Frozen* pInstance = new CEnemy_Weaver_Frozen();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEnemy_Weaver_Frozen");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnemy_Weaver_Frozen::Clone(void* pArg)
{
	CEnemy_Weaver_Frozen* pInstance = new CEnemy_Weaver_Frozen(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEnemy_Weaver_Frozen");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnemy_Weaver_Frozen::Free()
{
	__super::Free();
}
