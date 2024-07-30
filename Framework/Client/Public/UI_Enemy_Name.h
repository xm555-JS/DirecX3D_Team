#pragma once

#include "Client_Defines.h"
#include "UI_Solid.h"

BEGIN(Client)

class CUI_Enemy_Name final : public CUI_Solid
{
public:
	CUI_Enemy_Name();
	CUI_Enemy_Name(const CUI_Enemy_Name& rhs);
	virtual ~CUI_Enemy_Name() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	void SetUp_Pos();

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Enemy_Name* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END