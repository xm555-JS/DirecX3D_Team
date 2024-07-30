#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Gun_CrossHair final : public CUI
{
public:
	CUI_Gun_CrossHair();
	CUI_Gun_CrossHair(const CUI_Gun_CrossHair& rhs);
	virtual ~CUI_Gun_CrossHair() = default;

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

private:
	class CPlayer* m_pPlayer = nullptr;
	_float		   m_fAccTime = 0.f;

private:
	void Resizing_CrossHair(_float fTimeDelta);

public:
	static CUI_Gun_CrossHair* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END