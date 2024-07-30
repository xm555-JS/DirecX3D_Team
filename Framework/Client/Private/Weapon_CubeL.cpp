#include "stdafx.h"
#include "..\Public\Weapon_CubeL.h"

#include "GameInstance.h"
#include "HIerarchyNode.h"
#include "Unit.h"

CWeapon_CubeL::CWeapon_CubeL()
	: CWeapon()
{

}

CWeapon_CubeL::CWeapon_CubeL(const CWeapon_CubeL & rhs)
	: CWeapon(rhs)
{

}

HRESULT CWeapon_CubeL::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CWeapon_CubeL::Initialize(void * pArg)
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

HRESULT CWeapon_CubeL::SetUp_Components()
{
	CWeapon::SetUp_Components();
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_WEAPON_CUBE_L), ID_UINT(MODEL_STATIC), (CComponent**)&m_pModelCom));

	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	return S_OK;
}

CWeapon_CubeL* CWeapon_CubeL::Create()
{
	CWeapon_CubeL*		pInstance = new CWeapon_CubeL();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWeapon_CubeL");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CWeapon_CubeL::Clone(void * pArg)
{
	CWeapon_CubeL*		pInstance = new CWeapon_CubeL(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWeapon_CubeL");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_CubeL::Free()
{
	__super::Free();

}
