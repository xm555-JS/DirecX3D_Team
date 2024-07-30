#pragma once

#include "Base.h"

BEGIN(Engine)

class CLight;

class CLight_Manager final : public CBase
{
	DECLARE_SINGLETON(CLight_Manager)
private:
	CLight_Manager();
	virtual ~CLight_Manager() = default;

public:
	const LIGHTDESC* Get_LightDesc(_uint iIndex);
	void	Get_RenderLights(_fvector vPosition, list<CLight*>* pLightList);
	const list<CLight*>* Get_RenderLights() { return &m_RenderLights; }
	const list<CLight*>* Get_ShadowLights() { return &m_ShadowLights; }
	_uint			Get_LightNum(_fvector vPosition);
	CLight*			Get_Sun() { return m_pSun; }

	void			Set_Sun(CLight* pLight) { m_pSun = pLight; }

public:
	CLight* Add_Light(const LIGHTDESC& LightDesc);
	CLight*	Add_Light(CLight* pLight);
	void	Add_RenderLightGroup(CLight* pLight) { m_RenderLights.push_back(pLight); }
	void	Add_ShadowLightGroup(CLight* pLight) { m_ShadowLights.push_back(pLight); }
	void	Remove_RenderLightGroup(CLight* pLight) { m_RenderLights.remove(pLight); }
	void	Remove_ShadowLightGroup(CLight* pLight) { m_ShadowLights.remove(pLight); }
	//void	Remove_Light(CLight* pLight);

public:
	HRESULT	Initialize();
	void	Tick(_float fTimeDelta);
	void	LateTick(_float fTimeDelta);
	HRESULT Render_Lights(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
	HRESULT Render_AllLights(_vector vPosition, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
	HRESULT Render_Shadows(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
	HRESULT Render_AllShadows(_vector vPosition, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
	virtual void	On_EventMessege(EVENTDESC tEvent);


public:
	void	Clear();

private:
	list<CLight*>				m_Lights;
	list<CLight*>				m_RenderLights;
	list<CLight*>				m_ShadowLights;

	CLight*						m_pSun = nullptr;


	typedef list<CLight*>		LIGHTS;

public:
	virtual void Free() override;
};

END