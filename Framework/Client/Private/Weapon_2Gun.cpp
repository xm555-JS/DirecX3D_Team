#include "stdafx.h"
#include "..\Public\Weapon_2Gun.h"

#include "GameInstance.h"
#include "HIerarchyNode.h"
#include "Unit.h"

CWeapon_2Gun::CWeapon_2Gun()
	: CWeapon()
{

}

CWeapon_2Gun::CWeapon_2Gun(const CWeapon_2Gun & rhs)
	: CWeapon(rhs)
{

}

HRESULT CWeapon_2Gun::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CWeapon_2Gun::Initialize(void * pArg)
{
	USEGAMEINSTANCE;

	CTransform::TRANSFORMDESC		TransformDesc;
	FAILEDCHECK(__super::Initialize(&TransformDesc));
	FAILEDCHECK(SetUp_Components());


	m_pTransformCom->Set_Scaled(_float3(0.0045f, 0.0045f, 0.0045f));

	if (m_bLeft)
	{
		m_pTransformCom->RotationAngles(XMVectorSet(0.f, XMConvertToRadians(-90.0f), XMConvertToRadians(90.0f), 0.f));
	}
	else
		m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-90.0f));

	m_vGlowColor = _float4(0.3f, 0.1f, 0.3f, 0.5f);

	return S_OK;
}

HRESULT CWeapon_2Gun::SetUp_Components()
{
	CWeapon::SetUp_Components();
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_WEAPON_2GUN), ID_UINT(MODEL_STATIC), (CComponent**)&m_pModelCom));

	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	return S_OK;
}

CWeapon_2Gun* CWeapon_2Gun::Create()
{
	CWeapon_2Gun*		pInstance = new CWeapon_2Gun();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWeapon_2Gun");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CWeapon_2Gun::Clone(void * pArg)
{
	CWeapon_2Gun*		pInstance = new CWeapon_2Gun(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWeapon_2Gun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_2Gun::Free()
{
	__super::Free();

}
