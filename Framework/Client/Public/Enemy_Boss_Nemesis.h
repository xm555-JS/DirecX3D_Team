
#pragma once

#include "Client_Defines.h"
#include "Enemy_Boss.h"

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
class CEnemy_Boss_Nemesis final : public CEnemy_Boss
{
public:
	CEnemy_Boss_Nemesis();
	CEnemy_Boss_Nemesis(const CEnemy_Boss& rhs);
	virtual ~CEnemy_Boss_Nemesis() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

private:
	virtual HRESULT SetUp_Components();
	virtual HRESULT SetUp_ShaderResource();
	virtual	HRESULT SetUp_State_Handler();


public:
	static CEnemy_Boss_Nemesis* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END