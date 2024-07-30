#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Menu_Back final : public CUI
{
public:
	CUI_Menu_Back();
	CUI_Menu_Back(const CUI_Menu_Back& rhs);
	virtual ~CUI_Menu_Back() = default;

public:
	_bool	Get_MenuActive() { return m_bActive; }

	void	Set_MenuActive(_bool _bActive) { m_bActive = _bActive; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	_bool	m_bRender = false;
	_bool	m_bActive = false;
	_float	m_fAlphaRatio = 0.f;
	_float	m_fAccTime = 0.f;

private:
	void SetActive_MenuBack(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Menu_Back* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END