#include "stdafx.h"
#include "..\Public\UI_Solid.h"

#include "GameInstance.h"

CUI_Solid::CUI_Solid()
	: CGameObject()
{
}

CUI_Solid::CUI_Solid(const CUI_Solid & rhs)
	: CGameObject(rhs)
{
}

HRESULT CUI_Solid::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Solid::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	if (FAILED(__super::Initialize(&TransformDesc)))
		__debugbreak();


	return S_OK;
}

void CUI_Solid::Tick(_float fTimeDelta)
{

}

HRESULT CUI_Solid::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		__debugbreak();

	/* 셰이더 전역변수에 값을 던진다. */
	FAILEDCHECK(SetUp_ShaderResource());

	m_pShaderCom->Begin(m_iPassIdx);

	m_pVIBufferCom->Render();

	return S_OK;
}

void CUI_Solid::Billboard()
{
	USEGAMEINSTANCE;

	_float4x4	ViewMatrixInv;
	XMStoreFloat4x4(&ViewMatrixInv, XMMatrixInverse(nullptr, XMLoadFloat4x4(pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))));

	_float3 vScale = m_pTransformCom->Get_Scaled();

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMLoadFloat4((_float4*)&ViewMatrixInv.m[0][0]));
	m_pTransformCom->Set_State(CTransform::STATE_UP, XMLoadFloat4((_float4*)&ViewMatrixInv.m[1][0]));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMLoadFloat4((_float4*)&ViewMatrixInv.m[2][0]));

	m_pTransformCom->Set_Scaled(vScale);
}

HRESULT CUI_Solid::SetUp_ShaderResource()
{
	NULLCHECK(m_pShaderCom);
	USEGAMEINSTANCE;
	FAILEDCHECK(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4)));
	FAILEDCHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", pGameInstance->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4)));
	FAILEDCHECK(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", m_tUISolidDesc.iTextureIdx));

	return S_OK;
}

void CUI_Solid::Free()
{
	__super::Free();
}
