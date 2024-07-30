#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_CurFood_Num final : public CUI
{
public:
	CUI_CurFood_Num();
	CUI_CurFood_Num(const CUI_CurFood_Num& rhs);
	virtual ~CUI_CurFood_Num() = default;

public:
	void  Set_Render(_bool bRender) { m_bRender = bRender; }
	_bool Get_Render() { return m_bRender; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	_bool m_bRender = false;

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_CurFood_Num* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END