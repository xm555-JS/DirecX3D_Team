#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Proceeding final : public CUI
{
public:
	CUI_Proceeding();
	CUI_Proceeding(const CUI_Proceeding& rhs);
	virtual ~CUI_Proceeding() = default;

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
	_float m_fAlphaRatio = 1.f;
	_float m_fProgressRatio = 0.f;


private:
	void Resizing_EdgeRing(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components();
	virtual HRESULT SetUp_ShaderResource();

public:
	static CUI_Proceeding* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END