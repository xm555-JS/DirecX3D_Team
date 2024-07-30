#include "stdafx.h"
#include "..\Public\Weapon_Shield.h"

#include "GameInstance.h"
#include "HIerarchyNode.h"
#include "Unit.h"

CWeapon_Shield::CWeapon_Shield()
	: CWeapon()
{

}

CWeapon_Shield::CWeapon_Shield(const CWeapon_Shield & rhs)
	: CWeapon(rhs)
{

}

HRESULT CWeapon_Shield::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CWeapon_Shield::Initialize(void * pArg)
{
	USEGAMEINSTANCE;

	CTransform::TRANSFORMDESC		TransformDesc;
	FAILEDCHECK(__super::Initialize(&TransformDesc));
	FAILEDCHECK(SetUp_Components());


	m_pTransformCom->Set_Scaled(_float3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-90.0f));
	
	return S_OK;
}

HRESULT CWeapon_Shield::SetUp_Components()
{
	CWeapon::SetUp_Components();
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_WEAPON_SHIELD), ID_UINT(MODEL_STATIC), (CComponent**)&m_pModelCom));

	return S_OK;
}

CWeapon_Shield* CWeapon_Shield::Create()
{
	CWeapon_Shield*		pInstance = new CWeapon_Shield();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWeapon_Shield");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CWeapon_Shield::Clone(void * pArg)
{
	CWeapon_Shield*		pInstance = new CWeapon_Shield(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWeapon_Shield");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_Shield::Free()
{
	__super::Free();

}
