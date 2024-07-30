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
class CEnemy_Weaver_Hurricane final : public CGameObject
{
public:
	typedef struct HurricaneDesc
	{
		_float4			OrbitPos;
		_float4			StartPos;
		CGameObject*	pTarget;
		_uint			iIndex;
	}HURRICANEDESC;	

public:
	CEnemy_Weaver_Hurricane();
	CEnemy_Weaver_Hurricane(const CGameObject& rhs);
	virtual ~CEnemy_Weaver_Hurricane() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Pooling(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CCollider*		m_pCol_Sphere     = nullptr;
	_float			m_fDamage         = 0.f;
	_float4			m_vStartLook;
	HURRICANEDESC	m_HurricaneDesc;

	_bool			m_bEndAttack = false;
	_bool			m_bSound = true;
	_float			m_fSound[2] = { 0.f, };

	DECL_ONCE(m_bEffect0);
	DECL_ONCE(m_bEffect1);

private:
	_float m_fAccTime = 0.f;
	_float m_fDamagedTime = 0.f;

private:
	virtual HRESULT SetUp_Components();
	virtual HRESULT SetUp_ShaderResource();


public:
	static CEnemy_Weaver_Hurricane* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END