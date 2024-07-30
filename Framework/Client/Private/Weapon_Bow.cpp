#include "stdafx.h"
#include "..\Public\Weapon_Bow.h"

#include "GameInstance.h"
#include "HIerarchyNode.h"
#include "Unit.h"

CWeapon_Bow::CWeapon_Bow()
	: CWeapon()
{

}

CWeapon_Bow::CWeapon_Bow(const CWeapon_Bow & rhs)
	: CWeapon(rhs)
{

}

HRESULT CWeapon_Bow::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CWeapon_Bow::Initialize(void * pArg)
{
	USEGAMEINSTANCE;

	CTransform::TRANSFORMDESC		TransformDesc;
	FAILEDCHECK(__super::Initialize(&TransformDesc));
	FAILEDCHECK(SetUp_Components());

	m_fTimeAccDissolve = 0.f;
	m_fTimeDurationDissolve = 3.f;
	m_iDissolveTextureNoise = 252;
	m_iDissolveTextureColor = 75;

	m_pTransformCom->Set_Scaled(_float3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-90.0f));
	
	return S_OK;
}

void CWeapon_Bow::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pCol_Sphere->Update(XMLoadFloat4x4(&m_WorldMatrix));

	for (auto& iter : *m_pCol_Sphere->Get_EnterColliders())
	{
		((CUnit*)iter->Get_Owner())->Get_UnitCurInfo()->fHP -= ((CUnit*)m_pTarget)->Get_UnitCurInfo()->fAttack;
	}

}

void CWeapon_Bow::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	USEGAMEINSTANCE;
	pGameInstance->Add_ColliderGroup(ID_UINT(WEAPON, ENEMY_NORMAL), m_pCol_Sphere);

}

HRESULT CWeapon_Bow::Render()
{
	__super::Render();

#ifdef _DEBUG
	m_pCol_Sphere->Render();
#endif // _DEBUG

	return S_OK;
}

//void CWeapon_Bow::Tick(_float fTimeDelta)
//{
//	if (nullptr == m_pBonePtr)
//		return;
//
//	_matrix		ParentMatrix = m_pBonePtr->Get_OffsetMatrix() * m_pBonePtr->Get_CombinedMatrix() * m_pBonePtr->Get_TransformMatrix();
//	ParentMatrix.r[0] = XMVector3Normalize(ParentMatrix.r[0]);
//	ParentMatrix.r[1] = XMVector3Normalize(ParentMatrix.r[1]);
//	ParentMatrix.r[2] = XMVector3Normalize(ParentMatrix.r[2]);
//	
//	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * ParentMatrix * m_pParentTransformCom->Get_WorldMatrix());
//}

//void CWeapon_Bow::LateTick(_float fTimeDelta)
//{
//	USEGAMEINSTANCE;
//	pGameInstance->Add_RenderGroup(RENDER_NONALPHABLEND, this);
//}

//HRESULT CWeapon_Bow::Render()
//{
//	if (nullptr == m_pShaderCom ||
//		nullptr == m_pModelCom)
//		return E_FAIL;
//
//	/* 셰이더 전역변수에 값을 던진다. */
//	if (FAILED(SetUp_ShaderResource()))
//		return E_FAIL;
//	
//
//	_uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();
//
//	for (_uint i = 0; i < iNumMeshContainers; ++i)
//	{
//		if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
//			return E_FAIL;
//
//		m_pModelCom->Render(i, m_pShaderCom);
//	}	
//
//	return S_OK;
//}

HRESULT CWeapon_Bow::SetUp_Components()
{
	CWeapon::SetUp_Components();
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_ENEMY_WEAPON_BOW), ID_UINT(MODEL_STATIC), (CComponent**)&m_pModelCom));

	//For.Com_Collider
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE), (CComponent**)&m_pCol_Sphere, &ColliderDesc));
	m_pCol_Sphere->Set_Owner(this);


	return S_OK;
}

//HRESULT CWeapon_Bow::SetUp_ShaderResource()
//{
//	if (nullptr == m_pShaderCom)
//		return E_FAIL;
//
//	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
//
//	_float4x4		WorldMatrixTranspose;
//
//	XMStoreFloat4x4(&WorldMatrixTranspose, XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix)));
//
//	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &WorldMatrixTranspose, sizeof(_float4x4))))
//		return E_FAIL;
//	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
//		return E_FAIL;
//	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
//		return E_FAIL;	
//
//
//	RELEASE_INSTANCE(CGameInstance);
//
//	return S_OK;
//}

CWeapon_Bow* CWeapon_Bow::Create()
{
	CWeapon_Bow*		pInstance = new CWeapon_Bow();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWeapon_Bow");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CWeapon_Bow::Clone(void * pArg)
{
	CWeapon_Bow*		pInstance = new CWeapon_Bow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWeapon_Bow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_Bow::Free()
{
	__super::Free();

}
