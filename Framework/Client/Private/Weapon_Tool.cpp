#include "stdafx.h"
#include "..\Public\Weapon_Tool.h"

#include "GameInstance.h"
#include "HIerarchyNode.h"


CWeapon_Tool::CWeapon_Tool()
	: CWeapon()
{

}

CWeapon_Tool::CWeapon_Tool(const CWeapon_Tool & rhs)
	: CWeapon(rhs)
{

}

HRESULT CWeapon_Tool::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CWeapon_Tool::Initialize(void * pArg)
{
	USEGAMEINSTANCE;

	CTransform::TRANSFORMDESC		TransformDesc;
	FAILEDCHECK(__super::Initialize(&TransformDesc));

	if (nullptr != pArg)
	{
		memcpy(&m_iModelID, pArg, sizeof(_uint));
	}

	FAILEDCHECK(SetUp_Components());


	m_pTransformCom->Set_Scaled(_float3(0.01f, 0.01f, 0.01f));
	//m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-90.0f));

	pGameInstance->Add_TickGroup(TG_WEAPON, this);
	pGameInstance->Add_LateTickGroup(LTG_WEAPON, this);

	return S_OK;
}

//void CWeapon_Tool::Tick(_float fTimeDelta)
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

//void CWeapon_Tool::LateTick(_float fTimeDelta)
//{
//	USEGAMEINSTANCE;
//	pGameInstance->Add_RenderGroup(RENDER_NONALPHABLEND, this);
//}

//HRESULT CWeapon_Tool::Render()
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

HRESULT CWeapon_Tool::SetUp_Components()
{
	CWeapon::SetUp_Components();

	ID_UINT ModelID;

	ModelID = m_iModelID;
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ModelID, ID_UINT(MODEL_STATIC), (CComponent**)&m_pModelCom));

	return S_OK;
}

//HRESULT CWeapon_Tool::SetUp_ShaderResource()
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

CWeapon_Tool* CWeapon_Tool::Create()
{
	CWeapon_Tool*		pInstance = new CWeapon_Tool();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWeapon_Tool");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CWeapon_Tool::Clone(void * pArg)
{
	CWeapon_Tool*		pInstance = new CWeapon_Tool(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWeapon_Tool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_Tool::Free()
{
	__super::Free();

}
