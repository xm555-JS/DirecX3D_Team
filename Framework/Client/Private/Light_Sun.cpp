#include "..\Public\Light_Sun.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"
#include "GameInstance.h"
#include "RenderTarget.h"
#include "Light_Manager.h"
#include "GameObject.h"

#include "Effect_Manager.h"

CLight_Sun::CLight_Sun()
{
}

HRESULT CLight_Sun::Initialize(const LIGHTDESC& LightDesc)
{
	FAILEDCHECK(CLight::Initialize(LightDesc));
	m_bRangeIn = true;
	m_pLightManager->Add_RenderLightGroup(this);
	m_pLightManager->Add_ShadowLightGroup(this);

	return S_OK;
}

HRESULT CLight_Sun::Render(CShader * pShader, CVIBuffer_Rect * pVIBuffer)
{
	if (true == m_bUseForShadow		)
		return S_OK;

	FAILEDCHECK(pShader->Set_RawValue("g_vLightPos", &m_LightDesc.vPosition, sizeof(_float4)));
	FAILEDCHECK(pShader->Set_RawValue("g_fRange", &m_LightDesc.fRange, sizeof(_float)));

	FAILEDCHECK(pShader->Set_RawValue("g_vLightDiffuse", &m_LightDesc.vDiffuse, sizeof(_float4)));
	FAILEDCHECK(pShader->Set_RawValue("g_vLightAmbient", &m_LightDesc.vAmbient, sizeof(_float4)));
	FAILEDCHECK(pShader->Set_RawValue("g_vLightSpecular", &m_LightDesc.vSpecular, sizeof(_float4)));


	pShader->Begin(1);
	pVIBuffer->Render();

	return S_OK;
}


HRESULT CLight_Sun::Render_Shadows(CShader * pShader, CVIBuffer_Rect * pVIBuffer)
{
	_float			fPower = 3.f;
	FAILEDCHECK(pShader->Set_RawValue("g_fPower", &fPower, sizeof(_float)));
	FAILEDCHECK(pShader->Set_RawValue("g_fRange", &m_LightDesc.fRange, sizeof(_float)));
	FAILEDCHECK(pShader->Set_RawValue("g_fFar", &m_fFar, sizeof(_float)));

	_float4x4	ViewMatrix, ProjMatrix;
	XMStoreFloat4x4(&ViewMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_ViewMatrix)));
	XMStoreFloat4x4(&ProjMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_ProjMatrix)));

	FAILEDCHECK(pShader->Set_RawValue("g_LightViewMatrix", &ViewMatrix, sizeof(_float4x4)));
	FAILEDCHECK(pShader->Set_RawValue("g_LightProjMatrix", &ProjMatrix, sizeof(_float4x4)));
	FAILEDCHECK(pShader->Set_ShaderResourceView("g_LightDepthTexture", m_pLightDepth->Get_SRV()));

	pShader->Begin(0);
	pVIBuffer->Render();

	//m_pLightDepth->Render_Debug(pShader, pVIBuffer);

	return S_OK;
}

CLight_Sun * CLight_Sun::Create(const LIGHTDESC & LightDesc)
{
	CLight_Sun*		pInstance = new CLight_Sun();

	if (FAILED(pInstance->Initialize(LightDesc)))
	{
		MSG_BOX("Failed to Created : CLight_Sun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLight_Sun::Free()
{
	__super::Free();
}
