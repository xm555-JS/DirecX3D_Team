#pragma once

#include "Client_Defines.h"
#include "Weapon.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CHierarchyNode;
END

BEGIN(Client)

class CWeapon_2Gun final : public CWeapon
{
public:
	typedef struct tagWeaponDesc
	{
		const char*		pBoneName;
	}WEAPONDESC;
public:
	CWeapon_2Gun();
	CWeapon_2Gun(const CWeapon_2Gun& rhs);
	virtual ~CWeapon_2Gun() = default;

public:
	void		Set_LeftHand() { m_bLeft = true; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	//virtual void Tick(_float fTimeDelta);
	//virtual void LateTick(_float fTimeDelta);
	//virtual HRESULT Render();


private:
	CCollider* m_pCol_Sphere = nullptr;



private:
	_bool		m_bLeft = false;

private:
	HRESULT SetUp_Components();
	//HRESULT SetUp_ShaderResource();
	

public:
	static CWeapon_2Gun* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END