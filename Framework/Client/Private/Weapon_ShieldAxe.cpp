#include "stdafx.h"
#include "..\Public\Weapon_ShieldAxe.h"

#include "GameInstance.h"
#include "HIerarchyNode.h"
#include "Unit.h"

CWeapon_ShieldAxe::CWeapon_ShieldAxe()
	: CWeapon()
{

}

CWeapon_ShieldAxe::CWeapon_ShieldAxe(const CWeapon_ShieldAxe & rhs)
	: CWeapon(rhs)
{

}

void CWeapon_ShieldAxe::Set_ChangeWeapon()
{
	if (m_pModelCom == m_pModel[0])
		m_pModelCom = m_pModel[1];
	else
		m_pModelCom = m_pModel[0];

	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();
}

_uint CWeapon_ShieldAxe::Get_WeaponType()
{
	if (m_pModelCom == m_pModel[0])
		return 0;
	else
		return 1;
}

void CWeapon_ShieldAxe::Reset_Weapon()
{
	m_pModelCom = m_pModel[0];

	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();
}

HRESULT CWeapon_ShieldAxe::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CWeapon_ShieldAxe::Initialize(void * pArg)
{
	USEGAMEINSTANCE;

	CTransform::TRANSFORMDESC		TransformDesc;
	FAILEDCHECK(__super::Initialize(&TransformDesc));
	FAILEDCHECK(SetUp_Components());


	m_pTransformCom->Set_Scaled(_float3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-90.0f));

	m_vGlowColor = _float4(0.5f, 0.f, 0.f, 0.5f);

	return S_OK;
}

HRESULT CWeapon_ShieldAxe::SetUp_Components()
{
	CWeapon::SetUp_Components();
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_WEAPON_SHIELD), ID_UINT(MODEL_STATIC), (CComponent**)&m_pModel[0]));

	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_WEAPON_AXE), ID_UINT(MODEL_STATIC, 1), (CComponent**)&m_pModel[1]));

	m_pModelCom = m_pModel[0];

	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	return S_OK;
}

CWeapon_ShieldAxe* CWeapon_ShieldAxe::Create()
{
	CWeapon_ShieldAxe*		pInstance = new CWeapon_ShieldAxe();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWeapon_ShieldAxe");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CWeapon_ShieldAxe::Clone(void * pArg)
{
	CWeapon_ShieldAxe*		pInstance = new CWeapon_ShieldAxe(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWeapon_ShieldAxe");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_ShieldAxe::Free()
{
	__super::Free();

}
