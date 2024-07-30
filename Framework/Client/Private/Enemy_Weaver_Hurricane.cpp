#include "stdafx.h"
#include "State_Handler.h"

#include "Unit.h"
#include "GameInstance.h"
#include "Collider.h"
#include "Sound_Manager.h"
#include "..\Public\Enemy_Weaver_Hurricane.h"

#include "Effect_Manager.h"
#include "Imgui_Manager.h"

CEnemy_Weaver_Hurricane::CEnemy_Weaver_Hurricane()
	: CGameObject()
{
}

CEnemy_Weaver_Hurricane::CEnemy_Weaver_Hurricane(const CGameObject& rhs)
	: CGameObject(rhs)
{
}

HRESULT CEnemy_Weaver_Hurricane::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemy_Weaver_Hurricane::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		__debugbreak();

	if (FAILED(SetUp_Components()))
		__debugbreak();

	if (nullptr != pArg)
	{
		memcpy(&m_HurricaneDesc, pArg, sizeof(HURRICANEDESC));
	}

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_ENEMY, this);
	pGameInstance->Add_LateTickGroup(LTG_ENEMY, this);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_HurricaneDesc.StartPos));

	switch (m_HurricaneDesc.iIndex)
	{
	case 0:
		EFFECT_EXCUTE("Boss_Weaver_Attack_04_Tornado_Mini_A", XMMatrixIdentity(), 1.f, this, CEffect_Manager::FOLLOW_POS);
		break;
	case 1:
		EFFECT_EXCUTE("Boss_Weaver_Attack_04_Tornado_Mini_B", XMMatrixIdentity(), 1.f, this, CEffect_Manager::FOLLOW_POS);
		break;
	case 2:
		EFFECT_EXCUTE("Boss_Weaver_Attack_04_Tornado_Mini_C", XMMatrixIdentity(), 1.f, this, CEffect_Manager::FOLLOW_POS);
		break;
	default:
		break;
	}
	IMPL_ONCE(m_bSound, MGR(CSound_Manager)->PlaySurroundSound(TEXT("Hurricane.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, Get_Pos(), 0.5f));

	return S_OK;
}

HRESULT CEnemy_Weaver_Hurricane::Initialize_Pooling(void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_HurricaneDesc, pArg, sizeof(HURRICANEDESC));
	}

	USEGAMEINSTANCE;
	pGameInstance->Add_TickGroup(TG_ENEMY, this);
	pGameInstance->Add_LateTickGroup(LTG_ENEMY, this);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_HurricaneDesc.StartPos));

	m_fSound[0] = 0.f;
	m_fSound[1] = 0.f;
	m_fAccTime = 0.f;
	m_fDamagedTime = 0.f;
	m_bSound = true;

	switch (m_HurricaneDesc.iIndex)
	{
	case 0:
		EFFECT_EXCUTE("Boss_Weaver_Attack_04_Tornado_Mini_A", XMMatrixIdentity(), 1.f, this, CEffect_Manager::FOLLOW_POS);
		break;
	case 1:
		EFFECT_EXCUTE("Boss_Weaver_Attack_04_Tornado_Mini_B", XMMatrixIdentity(), 1.f, this, CEffect_Manager::FOLLOW_POS);
		break;
	case 2:
		EFFECT_EXCUTE("Boss_Weaver_Attack_04_Tornado_Mini_C", XMMatrixIdentity(), 1.f, this, CEffect_Manager::FOLLOW_POS);
		break;
	default:
		break;
	}
	IMPL_ONCE(m_bSound, MGR(CSound_Manager)->PlaySurroundSound(TEXT("Hurricane.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, Get_Pos(), 0.5f));


	return S_OK;
}

void CEnemy_Weaver_Hurricane::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;

	USEGAMEINSTANCE;

	IMPL_ALARM(m_fSound[0], MGR(CSound_Manager)->PlaySurroundSound(TEXT("Hurricane_Loop.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, Get_Pos(), 0.5f), 0.5f, fTimeDelta);
	//IMPL_ALARM(m_fSound[1], MGR(CSound_Manager)->PlaySurroundSound(TEXT("IceBeam.wav"), CSound_Manager::CHANNEL_ENEMY_EFFECT, Get_Pos(), 0.5f), 1.5f, fTimeDelta);

	m_fAccTime += fTimeDelta;
	m_fDamagedTime += fTimeDelta;

	_float4 vOwnerPos;
	XMStoreFloat4(&vOwnerPos, m_HurricaneDesc.pTarget->Get_Pos());
	m_pTransformCom->OrbitY(m_HurricaneDesc.OrbitPos, vOwnerPos, m_fAccTime * 1.5f);
	XMStoreFloat4(&m_vStartLook, (m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) - m_HurricaneDesc.pTarget->Get_Pos()));
	m_pTransformCom->YFixedLook(XMLoadFloat4(&m_vStartLook));
//	m_pTransformCom->Go_Straight(fTimeDelta, 2.f);

	for (auto& Attackediter : *m_pCol_Sphere->Get_EnterColliders())
	{
		((CUnit*)Attackediter->Get_Owner())->Damaged(ELEMENTAL_ICE, fRandom(510.f, 580.f));
	}

	if (5.f < m_fAccTime)
	{
		pGameInstance->Add_PoolingObject(this);
	}

	m_pCol_Sphere->Update(m_pTransformCom->Get_WorldMatrix());
}

void CEnemy_Weaver_Hurricane::LateTick(_float fTimeDelta)
{
	GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);

	GAMEINSTANCE->Add_ColliderGroup(ID_UINT(COL_ENEMY_ATTACK), m_pCol_Sphere);
}

HRESULT CEnemy_Weaver_Hurricane::Render()
{
#ifdef _DEBUG
	m_pCol_Sphere->Render();
#endif

	return S_OK;
}

HRESULT CEnemy_Weaver_Hurricane::SetUp_Components()
{
	USEGAMEINSTANCE;

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(4.f, 0.f, 0.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 1.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK), (CComponent**)&m_pCol_Sphere, &ColliderDesc));
	m_pCol_Sphere->Set_Owner(this);

	return S_OK;
}

HRESULT CEnemy_Weaver_Hurricane::SetUp_ShaderResource()
{
	return S_OK;
}

CEnemy_Weaver_Hurricane* CEnemy_Weaver_Hurricane::Create()
{
	CEnemy_Weaver_Hurricane* pInstance = new CEnemy_Weaver_Hurricane();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEnemy_Weaver_Hurricane");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnemy_Weaver_Hurricane::Clone(void* pArg)
{
	CEnemy_Weaver_Hurricane* pInstance = new CEnemy_Weaver_Hurricane(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEnemy_Weaver_Hurricane");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnemy_Weaver_Hurricane::Free()
{
	__super::Free();
}
