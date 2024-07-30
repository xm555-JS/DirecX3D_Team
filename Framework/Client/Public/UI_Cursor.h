#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Cursor final : public CUI
{
private:
	CUI_Cursor();
	CUI_Cursor(const CUI_Cursor& rhs);
	virtual ~CUI_Cursor() = default;

public:
	void Set_Render(_bool bRender) { m_bRender = bRender; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	_bool	m_bRender = false;

private:
	virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

	
public:
	static CUI_Cursor* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END