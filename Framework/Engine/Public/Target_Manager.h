#pragma once

/* 렌더타겟들을 모아놓는다. */
#include "Base.h"

BEGIN(Engine)

class CRenderTarget;
class CShader;
class CVIBuffer_Rect;


class CTarget_Manager final : public CBase
{
	DECLARE_SINGLETON(CTarget_Manager)
public:
	CTarget_Manager();
	virtual ~CTarget_Manager() = default;

public:
	//ID3D11ShaderResourceView* Get_SRV(const _tchar* pTargetTag);

public:
	HRESULT Initialize();

	//HRESULT			Add_RenderTarget(const _tchar* pTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor);
	//HRESULT			Add_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);
	CRenderTarget*	Create_RenderTarget(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor);

	HRESULT Begin_MRT(list<CRenderTarget*>* pMRTList, ID3D11DepthStencilView* pDepthStencilView = nullptr);
	HRESULT BeginOverwrite_MRT(list<CRenderTarget*>* pMRTList, ID3D11DepthStencilView* pDepthStencilView = nullptr);
	//HRESULT Begin_MRT(const _tchar* pMRTTag, ID3D11DepthStencilView* pDepthStencilView = nullptr);
	HRESULT End_MRT();

#ifdef _DEBUG
public:
	//HRESULT Ready_Debug(const _tchar* pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	//HRESULT Render_Debug(list<CRenderTarget*>* pMRTList, CShader* pShader, CVIBuffer_Rect* pVIBuffer);
	//HRESULT Render_Debug(const _tchar* pMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif // _DEBUG

//private:
//	/* 렌더타겟들을 모아놓는다. */
//	map<const _tchar*, CRenderTarget*>			m_RenderTargets;
//	typedef map<const _tchar*, CRenderTarget*>	RENDERTARGETS;
//
//	/* 동시에 바인딩되어야하는 렌더타겟들을 그룹지어놓는다. */
//	map<const _tchar*, list<CRenderTarget*>>				m_MRTs;
//	typedef map<const _tchar*, list<CRenderTarget*>>		MRTS;

private:
	ID3D11RenderTargetView*					m_pBackBufferView = nullptr;
	ID3D11DepthStencilView*					m_pDepthStencilView = nullptr;

	ComPtr<ID3D11Device>					m_pDevice = nullptr;
	ComPtr<ID3D11DeviceContext>				m_pContext = nullptr;

public:
	//CRenderTarget* Find_RenderTarget(const _tchar* pTargetTag);
	//list<CRenderTarget*>* Find_MRT(const _tchar* pMRTTag);


public:
	virtual void Free();
};

END