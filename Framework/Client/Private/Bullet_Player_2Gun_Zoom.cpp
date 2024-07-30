#include "stdafx.h"
#include "Bullet_Player_2Gun_Zoom.h"
#include "Player.h"

#include "GameInstance.h"
#include "Camera.h"
#include "Effect_Manager.h"

CBullet_Player_2Gun_Zoom::CBullet_Player_2Gun_Zoom()
	: CBullet()
{
}

CBullet_Player_2Gun_Zoom::CBullet_Player_2Gun_Zoom(const CBullet_Player_2Gun_Zoom& rhs)
	: CBullet(rhs)
{
}

HRESULT CBullet_Player_2Gun_Zoom::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBullet_Player_2Gun_Zoom::Initialize(void* pArg)
{
	m_fFrustumRange = 0.5f;
	FAILEDCHECK(__super::Initialize(pArg));

	FAILEDCHECK(SetUp_Components());

	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_WEAPON, this);
	pGameInstance->Add_LateTickGroup(LTG_WEAPON, this);

	if (nullptr != pArg)
	{
		memcpy(&m_vPos, pArg, sizeof(_float4));
	}

	m_BulletDesc.vStartPos = m_vPos;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_BulletDesc.vStartPos));

	m_pOwner = pGameInstance->Get_Object(LEVEL_GAMEPLAY, ID_UINT(PLAYER), 0);

	m_pTransformCom->LookTo(pGameInstance->Get_CurrentCamera()->Get_Transform()->Get_WorldMatrix().r[2]);

	XMStoreFloat4(&m_BulletDesc.vLookAt, pGameInstance->Get_CurrentCamera()->Get_Transform()->Get_WorldMatrix().r[2]);

	m_BulletDesc.fBulletSpeed = 50.f;
	m_BulletDesc.fBulletDeadTime = 2.f;
	m_fDamage = fRandom(150.f, 200.f);  //10.f;
	m_fDeadTimeAcc = 0;

	EFFECT_EXCUTE("Player_Gun_Attack_Bullet", XMMatrixIdentity(), 1.f, this, CEffect_Manager::FOLLOW_POS_RUL);
	return S_OK;
}

HRESULT CBullet_Player_2Gun_Zoom::Initialize_Pooling(void* pArg)
{
	USEGAMEINSTANCE;

	pGameInstance->Add_TickGroup(TG_WEAPON, this);
	pGameInstance->Add_LateTickGroup(LTG_WEAPON, this);

	if (nullptr != pArg)
	{
		memcpy(&m_vPos, pArg, sizeof(_float4));
	}

	m_BulletDesc.vStartPos = m_vPos;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_BulletDesc.vStartPos));

	m_pTransformCom->LookTo(pGameInstance->Get_CurrentCamera()->Get_Transform()->Get_WorldMatrix().r[2]);

	XMStoreFloat4(&m_BulletDesc.vLookAt, pGameInstance->Get_CurrentCamera()->Get_Transform()->Get_WorldMatrix().r[2]);

	m_fFrustumRange = 0.5f;
	m_BulletDesc.fBulletSpeed = 50.f;
	m_BulletDesc.fBulletDeadTime = 2.f;
	m_fDamage = fRandom(150.f, 200.f);  //10.f;
	m_fDeadTimeAcc = 0;

	m_pCol_Sphere->Clear_Colliders();

	EFFECT_EXCUTE("Player_Gun_Attack_Bullet", XMMatrixIdentity(), 1.f, this, CEffect_Manager::FOLLOW_POS_RUL);
	return S_OK;
}

void CBullet_Player_2Gun_Zoom::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;
	USEGAMEINSTANCE;

	m_pTransformCom->Go_Straight(fTimeDelta, m_BulletDesc.fBulletSpeed);

	m_fDeadTimeAcc += fTimeDelta;
	if (m_BulletDesc.fBulletDeadTime < m_fDeadTimeAcc)
		pGameInstance->Add_PoolingObject(this);

	m_pCol_Sphere->Update(m_pTransformCom->Get_WorldMatrix());
	for (auto& iter : *m_pCol_Sphere->Get_EnterColliders())
	{
		pGameInstance->Add_PoolingObject(this);
		((CUnit*)iter->Get_Owner())->Damaged(ELEMENTAL_LIGHTNING, m_fDamage);
	}
}

void CBullet_Player_2Gun_Zoom::LateTick(_float fTimeDelta)
{
	USEGAMEINSTANCE;
	if (IsInFrustum())
	{
		pGameInstance->Add_RenderGroup(RENDER_NONALPHABLEND, this);
	}
	pGameInstance->Add_ColliderGroup(ID_UINT(COL_PLAYER_ATTACK), m_pCol_Sphere);

}

HRESULT CBullet_Player_2Gun_Zoom::Render()
{
	//if (nullptr == m_pShaderCom || nullptr == m_pModelCom)
	//	return E_FAIL;

	//_uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	/* 셰이더 전역변수에 값을 던진다. */
	//FAILEDCHECK(SetUp_ShaderResource());
	//
	//m_pShaderCom->Begin(0);
	//
	//for (_uint i = 0; i < iNumMeshContainers; ++i)
	//{
	//	FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE));
	//	FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS));
	//
	//	m_pModelCom->Render(i, m_pShaderCom);
	//}

#ifdef _DEBUG

	if (nullptr != m_pCol_Sphere)
		m_pCol_Sphere->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CBullet_Player_2Gun_Zoom::SetUp_Components()
{
	__super::SetUp_Components();

	//FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(SHADER, SHADER_VTXMODEL), ID_UINT(SHADER), (CComponent**)&m_pShaderCom));
	//FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_STATIC, MODEL_BULLET, MODEL_ENEMY_BULLET_ARROW), ID_UINT(MODEL_STATIC), (CComponent**)&m_pModelCom));
	//m_pModelCom->Set_Owner(this);


	CCollider::COLLIDERDESC			ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.1f, 0.1f, .1f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_PLAYER_ATTACK), (CComponent**)&m_pCol_Sphere, &ColliderDesc));
	m_pCol_Sphere->Set_Owner(this);

	return S_OK;
}

#if 0
HRESULT CBullet_Player_2Gun::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		__debugbreak();

	USEGAMEINSTANCE;

	FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));

	return S_OK;
}
#endif // 0



CBullet_Player_2Gun_Zoom* CBullet_Player_2Gun_Zoom::Create()
{
	CBullet_Player_2Gun_Zoom* pInstance = new CBullet_Player_2Gun_Zoom();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBullet_Player_2Gun_Zoom");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBullet_Player_2Gun_Zoom::Clone(void* pArg)
{
	CBullet_Player_2Gun_Zoom* pInstance = new CBullet_Player_2Gun_Zoom(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBullet_Player_2Gun_Zoom");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBullet_Player_2Gun_Zoom::Free()
{
	__super::Free();
}
