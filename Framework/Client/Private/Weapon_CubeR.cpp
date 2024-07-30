#include "stdafx.h"
#include "..\Public\Weapon_CubeR.h"

#include "GameInstance.h"
#include "HIerarchyNode.h"
#include "Unit.h"

CWeapon_CubeR::CWeapon_CubeR()
	: CWeapon()
{

}

CWeapon_CubeR::CWeapon_CubeR(const CWeapon_CubeR & rhs)
	: CWeapon(rhs)
{

}

HRESULT CWeapon_CubeR::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CWeapon_CubeR::Initialize(void * pArg)
{
	USEGAMEINSTANCE;

	CTransform::TRANSFORMDESC		TransformDesc;
	FAILEDCHECK(__super::Initialize(&TransformDesc));
	FAILEDCHECK(SetUp_Components());

	m_pTransformCom->Set_Scaled(_float3(0.01f, 0.01f, 0.01f));
	//m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-90.0f));

	m_vGlowColor = _float4(0.3f, 0.1f, 0.3f, 0.5f);

	return S_OK;
}

HRESULT CWeapon_CubeR::SetUp_Components()
{
	CWeapon::SetUp_Components();
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_WEAPON_CUBE_R), ID_UINT(MODEL_STATIC), (CComponent**)&m_pModelCom));

	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	return S_OK;
}

CWeapon_CubeR* CWeapon_CubeR::Create()
{
	CWeapon_CubeR*		pInstance = new CWeapon_CubeR();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWeapon_CubeR");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CWeapon_CubeR::Clone(void * pArg)
{
	CWeapon_CubeR*		pInstance = new CWeapon_CubeR(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWeapon_CubeR");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_CubeR::Free()
{
	__super::Free();

}
