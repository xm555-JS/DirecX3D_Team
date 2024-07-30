#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CVIBuffer;
class CTexture;
class CPhyXCollider;
class CCollider;
END

BEGIN(Client)
class CCursor;
class CEnemy_Weaver_Frozen final : public CGameObject
{
public:
	CEnemy_Weaver_Frozen();
	CEnemy_Weaver_Frozen(const CGameObject& rhs);
	virtual ~CEnemy_Weaver_Frozen() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Pooling(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	_float4			m_vPos            = { 0.f, 0.f, 0.f, 1.f };
	CCollider*		m_pCol_Sphere     = nullptr;
	_float			m_fDamage         = 0.f;
	_bool			m_bIsPhaseChanged = false;
	_bool			m_bToggle_Effcet = false;
	DECL_ONCE(m_bEffect0);
	DECL_ONCE(m_bEffect1);

private:
	_float			m_fAccTime = 0.f;
	_bool			m_bSound = true;

private:
	virtual HRESULT SetUp_Components();
	virtual HRESULT SetUp_ShaderResource();


public:
	static CEnemy_Weaver_Frozen* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END