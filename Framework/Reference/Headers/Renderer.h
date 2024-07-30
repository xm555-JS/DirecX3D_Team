#pragma once

/* 화면에 그려져야할 객체들을 그리는 순서대로 모아서 관리하낟.  */
/* 가지고 있는 객체들의 렌더함수를 호출해주낟. */

#include "Component.h"
#include "Delegate.h"

BEGIN(Engine)

class CShader;
class CGameObject;
class CRenderTarget;
class CVIBuffer_Rect;


class ENGINE_DLL CRenderer final : public CComponent
{

private:
	CRenderer();
	virtual ~CRenderer() = default;
	

public:
	list<class CGameObject*>* Get_RenderObjects() { return m_RenderObjects; }



	HRESULT Add_RenderGroup(RENDERGROUP	eGroup, class CGameObject* pGameObject);


	CRenderTarget*				Find_RenderTarget(const _tchar* pTargetTag);
	ID3D11DepthStencilView*		Find_DepthStencilView(const _tchar* pTargetTag);
	D3D11_VIEWPORT				Find_Viewport(const _tchar* pTargetTag);
	list<CRenderTarget*>*		Find_MRT(const _tchar* pMRTTag);
	ID3D11ShaderResourceView*	Get_SRV(const _tchar* pTargetTag);


public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;
	HRESULT					Render_Debug(const _tchar* pMRTTag, CShader* pShader, CVIBuffer_Rect* pVIBuffer);


public:
	void					Creat_DDSFromRT(const _tchar* pMRTTag, const _tchar* pPath);


private:
	list<class CGameObject*>				m_RenderObjects[RENDER_END];
	typedef list<class CGameObject*>		RENDEROBJECTS;
	 
private:
	class CTarget_Manager*					m_pTarget_Manager = nullptr;

	ComPtr<ID3D11DeviceContext>				m_pContext = nullptr;
	ComPtr<ID3D11Device>					m_pDevice = nullptr;

private:
	/* 렌더타겟들을 모아놓는다. */
	map<const _tchar*, CRenderTarget*>			m_RenderTargets;
	typedef map<const _tchar*, CRenderTarget*>	RENDERTARGETS;

	/* 동시에 바인딩되어야하는 렌더타겟들을 그룹지어놓는다. */
	map<const _tchar*, list<CRenderTarget*>>				m_MRTs; 
	typedef map<const _tchar*, list<CRenderTarget*>>		MRTS;


	map<const _tchar*, ID3D11DepthStencilView*>			m_DepthStencilViews;
	map<const _tchar*, D3D11_VIEWPORT>					m_Viewports;


	D3D11_VIEWPORT				m_ViewPortDesc;
	_float						m_fTargetScale = 0.f;

private:
	HRESULT			Add_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);
	HRESULT			Ready_Debug(const _tchar* pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);

	void			Create_DepthStencilView(const _tchar* pTargetTag, _float fTargetScale);
	void			Create_DepthStencilView_DowinScale(const _tchar* pTargetTag, _uint iTargetScale);
	void			Create_Viewport(const _tchar* pTargetTag, _float fTargetScale);
	void			Create_Viewport_DowinScale(const _tchar* pTargetTag, _uint iTargetScale);
	



public:
	static CRenderer* Create();
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END