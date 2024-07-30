#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Guide_Window final : public CUI
{
public:
	CUI_Guide_Window();
	CUI_Guide_Window(const CUI_Guide_Window& rhs);
	virtual ~CUI_Guide_Window() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Pooling(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	_float m_fBright = 1.f;
	_float m_fAccTime = 0.f;
	_float m_fDir = 1.f;

	_float m_fInitTime = 0.f;
	_float m_fAlphaRatio = 0.f;

private:
	void SetUp_Bright(_float fTimeDelta);
	void SetUp_Pos(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Guide_Window* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END