#pragma once

#include "Base.h"

BEGIN(Engine)

class CGraphic_Device final : public CBase
{
	DECLARE_SINGLETON(CGraphic_Device)

public:
	CGraphic_Device();
	virtual ~CGraphic_Device() = default;

public:
	ComPtr<ID3D11Device>			Get_Device() { return m_pDevice; }
	ComPtr<ID3D11DeviceContext>		Get_Context() { return m_pDeviceContext; }
	ID3D11ShaderResourceView*		Get_BackBufferSRV() const { return m_pBackBufferSRV; }
	ID3D11RenderTargetView*			Get_BackBufferRTV() { return m_pBackBufferRTV; }
	IDXGISwapChain*					Get_SwapChain() { return m_pSwapChain; }

public:
	HRESULT Ready_Graphic_Device(HWND hWnd, GRAPHICDESC::WINMODE WinMode, _uint iWinCX, _uint iWinCY);
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_DepthStencil_View();
	HRESULT Present();
private:	

	/* �޸� �Ҵ�. (��������, �ε�������, �ؽ��ķε�) */
	ComPtr<ID3D11Device>		m_pDevice = nullptr;

	/* ���ε��۾�. ��ɽ��� (�������۸� SetStreamSource(), SetIndices(), SetTransform(), SetTexture() */
	ComPtr<ID3D11DeviceContext>	m_pDeviceContext = nullptr;

	IDXGISwapChain*			m_pSwapChain = nullptr;	

	ID3D11RenderTargetView*			m_pBackBufferRTV = nullptr;
	ID3D11DepthStencilView*			m_pDepthStencilView = nullptr;
	ID3D11ShaderResourceView*		m_pBackBufferSRV = nullptr;

private:
	HRESULT Ready_SwapChain(HWND hWnd, GRAPHICDESC::WINMODE WinMode, _uint iWinCX, _uint iWinCY);
	HRESULT Ready_BackBufferRenderTargetView();
	HRESULT Ready_DepthStencilRenderTargetView(_uint iWinCX, _uint iWinCY);

public:
	virtual void Free() override;
};

END