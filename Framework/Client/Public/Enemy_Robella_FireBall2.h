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
class CEnemy_Robella_FireBall2 final : public CBullet_Normal
{
public:
	CEnemy_Robella_FireBall2();
	CEnemy_Robella_FireBall2(const CBullet_Normal& rhs);
	virtual ~CEnemy_Robella_FireBall2() = default;

public:
	void Set_Shoot() { m_bShoot = true; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Pooling(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	_float4			m_vPos            = { 0.f, 0.f, 0.f, 1.f };
	_float4			m_vTargetPos	  = { 0.f, 0.f, 0.f, 1.f };
	_float			m_fDamage       = 0.f;
	_bool			m_bShoot		= false;
	_float			m_vDeadHeight   = 0.f;
	
	DECL_ONCE(m_bSound0);
	DECL_ONCE(m_bSound1);

private:
	_float m_fTimeAcc_Effect0 = 0.f;
	_float m_fTimeAcc_Effect1 = 0.f;
	_float m_fTimeAcc = 0.f;

private:
	virtual HRESULT SetUp_Components();
	virtual HRESULT SetUp_ShaderResource();


public:
	static CEnemy_Robella_FireBall2* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END