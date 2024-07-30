#pragma once

#include "Client_Defines.h"
#include "Bullet_Normal.h"

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
class CEnemy_Leviathan_Thunder final : public CBullet_Normal
{
public:
	CEnemy_Leviathan_Thunder();
	CEnemy_Leviathan_Thunder(const CBullet_Normal& rhs);
	virtual ~CEnemy_Leviathan_Thunder() = default;

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
	DECL_ONCE(m_bEffect0);
	DECL_ONCE(m_bEffect1);
	DECL_ONCE(m_bSound0);

private:
	_float m_fAccTime = 0.f;

private:
	virtual HRESULT SetUp_Components();
	virtual HRESULT SetUp_ShaderResource();


public:
	static CEnemy_Leviathan_Thunder* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END