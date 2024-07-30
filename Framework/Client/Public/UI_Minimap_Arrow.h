#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Engine)
class CCamera;
END


BEGIN(Client)

class CUI_Minimap_Arrow final : public CUI
{
public:
	CUI_Minimap_Arrow();
	CUI_Minimap_Arrow(const CUI_Minimap_Arrow& rhs);
	virtual ~CUI_Minimap_Arrow() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	class CPlayer* m_pPlayer = nullptr;
	CCamera* m_pCamera = nullptr;

	_float m_fAngleDir = 1.f;

private:
	void SetUp_PlayerArrow();
	void SetUp_CameraArrow();

private:
	virtual HRESULT SetUp_Components(); 
	virtual HRESULT SetUp_ShaderResource(); 

public:
	static CUI_Minimap_Arrow* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END