#pragma once

#include "Client_Defines.h"
#include "UI_Cook_Window.h"

BEGIN(Client)

class CUI_Cook_FoodSelect final : public CUI_Cook_WIndow
{
public:
	CUI_Cook_FoodSelect();
	CUI_Cook_FoodSelect(const CUI_Cook_FoodSelect& rhs);
	virtual ~CUI_Cook_FoodSelect() = default;

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
	_float m_fSizeTime = 0.f;

private:
	void Select_Icon_Anim(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Cook_FoodSelect* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END