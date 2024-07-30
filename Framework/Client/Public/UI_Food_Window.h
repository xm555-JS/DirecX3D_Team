#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Food_Window final : public CUI
{
public:
	CUI_Food_Window();
	CUI_Food_Window(const CUI_Food_Window& rhs);
	virtual ~CUI_Food_Window() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	_float m_fAlphaRatio = 0.f;
	_float m_fAccTime = 0.f;

private:
	void Create_FoodInven_Window(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Food_Window* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END