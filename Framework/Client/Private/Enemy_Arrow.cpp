#include "stdafx.h"
#include "..\Public\Enemy_Arrow.h"
#include "State_Handler.h"
#include "Navigation.h"

#include "GameInstance.h"
#include "UI.h"
#include "UI_Solid.h"
#include "Player.h"
#include "Effect_Manager.h"

CEnemy_Arrow::CEnemy_Arrow()
	: CBullet()
{
}

CEnemy_Arrow::CEnemy_Arrow(const CEnemy_Arrow& rhs)
	: CBullet(rhs)
{
}

HRESULT CEnemy_Arrow::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemy_Arrow::Initialize(void* pArg)
{
	m_fFrustumRange = 0.5f;

	FAILEDCHECK(__super::Initialize(pArg));

	FAILEDCHECK(SetUp_Components());

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_ENEMY, this);
	pGameInstance->Add_LateTickGroup(LTG_ENEMY, this);

	memcpy(&m_BulletDesc, pArg, sizeof(CEnemy_Arrow::BULLETDESC));

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_BulletDesc.vStartPos));
	m_pTransformCom->LookAt(XMLoadFloat4(&m_BulletDesc.vLookAt));
	return S_OK;
}

HRESULT CEnemy_Arrow::Initialize_Pooling(void* pArg)
{
	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_ENEMY, this);
	pGameInstance->Add_LateTickGroup(LTG_ENEMY, this);

	memcpy(&m_BulletDesc, pArg, sizeof(CEnemy_Arrow::BULLETDESC));

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_BulletDesc.vStartPos));
	m_pTransformCom->LookAt(XMLoadFloat4(&m_BulletDesc.vLookAt));
	m_fFrustumRange = 0.5f;

	return S_OK;
}

void CEnemy_Arrow::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;

	m_fDeadTimeAcc += fTimeDelta;
	m_fAccTime += fTimeDelta;

	USEGAMEINSTANCE;
	CPlayer* Player = (CPlayer*)pGameInstance->Get_Object(LEVEL_GAMEPLAY, ID_UINT(PLAYER));

	m_pTransformCom->Go_Straight(fTimeDelta, m_BulletDesc.fBulletSpeed);

	//if(XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION))< 설정한 y보다 작아진다면 )
	if (m_fDeadTimeAcc > m_BulletDesc.fBulletDeadTime)
		pGameInstance->Add_DeadObject(this);


	m_pCol_Sphere->Update(m_pTransformCom->Get_WorldMatrix());
	m_pTransformCom->Tick(fTimeDelta);

	m_fDamage = fRandom(400.f, 500.f);

	for (auto& iter : *m_pCol_Sphere->Get_EnterColliders())
	{
		pGameInstance->Add_PoolingObject(this);
		((CUnit*)iter->Get_Owner())->Damaged(ELEMENTAL_MELEE, m_fDamage);
	}

}

void CEnemy_Arrow::LateTick(_float fTimeDelta)
{
	//if (!m_pTransformCom->Is_InAir())
	//	m_pNaviCom->SetUp_OnTerrain(m_pTransformCom);

	USEGAMEINSTANCE;
	if (IsInFrustum())
	{
		GAMEINSTANCE->Add_RenderGroup(RENDER_NONALPHABLEND, this);

		if (m_fAccTime >= 0.05f)
		{
			_matrix OffsetMatrix = /*XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(-90.f))
				**/ XMMatrixTranslation(0.f, -0.5f, 0.f);

			IMPL_ONCE(m_bEffect0, EFFECT_EXCUTE("Monster_Arrow", OffsetMatrix, 1.f, this, CEffect_Manager::FOLLOW_POS_RUL, ""));
		}
	}
	pGameInstance->Add_ColliderGroup(ID_UINT(COL_ENEMY_ATTACK), m_pCol_Sphere);
}

HRESULT CEnemy_Arrow::SetUp_Components()
{
	__super::SetUp_Components();

	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXMODEL), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_STATIC, MODEL_BULLET, MODEL_ENEMY_BULLET_ARROW), ID_UINT(MODEL_STATIC), (CComponent**)&m_pModelCom));
	m_pModelCom->Set_Owner(this);


	CCollider::COLLIDERDESC			ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.1f, 0.1f, 0.1f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 1.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK), (CComponent**)&m_pCol_Sphere, &ColliderDesc));
	m_pCol_Sphere->Set_Owner(this);

	return S_OK;
}

HRESULT CEnemy_Arrow::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		__debugbreak();

	USEGAMEINSTANCE;

	FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));

	return S_OK;
}


CEnemy_Arrow* CEnemy_Arrow::Create()
{
	CEnemy_Arrow* pInstance = new CEnemy_Arrow();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEnemy_Arrow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnemy_Arrow::Clone(void* pArg)
{
	CEnemy_Arrow* pInstance = new CEnemy_Arrow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEnemy_Arrow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnemy_Arrow::Free()
{
	__super::Free();
}
