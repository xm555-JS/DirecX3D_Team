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
/*하이에나 졸개*/
class CHyenaGang_Minions final : public CEnemy_Normal
{
public:
	CHyenaGang_Minions();
	CHyenaGang_Minions(const CHyenaGang_Minions& rhs);
	virtual ~CHyenaGang_Minions() = default;

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
	static CHyenaGang_Minions* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END