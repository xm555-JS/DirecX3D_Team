#pragma once

#include "Client_Defines.h"
#include "UI_Cook_Window.h"

BEGIN(Client)

class CUI_Cook_Icon final : public CUI_Cook_WIndow
{
public:
	CUI_Cook_Icon();
	CUI_Cook_Icon(const CUI_Cook_Icon& rhs);
	virtual ~CUI_Cook_Icon() = default;

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
	void Create_Window_Edge(_float fTimeDelta);
	void Create_Foodlist_Window(_float fTimeDelta);
	void Create_Recipe_Window(_float fTimeDelta);

private:
	CUI* m_pFood_Select = nullptr;

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Cook_Icon* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END