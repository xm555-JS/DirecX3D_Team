#pragma once

#include "Client_Defines.h"
#include "UI_Equip_Window.h"

BEGIN(Client)

class CUI_Equip_Detail final : public CUI_Equip_Window
{
public:
	CUI_Equip_Detail();
	CUI_Equip_Detail(const CUI_Equip_Detail& rhs);
	virtual ~CUI_Equip_Detail() = default;

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
	_uint m_iPreTextureIdx;
	_bool m_bChangeTexture = false;

	_float m_fWaitTime = 0.f;

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Equip_Detail* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END