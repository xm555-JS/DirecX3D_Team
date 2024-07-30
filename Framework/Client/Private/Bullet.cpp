#include "stdafx.h"

#include "..\Public\Bullet.h"

#include "GameInstance.h"
#include "Navigation.h"

CBullet::CBullet()
	: CGameObject()
{
}

CBullet::CBullet(const CBullet& rhs)
	: CGameObject(rhs)
{
}

HRESULT CBullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBullet::Initialize(void* pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	FAILEDCHECK(__super::Initialize(&TransformDesc));

	return S_OK;
}

void CBullet::Tick(_float fTimeDelta)
{
}

void CBullet::LateTick(_float fTimeDelta)
{
}

HRESULT CBullet::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	_uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	/* 셰이더 전역변수에 값을 던진다. */
	FAILEDCHECK(SetUp_ShaderResource());

	m_pShaderCom->Begin(0);

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE));
		FAILEDCHECK(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS));

		m_pModelCom->Render(i, m_pShaderCom);
	}

#ifdef _DEBUG
	
	if(nullptr != m_pCol_Sphere)
		m_pCol_Sphere->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CBullet::SetUp_Components()
{
	/* For.Com_Navigation */
	CNavigation::NAVIDESC		NaviDesc;
	NaviDesc.m_iCurrentIndex = 0;
	NaviDesc.pOwner = this;
	FAILEDCHECK(__super::Add_Component(m_iCurLevel, ID_UINT(NAVIGATION), ID_UINT(NAVIGATION), (CComponent**)&m_pNaviCom, &NaviDesc));
	m_pNaviCom->Find_CurrentIndex(m_pTransformCom);

	return S_OK;
}

HRESULT CBullet::SetUp_ShaderResource()
{
	NULLCHECK(m_pShaderCom);

	USEGAMEINSTANCE;

	FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));

	return S_OK;
}

void CBullet::Free()
{
	__super::Free();
}
