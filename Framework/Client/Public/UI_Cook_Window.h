#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Cook_WIndow abstract : public CUI
{
public:
	CUI_Cook_WIndow();
	CUI_Cook_WIndow(const CUI_Cook_WIndow& rhs);
	virtual ~CUI_Cook_WIndow() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Pooling(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

protected:
	_float m_fAlphaRatio = 1.f;
	_float m_fAlphaTime = 0.f;
	_float m_fAccTime = 0.f;
	_float m_fDir = 1.f;

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	virtual CGameObject* Clone(void* pArg) override { return nullptr; }
	virtual void Free() override;
};

END