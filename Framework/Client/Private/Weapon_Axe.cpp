#include "stdafx.h"
#include "..\Public\Weapon_Axe.h"

#include "GameInstance.h"
#include "HIerarchyNode.h"
#include "Unit.h"

CWeapon_Axe::CWeapon_Axe()
	: CWeapon()
{

}

CWeapon_Axe::CWeapon_Axe(const CWeapon_Axe & rhs)
	: CWeapon(rhs)
{

}

HRESULT CWeapon_Axe::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CWeapon_Axe::Initialize(void * pArg)
{
	USEGAMEINSTANCE;

	CTransform::TRANSFORMDESC		TransformDesc;
	FAILEDCHECK(__super::Initialize(&TransformDesc));
	FAILEDCHECK(SetUp_Components());


	m_pTransformCom->Set_Scaled(_float3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-90.0f));
	
	return S_OK;
}

HRESULT CWeapon_Axe::SetUp_Components()
{
	CWeapon::SetUp_Components();
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_WEAPON_AXE), ID_UINT(MODEL_STATIC), (CComponent**)&m_pModelCom));

	return S_OK;
}

CWeapon_Axe* CWeapon_Axe::Create()
{
	CWeapon_Axe*		pInstance = new CWeapon_Axe();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWeapon_Axe");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CWeapon_Axe::Clone(void * pArg)
{
	CWeapon_Axe*		pInstance = new CWeapon_Axe(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWeapon_Axe");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_Axe::Free()
{
	__super::Free();

}
