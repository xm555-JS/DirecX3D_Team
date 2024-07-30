#pragma once

#include "Client_Defines.h"
#include "UI_Inventory.h"

BEGIN(Client)

class CUI_Inven_Icon final : public CUI_Inventory
{
public:
	CUI_Inven_Icon();
	CUI_Inven_Icon(const CUI_Inven_Icon& rhs);
	virtual ~CUI_Inven_Icon() = default;

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
	_float m_fMoveUVSpeed = 0.f;

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Inven_Icon* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END