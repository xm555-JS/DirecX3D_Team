#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Minigame_Notice final : public CUI
{
public:
	CUI_Minigame_Notice();
	CUI_Minigame_Notice(const CUI_Minigame_Notice& rhs);
	virtual ~CUI_Minigame_Notice() = default;

public:
	void Set_TimeDir(_float fDir) { m_fTimeDir = fDir; }

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
	_float m_fAccTime = 0.f;
	_float m_fTimeDir = 1.f;

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Minigame_Notice* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END