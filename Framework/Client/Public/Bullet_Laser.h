#pragma once

#include "Client_Defines.h"
#include "Bullet.h"

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
class CBullet_Laser abstract : public CBullet
{
public:
	CBullet_Laser();
	CBullet_Laser(const CBullet& rhs);
	virtual ~CBullet_Laser() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components();
	virtual HRESULT SetUp_ShaderResource();


public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END