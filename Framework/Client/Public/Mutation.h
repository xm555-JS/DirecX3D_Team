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
/*돌연변이*/
class CMutation final : public CEnemy_Normal
{
public:
	CMutation();
	CMutation(const CMutation& rhs);
	virtual ~CMutation() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_Components();

	HRESULT SetUp_State_Handler();


public:
	static CMutation* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END