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
class CEnemy_Bullet_FrostDragon final : public CBullet_Normal
{
public:
	CEnemy_Bullet_FrostDragon();
	CEnemy_Bullet_FrostDragon(const CBullet_Normal& rhs);
	virtual ~CEnemy_Bullet_FrostDragon() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Pooling(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

private:
	_float4			m_vStartLook;
	DECL_ONCE(m_bEffect0);
	DECL_ONCE(m_bSound0);

private:
	_float m_fAccTime = 0.f;
	_float m_fAccTime_Effect0 = 0.f;
	_float m_fAccTime_Effect1 = 0.f;

private:
	virtual HRESULT SetUp_Components();
	virtual HRESULT SetUp_ShaderResource();


public:
	static CEnemy_Bullet_FrostDragon* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END