#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Engine)
class CCamera;
END


BEGIN(Client)

class CUI_Loading_Screen final : public CUI
{
public:
	CUI_Loading_Screen();
	CUI_Loading_Screen(const CUI_Loading_Screen& rhs);
	virtual ~CUI_Loading_Screen() = default;

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
	CCamera* m_pInvenCamera = nullptr;
	CCamera* m_pEquipCamera = nullptr;
	CCamera* m_pTargetCamera = nullptr;

	_float m_fAlphaRatio = 1.f;
	_float m_fAccTime = 0.f;
	_float m_fWaitTIme = 0.f;

	DECL_ONCE(m_bWarpPlayer);

private:
	void Create_Camera();
	void Move_Player(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Loading_Screen* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END