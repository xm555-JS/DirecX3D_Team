#pragma once

#include "Client_Defines.h"
#include "Enemy_Normal.h"

//BEGIN(Engine)
//class CShader;
//class CRenderer;
//class CModel;
//class CVIBuffer;
//class CTexture;
//class CPhyXCollider;
//class CCollider;
//class CState_Handler;
//END

BEGIN(Client)
/*°»½ºÅÍ Ä¸*/
class CGangster_Cap final : public CEnemy_Normal
{
public:
	CGangster_Cap();
	CGangster_Cap(const CGangster_Cap& rhs);
	virtual ~CGangster_Cap() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void	On_Dead();

private:
	HRESULT SetUp_Components();

	HRESULT SetUp_State_Handler();

private:
	HRESULT SetUp_Weapon();

public:
	static CGangster_Cap* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END