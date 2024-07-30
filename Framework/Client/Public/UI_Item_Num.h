#pragma once

#include "Client_Defines.h"
#include "UI_Inventory.h"

BEGIN(Client)

class CUI_Item_Num final : public CUI_Inventory
{
public:
	CUI_Item_Num();
	CUI_Item_Num(const CUI_Item_Num& rhs);
	virtual ~CUI_Item_Num() = default;

public:
	void  Set_Render(_bool bRender) { m_bRender = bRender; }
	_bool Get_Render() { return m_bRender; }

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
	_bool m_bRender = false;

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Item_Num* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END