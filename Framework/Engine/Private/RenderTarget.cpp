#include "..\Public\RenderTarget.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"
#include "GameInstance.h"

CRenderTarget::CRenderTarget()
{
	USEGAMEINSTANCE;
	m_pDevice = DEVICE;
	m_pContext = CONTEXT;
}

HRESULT CRenderTarget::Initialize(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor)
{
	m_vClearColor = vClearColor;


	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = iWidth;
	TextureDesc.Height = iHeight;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = eFormat;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pTexture)))
		__debugbreak();

	if (FAILED(m_pDevice->CreateRenderTargetView(m_pTexture, nullptr, &m_pRTV)))
		__debugbreak();

	if (FAILED(m_pDevice->CreateShaderResourceView(m_pTexture, nullptr, &m_pSRV)))
		__debugbreak();

	return S_OK;
}

HRESULT CRenderTarget::Clear()
{
	/*if (m_vClearColor.x == 1.f)
		DirectX::SaveDDSTextureToFile(m_pContext, m_pTexture, TEXT("../Bin/Diffuse.dds"));
	if (m_vClearColor.y == 1.f)
		DirectX::SaveDDSTextureToFile(m_pContext, m_pTexture, TEXT("../Bin/Normal.dds"));

*/
	m_pContext->ClearRenderTargetView(m_pRTV, (_float*)&m_vClearColor);


	return S_OK;
}

#ifdef _DEBUG

HRESULT CRenderTarget::Ready_Debug(_float fX, _float fY, _float fSizeX, _float fSizeY)
{

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));

	_uint		iNumViewports = 1;

	m_pContext->RSGetViewports(&iNumViewports, &ViewPortDesc);

	_matrix		WorldMatrix = XMMatrixIdentity();	

	WorldMatrix.r[0] = XMVectorSet(fSizeX, 0.f, 0.f, 0.f);
	WorldMatrix.r[1] = XMVectorSet(0.f, fSizeY, 0.f, 0.f);
	WorldMatrix.r[3] = XMVectorSet(fX - (ViewPortDesc.Width * 0.5f), -fY + (ViewPortDesc.Height * 0.5f), 0.f, 1.f);

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixTranspose(WorldMatrix));

	return S_OK;
}

HRESULT CRenderTarget::Render_Debug(CShader * pShader, CVIBuffer_Rect * pVIBuffer)
{
	/* ���������� ���� ������ ������. */
	if (FAILED(pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4))))
		__debugbreak();

	if (FAILED(pShader->Set_ShaderResourceView("g_Texture", m_pSRV)))
		__debugbreak();

	if (FAILED(pShader->Begin(0)))
		__debugbreak();

	return pVIBuffer->Render();
}

#endif // _DEBUG

CRenderTarget * CRenderTarget::Create(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor)
{
	CRenderTarget*		pInstance = new CRenderTarget();

	if (FAILED(pInstance->Initialize(iWidth, iHeight, eFormat, vClearColor)))
	{
		MSG_BOX("Failed to Created : CRenderTarget");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRenderTarget::Free()
{
	Safe_Release(m_pSRV);
	Safe_Release(m_pRTV);

	Safe_Release(m_pTexture);
}
