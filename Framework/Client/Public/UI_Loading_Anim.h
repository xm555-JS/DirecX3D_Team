#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Loading_Anim final : public CUI
{
public:
	CUI_Loading_Anim();
	CUI_Loading_Anim(const CUI_Loading_Anim& rhs);
	virtual ~CUI_Loading_Anim() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Loading_Anim* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END