#include "stdafx.h"
#include "State_Handler.h"

#include "Unit.h"
#include "GameInstance.h"
#include "Collider.h"
#include "Sound_Manager.h"
#include "Enemy_Leviathan_IceSpear.h"

#include "Effect_Base.h"
#include "Imgui_Manager.h"

CEnemy_Leviathan_IceSpear::CEnemy_Leviathan_IceSpear()
	: CBullet_Normal()
{
}

CEnemy_Leviathan_IceSpear::CEnemy_Leviathan_IceSpear(const CBullet_Normal& rhs)
	: CBullet_Normal(rhs)
{
}

HRESULT CEnemy_Leviathan_IceSpear::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemy_Leviathan_IceSpear::Initialize(void* pArg)
{
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

	if (!m_bIsPhaseChanged)
	{
		//EFFECT_POS("Boss_Weaver_Attack_06_Spear_Ground", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(fRandom(-fRange, fRange), 0.f, fRandom(-fRange, fRange), 1.f), XMVectorSet(0.f, XMConvertToRadians(fRandom(-180.f, 180.f)), 0.f, 0.f), 1, XMVectorSet(DEBUGFLOAT, DEBUGFLOAT, DEBUGFLOAT, 1.f));
		EFFECT_POS("Boss_Weaver_Attack_06_Spear_Spear1", Get_WorldMatrix(),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, XMConvertToRadians(fRandom(-45.f, 45.f)), 0.f, 0.f),
			1.f , XMVectorSet(1.5f, 1.5f, 1.5f, 1.5f)
		);
	}
	else
	{
		EFFECT_POS("Boss_Weaver_Attack_06_Spear_Spear2", Get_WorldMatrix(),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, XMConvertToRadians(fRandom(-45.f, 45.f)), 0.f, 0.f),
			1.f, XMVectorSet(1.5f, 1.5f, 1.5f, 1.5f)
		);
	}

	return S_OK;
}

HRESULT CEnemy_Leviathan_IceSpear::Initialize_Pooling(void* pArg)
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

	m_fAccTime = 0.f;

	m_fDamage = fRandom(640.f, 2040.f);

	if (!m_bIsPhaseChanged)
	{
		//EFFECT_POS("Boss_Weaver_Attack_06_Spear_Ground", m_pState_Handler->Get_Owner()->Get_WorldMatrix(), XMVectorSet(fRandom(-fRange, fRange), 0.f, fRandom(-fRange, fRange), 1.f), XMVectorSet(0.f, XMConvertToRadians(fRandom(-180.f, 180.f)), 0.f, 0.f), 1, XMVectorSet(DEBUGFLOAT, DEBUGFLOAT, DEBUGFLOAT, 1.f));
		EFFECT_POS("Boss_Weaver_Attack_06_Spear_Spear1", Get_WorldMatrix(),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, XMConvertToRadians(fRandom(-45.f, 45.f)), 0.f, 0.f),
			1.f, XMVectorSet(1.5f, 1.5f, 1.5f, 1.5f)
		);
	}
	else
	{
		EFFECT_POS("Boss_Weaver_Attack_06_Spear_Spear2", Get_WorldMatrix(),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, XMConvertToRadians(fRandom(-45.f, 45.f)), 0.f, 0.f),
			1.f, XMVectorSet(1.5f, 1.5f, 1.5f, 1.5f)
		);
	}

	RESET_ONCE(m_bSound0);

	return S_OK;
}

void CEnemy_Leviathan_IceSpear::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;

	USEGAMEINSTANCE;

	IMPL_ONCE(m_bSound0, MGR(CSound_Manager)->PlaySurroundSound(TEXT("IceField.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, Get_Pos(), 0.5f));

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

void CEnemy_Leviathan_IceSpear::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);

	GAMEINSTANCE->Add_ColliderGroup(ID_UINT(COL_ENEMY_ATTACK), m_pCol_Sphere);
}

HRESULT CEnemy_Leviathan_IceSpear::Render()
{
#ifdef _DEBUG
	m_pCol_Sphere->Render();
#endif

	return S_OK;
}

HRESULT CEnemy_Leviathan_IceSpear::SetUp_Components()
{
	USEGAMEINSTANCE;

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(10.f, 0.f, 0.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK), (CComponent**)&m_pCol_Sphere, &ColliderDesc));
	m_pCol_Sphere->Set_Owner(this);

	return S_OK;
}

HRESULT CEnemy_Leviathan_IceSpear::SetUp_ShaderResource()
{
	return S_OK;
}

CEnemy_Leviathan_IceSpear* CEnemy_Leviathan_IceSpear::Create()
{
	CEnemy_Leviathan_IceSpear* pInstance = new CEnemy_Leviathan_IceSpear();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEnemy_Leviathan_IceSpear");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnemy_Leviathan_IceSpear::Clone(void* pArg)
{
	CEnemy_Leviathan_IceSpear* pInstance = new CEnemy_Leviathan_IceSpear(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEnemy_Leviathan_IceSpear");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnemy_Leviathan_IceSpear::Free()
{
	__super::Free();
}
