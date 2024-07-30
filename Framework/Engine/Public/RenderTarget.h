#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CRenderTarget final : public CBase
{
private:
	CRenderTarget();
	virtual ~CRenderTarget() = default;

public:
	ID3D11ShaderResourceView* Get_SRV() const {
		return m_pSRV;
	}

	ID3D11RenderTargetView* Get_RTV() const {
		return m_pRTV;
	}

public:
	/* 1. ID3D11Texture2D ��� ��ü�� �� ����. */
	/* 2. ID3D11RenderTargetView�� ����. */
	/* 3. ID3D11ShaderResourceView�� ����. */
	HRESULT Initialize(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor);
	HRESULT Clear();

#ifdef _DEBUG
	HRESULT Ready_Debug(_float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif // _DEBUG

private:
	ComPtr<ID3D11Device>					m_pDevice = nullptr;
	ComPtr<ID3D11DeviceContext>				m_pContext = nullptr;

	_float4					m_vClearColor;

#ifdef _DEBUG
private:
	_float4x4				m_WorldMatrix;
	
#endif // _DEBUG

private:
	ID3D11Texture2D*					m_pTexture = nullptr;
	/* ����Ÿ������ ���ε� �� �� �ִ� ��ü .*/
	ID3D11RenderTargetView*				m_pRTV = nullptr;

	/* ���̴� ���������� ���ε� �� �� �ִ� ��ü. */
	ID3D11ShaderResourceView*			m_pSRV = nullptr;

public:
	static CRenderTarget* Create(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor);	
	virtual void Free() override;
};

END