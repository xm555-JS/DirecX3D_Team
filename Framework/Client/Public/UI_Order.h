#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Order abstract : public CUI
{
public:
	CUI_Order();
	CUI_Order(const CUI_Order& rhs);
	virtual ~CUI_Order() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

protected:
	_float m_fAlphaRatio = 1.f;
	_float m_fAlphaTime = 0.f;
	_float m_fSizeTime = 0.f;
	_float m_fMoveTime = 0.f;

protected:
	void Create_Change_Alpha(_float fTimeDelta);
	void Create_Change_Size(_float fTimeDelta);
	void Create_Change_Pos(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	virtual CGameObject* Clone(void* pArg) override { return nullptr; }
	virtual void Free() override;
};

END