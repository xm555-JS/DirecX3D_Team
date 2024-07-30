#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Minimap final : public CUI
{
public:
	CUI_Minimap();
	CUI_Minimap(const CUI_Minimap& rhs);
	virtual ~CUI_Minimap() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	CPlayer* m_pPlayer = nullptr;
	_float2  m_vMapPos = { 0.f, 0.f };

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Minimap* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END