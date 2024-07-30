#include "stdafx.h"
#include "..\Public\Mobility_Glider.h"

#include "GameInstance.h"
#include "HIerarchyNode.h"
#include "Unit.h"
#include "Imgui_Manager.h"

CMobility_Glider::CMobility_Glider()
	: CWeapon()
{

}

CMobility_Glider::CMobility_Glider(const CMobility_Glider & rhs)
	: CWeapon(rhs)
{

}

HRESULT CMobility_Glider::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CMobility_Glider::Initialize(void * pArg)
{
	USEGAMEINSTANCE;

	CTransform::TRANSFORMDESC		TransformDesc;
	FAILEDCHECK(__super::Initialize(&TransformDesc));
	FAILEDCHECK(SetUp_Components());

	m_pTransformCom->Set_Scaled(_float3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(-60.f));

	m_vGlowColor = _float4(0.3f, 0.1f, 0.3f, 0.5f);

	return S_OK;
}
HRESULT CMobility_Glider::SetUp_Components()
{
	CWeapon::SetUp_Components();
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_MOBILITY_GLIDER), ID_UINT(MODEL_STATIC), (CComponent**)&m_pModelCom));

	m_iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	return S_OK;
}

CMobility_Glider* CMobility_Glider::Create()
{
	CMobility_Glider*		pInstance = new CMobility_Glider();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMobility_Glider");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CMobility_Glider::Clone(void * pArg)
{
	CMobility_Glider*		pInstance = new CMobility_Glider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMobility_Glider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMobility_Glider::Free()
{
	__super::Free();

}
