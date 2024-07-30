#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Boss_Name final : public CUI
{
public:
	CUI_Boss_Name();
	CUI_Boss_Name(const CUI_Boss_Name& rhs);
	virtual ~CUI_Boss_Name() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	_bool m_bRender = true;

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Boss_Name* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END