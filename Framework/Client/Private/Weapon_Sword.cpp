#include "stdafx.h"
#include "..\Public\Weapon_Sword.h"

#include "GameInstance.h"
#include "HIerarchyNode.h"
#include "Unit.h"

CWeapon_Sword::CWeapon_Sword()
	: CWeapon()
{

}

CWeapon_Sword::CWeapon_Sword(const CWeapon_Sword & rhs)
	: CWeapon(rhs)
{

}

HRESULT CWeapon_Sword::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CWeapon_Sword::Initialize(void * pArg)
{
	USEGAMEINSTANCE;

	CTransform::TRANSFORMDESC		TransformDesc;
	FAILEDCHECK(__super::Initialize(&TransformDesc));
	FAILEDCHECK(SetUp_Components());


	m_pTransformCom->Set_Scaled(_float3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-90.0f));

	m_vGlowColor = _float4(0.3f, 0.1f, 0.3f, 0.5f);

	return S_OK;
}

HRESULT CWeapon_Sword::SetUp_Components()
{
	CWeapon::SetUp_Components();
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_WEAPON_SWORD), ID_UINT(MODEL_STATIC), (CComponent**)&m_pModelCom));

	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	return S_OK;
}

CWeapon_Sword* CWeapon_Sword::Create()
{
	CWeapon_Sword*		pInstance = new CWeapon_Sword();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWeapon_Sword");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CWeapon_Sword::Clone(void * pArg)
{
	CWeapon_Sword*		pInstance = new CWeapon_Sword(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWeapon_Sword");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_Sword::Free()
{
	__super::Free();

}
