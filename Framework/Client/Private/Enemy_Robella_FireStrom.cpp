#include "stdafx.h"
#include "State_Handler.h"

#include "Unit.h"
#include "GameInstance.h"
#include "Collider.h"
#include "Sound_Manager.h"
#include "..\Public\Enemy_Robella_FireStrom.h"

#include "Effect_Manager.h"
#include "Imgui_Manager.h"

CEnemy_Robella_FireStrom::CEnemy_Robella_FireStrom()
	: CGameObject()
{
}

CEnemy_Robella_FireStrom::CEnemy_Robella_FireStrom(const CGameObject& rhs)
	: CGameObject(rhs)
{
}

HRESULT CEnemy_Robella_FireStrom::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemy_Robella_FireStrom::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		__debugbreak();

	if (FAILED(SetUp_Components()))
		__debugbreak();

	if (nullptr != pArg)
	{
		memcpy(&m_vPos, pArg, sizeof(_float4));
	}

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_ENEMY, this);
	pGameInstance->Add_LateTickGroup(LTG_ENEMY, this);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_vPos));

	m_fDamage = fRandom(510.f, 580.f);

	return S_OK;
}

HRESULT CEnemy_Robella_FireStrom::Initialize_Pooling(void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_vPos, pArg, sizeof(_float4));
	}

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_ENEMY, this);
	pGameInstance->Add_LateTickGroup(LTG_ENEMY, this);

	m_pCol_Sphere->Clear_Colliders();

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_vPos));

	m_fAccTime = 0.f;

	RESET_ONCE(m_bEffect0);
	RESET_ONCE(m_bEffect1);

	RESET_ONCE(m_bSound0);
	RESET_ONCE(m_bSound1);

	return S_OK;
}

void CEnemy_Robella_FireStrom::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;

	USEGAMEINSTANCE;

	m_fAccTime += fTimeDelta;

	if(1.f < m_fAccTime)
	{
		// 포스 회전 스케일
		IMPL_ONCE(m_bEffect1, EFFECT_POS("Robella_Explo", Get_WorldMatrix(),
			XMVectorSet(0.f, -1.95f, 0.f, 1.f),
			XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f,
			XMVectorSet(1.f, 1.5f, 1.f, 0.f)));

		for (auto& iter : *m_pCol_Sphere->Get_EnterColliders())
		{
			((CUnit*)iter->Get_Owner())->Damaged(ELEMENTAL_FIRE, fRandom(510.f, 580.f));
		}

		IMPL_ONCE(m_bSound0, MGR(CSound_Manager)->PlaySurroundSound(TEXT("FireTorado_1Phase.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT,
			m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION), 0.1f));

		if (1.2f < m_fAccTime)
		{
			IMPL_ONCE(m_bSound1, MGR(CSound_Manager)->PlaySurroundSound(TEXT("Fire_Tornado.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT,
				m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION), 0.6f));
		}
	}
	else
	{
		IMPL_ONCE(m_bEffect0, EFFECT_POS("Boss_Robella_Poision_Mark", Get_WorldMatrix(),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMVectorSet(0.f, 0.f, 0.f, 0.f), 1.f,
			XMVectorSet(0.5f, 0.5f, 0.5f, 0.f)));
	}

	if (m_fAccTime > 1.5f)
		pGameInstance->Add_PoolingObject(this);

	m_pCol_Sphere->Update(m_pTransformCom->Get_WorldMatrix());
}

void CEnemy_Robella_FireStrom::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);

	GAMEINSTANCE->Add_ColliderGroup(ID_UINT(COL_ENEMY_ATTACK), m_pCol_Sphere);
}

HRESULT CEnemy_Robella_FireStrom::Render()
{
#ifdef _DEBUG
	m_pCol_Sphere->Render();
#endif

	return S_OK;
}

HRESULT CEnemy_Robella_FireStrom::SetUp_Components()
{
	USEGAMEINSTANCE;

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(3.f, 0.f, 0.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 1.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK), (CComponent**)&m_pCol_Sphere, &ColliderDesc));
	m_pCol_Sphere->Set_Owner(this);

	return S_OK;
}

HRESULT CEnemy_Robella_FireStrom::SetUp_ShaderResource()
{
	return S_OK;
}

CEnemy_Robella_FireStrom* CEnemy_Robella_FireStrom::Create()
{
	CEnemy_Robella_FireStrom* pInstance = new CEnemy_Robella_FireStrom();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEnemy_Robella_FireStrom");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnemy_Robella_FireStrom::Clone(void* pArg)
{
	CEnemy_Robella_FireStrom* pInstance = new CEnemy_Robella_FireStrom(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEnemy_Robella_FireStrom");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnemy_Robella_FireStrom::Free()
{
	__super::Free();
}
