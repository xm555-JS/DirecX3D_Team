#include "stdafx.h"
#include "..\Public\Weapon_Hammer.h"

#include "GameInstance.h"
#include "HIerarchyNode.h"
#include "Unit.h"

CWeapon_Hammer::CWeapon_Hammer()
	: CWeapon()
{

}

CWeapon_Hammer::CWeapon_Hammer(const CWeapon_Hammer & rhs)
	: CWeapon(rhs)
{

}

HRESULT CWeapon_Hammer::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CWeapon_Hammer::Initialize(void * pArg)
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
	m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(-90.f));
	
	return S_OK;
}

void CWeapon_Hammer::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_ColMatrix = XMLoadFloat4x4(&m_WorldMatrix);
	m_ColMatrix.r[0] *= 400.f;
	m_ColMatrix.r[1] *= 400.f;
	m_ColMatrix.r[2] *= 400.f;

	m_pCol_Sphere->Update(m_ColMatrix);

}

void CWeapon_Hammer::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	USEGAMEINSTANCE;
	pGameInstance->Add_ColliderGroup(ID_UINT(COL_ENEMY_ATTACK), m_pCol_Sphere);

}

HRESULT CWeapon_Hammer::Render()
{
	__super::Render();

#ifdef _DEBUG
	m_pCol_Sphere->Render();
#endif // _DEBUG

	return S_OK;
}



HRESULT CWeapon_Hammer::SetUp_Components()
{
	CWeapon::SetUp_Components();
	/* For.Com_Shader */
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(MODEL_STATIC, MODEL_WEAPON, MODEL_ENEMY_WEAPON_HAMMER), ID_UINT(MODEL_STATIC), (CComponent**)&m_pModelCom));

	//For.Com_Collider
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float3(0.f, 0.5f, 0.f);
	FAILEDCHECK(__super::Add_Component(LEVEL_STATIC, ID_UINT(COLLIDER, COLLIDER_SPHERE), ID_UINT(COLLIDER, COLLIDER_SPHERE, COL_ENEMY_ATTACK), (CComponent**)&m_pCol_Sphere, &ColliderDesc));
	m_pCol_Sphere->Set_Owner(this);


	return S_OK;
}


CWeapon_Hammer* CWeapon_Hammer::Create()
{
	CWeapon_Hammer*		pInstance = new CWeapon_Hammer();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWeapon_Hammer");		
		Safe_Release(pInstance);
	}

	return pInstance; 
}

CGameObject * CWeapon_Hammer::Clone(void * pArg)
{
	CWeapon_Hammer*		pInstance = new CWeapon_Hammer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWeapon_Hammer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_Hammer::Free()
{
	__super::Free();

}
