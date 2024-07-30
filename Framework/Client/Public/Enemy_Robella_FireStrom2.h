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
class CEnemy_Robella_FireStrom2 final : public CGameObject
{
public:
	CEnemy_Robella_FireStrom2();
	CEnemy_Robella_FireStrom2(const CGameObject& rhs);
	virtual ~CEnemy_Robella_FireStrom2() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Pooling(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CGameObject*	m_pTarget = nullptr;
	CCollider*		m_pCol_Sphere     = nullptr;
	_float			m_fDamage         = 0.f;
	_bool			m_bIsPhaseChanged = false;
	DECL_ONCE(m_bEffect0);
	DECL_ONCE(m_bEffect1);

	DECL_ONCE(m_bSound0);
	DECL_ONCE(m_bSound1);

private:
	_float m_fAccTime = 0.f;
	_float m_fAttackAccTime = 0.f;

private:
	virtual HRESULT SetUp_Components();
	virtual HRESULT SetUp_ShaderResource();


public:
	static CEnemy_Robella_FireStrom2* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END