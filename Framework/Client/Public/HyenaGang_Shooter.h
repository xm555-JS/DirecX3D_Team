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
/*하이에나 사수*/
class CHyenaGang_Shooter final : public CEnemy_Normal
{
public:
	CHyenaGang_Shooter();
	CHyenaGang_Shooter(const CHyenaGang_Shooter& rhs);
	virtual ~CHyenaGang_Shooter() = default;

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

	HRESULT SetUp_Weapon();


public:
	static CHyenaGang_Shooter* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END