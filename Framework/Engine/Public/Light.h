#pragma once

#include "Base.h"

BEGIN(Engine)

class CLight_Manager;
class CRenderTarget;
class CGameObject;


class ENGINE_DLL CLight : public CBase
{
protected:
	CLight();
	virtual ~CLight() = default;

public:
	const LIGHTDESC*		Get_LightDesc() const { return &m_LightDesc; }
	CRenderTarget*			Get_RendetTarget() { return m_pLightDepth; }
	vector<CGameObject*>*	Get_RenderObjects() { return &m_RenderObject; }
	CGameObject*			Get_LookAtObject() { return m_pLookAtObject; }
	_matrix					Get_ViewMatrix() { return XMLoadFloat4x4(&m_ViewMatrix); }
	_matrix					Get_ProjMatrix() { return XMLoadFloat4x4(&m_ProjMatrix); }


	void					Set_Position(_fvector vPos) { 
		XMStoreFloat4(&m_LightDesc.vPosition, vPos);
		m_LightDesc.vPosition.w = 1.f;
	}
	void	Set_LookAtObject(CGameObject* pObejct) { m_pLookAtObject = pObejct; }
	void	Set_UseForShadow(_bool bState) { m_bUseForShadow = bState; }
	void	Set_ProjMatrix(_matrix ProjMatrix) { XMStoreFloat4x4(&m_ProjMatrix, ProjMatrix); }
	void	Set_Far(_float fFar) { m_fFar = fFar; }

	void	Add_RenderObject(CGameObject* pObject) { m_RenderObject.push_back(pObject); }


public:
	virtual HRESULT Initialize(const LIGHTDESC& LightDesc);
	virtual void	Bind_PipeLine();
	virtual void	Tick(_float fTimeDelta);
	virtual void	LateTick(_float fTimeDelta);
	virtual HRESULT Render(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
	virtual HRESULT Render_Shadows(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
	virtual HRESULT Render_Depth(class CShader* pShader, _uint iPass);

#ifdef _DEBUG
	HRESULT Render_Debug(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif // _DEBUG


public:
	_bool	Is_RangeIn(_vector vPosition);

public:
	virtual HRESULT	SetUp_DepthComponents(CShader* pShader);

protected:
	LIGHTDESC					m_LightDesc;
	CRenderTarget*				m_pLightDepth = nullptr;
	CLight_Manager*				m_pLightManager = nullptr;

	_bool						m_bRangeIn = false;

	_float4x4					m_WorldMatrix;
	_float4x4					m_ViewMatrix;
	_float4x4					m_ProjMatrix;

	ComPtr<ID3D11Device>			m_pDevice = nullptr;
	ComPtr<ID3D11DeviceContext>		m_pContext = nullptr;



	vector<CGameObject*>		m_RenderObject;

protected:
	_uint						m_iDepthTargetCX = 0;
	_uint						m_iDepthTargetCY = 0;

	D3D11_VIEWPORT				m_pOldViewport[1];
	ID3D11DepthStencilView*		m_pDepthStencilView = nullptr;
	D3D11_VIEWPORT				m_ViewPortDesc;


protected:
	CGameObject*				m_pLookAtObject = nullptr;
	_bool						m_bUseForShadow = false;

	_float						m_fFar = 0.f;

protected:
	void	Create_Viewport(); // 지금은 안쓰지만 만약 그림자를 더 자세하게 그릴떄 사용
	void	Create_DepthStencil();
	virtual void	Setup_ViewPort();

public:
	static CLight* Create(const LIGHTDESC& LightDesc);
	virtual void Free() override;
};

END