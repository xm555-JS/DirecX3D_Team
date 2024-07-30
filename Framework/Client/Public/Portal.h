#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CVIBuffer;
class CTexture;
class CCollider;
END

BEGIN(Client)
class CCursor;
class CEffect_Base;
class CPortal : public CGameObject
{
public:
	CPortal();
	CPortal(const CPortal& rhs);
	virtual ~CPortal() = default;

public:
	typedef struct PortalDesc
	{
		_float4 m_vPos, m_vTargetPos;
		_bool m_bUseAble = false;
		PORTAL_TYPE m_ePortalType = PORTAL_END;
	}PORTALDESC;

public:
	void	Set_UsePortal(_bool bUseAble) { m_PortalDesc.m_bUseAble = bUseAble; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();


protected:
	class CCollider* m_pCol_Portal = nullptr;

private:
	PORTALDESC	m_PortalDesc;

	list<CGameObject*> m_PlaceMarklist;
	list<CEffect_Base*> m_listEffect;
	DECL_ONCE(m_bCreate_PlaceMark);
	DECL_ONCE(m_bPooling_PlaceMark);

private:
	HRESULT SetUp_Teleport();
	void Create_UI_PlaceMark(_float fOffsetY = 0.f);
	void Pooling_UI_PlaceMark();

public:
	static CPortal* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

