#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Inventory abstract : public CUI
{
public:
	CUI_Inventory();
	CUI_Inventory(const CUI_Inventory& rhs);
	virtual ~CUI_Inventory() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Pooling(void* pArg) { return S_OK; }
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

protected:
	_float m_fAccTime = 0.f;
	_float m_fSizeRatioX = 0.f;
	_float m_fAlphaRatio = 0.f;


protected:
	void Create_Inven_Window(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	virtual CGameObject* Clone(void* pArg) override { return nullptr;}
	virtual void Free() override;
};

END