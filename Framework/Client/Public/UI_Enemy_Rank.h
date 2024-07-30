#pragma once

#include "Client_Defines.h"
#include "UI_Solid.h"

BEGIN(Client)

class CUI_Enemy_Rank final : public CUI_Solid
{
public:
	CUI_Enemy_Rank();
	CUI_Enemy_Rank(const CUI_Enemy_Rank& rhs);
	virtual ~CUI_Enemy_Rank() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	void SetUp_Pos();

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Enemy_Rank* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END