#pragma once

#include "Client_Defines.h"
#include "Enemy_Normal.h"

BEGIN(Engine)
//class CShader;
//class CRenderer;
//class CModel;
//class CVIBuffer;
//class CTexture;
//class CPhyXCollider;
class CCollider;
//class CState_Handler;
END

BEGIN(Client)
/*»ç³É°³*/
class CHunterDog final : public CEnemy_Normal
{
public:
	CHunterDog();
	CHunterDog(const CHunterDog& rhs);
	virtual ~CHunterDog() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CCollider* m_pCol_Sphere = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

	HRESULT SetUp_State_Handler();


public:
	static CHunterDog* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END