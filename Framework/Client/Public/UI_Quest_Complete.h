#pragma once

#include "Client_Defines.h"
#include "UI_Quest.h"

BEGIN(Client)

class CUI_Quest_Complete final : public CUI_Quest
{
public:
	CUI_Quest_Complete();
	CUI_Quest_Complete(const CUI_Quest_Complete& rhs);
	virtual ~CUI_Quest_Complete() = default;

public:
	void Set_TimeDir(_float m_fDir) { m_fTimeDir = m_fDir; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	_float m_fAccTime = 0.f;
	_float m_fTimeDir = -1.f;

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Quest_Complete* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END