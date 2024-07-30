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
class CEnemy_Boomb_FrostDragon final : public CBullet_Normal
{

public:
	CEnemy_Boomb_FrostDragon();
	CEnemy_Boomb_FrostDragon(const CBullet_Normal& rhs);
	virtual ~CEnemy_Boomb_FrostDragon() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Pooling(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

private:
	_vector			m_vStartLook;

private:
	_float m_fAccTime = 0.f;

private:
	virtual HRESULT SetUp_Components();
	virtual HRESULT SetUp_ShaderResource();


public:
	static CEnemy_Boomb_FrostDragon* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END