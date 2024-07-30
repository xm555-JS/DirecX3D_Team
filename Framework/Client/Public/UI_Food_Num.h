#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Food_Num final : public CUI
{
public:
	CUI_Food_Num();
	CUI_Food_Num(const CUI_Food_Num& rhs);
	virtual ~CUI_Food_Num() = default;

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
	_float m_fAlphaRatio = 0.f;
	_float m_fAccTime = 0.f;
	_bool m_bRender = false;

private:
	void Resize_FoodNum(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Food_Num* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END