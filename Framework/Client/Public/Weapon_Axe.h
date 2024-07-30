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

class CWeapon_Axe final : public CWeapon
{
public:
	typedef struct tagWeaponDesc
	{
		const char*		pBoneName;
	}WEAPONDESC;
public:
	CWeapon_Axe();
	CWeapon_Axe(const CWeapon_Axe& rhs);
	virtual ~CWeapon_Axe() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	//virtual void Tick(_float fTimeDelta);
	//virtual void LateTick(_float fTimeDelta);
	//virtual HRESULT Render();


private:
	CCollider* m_pCol_Sphere = nullptr;


private:
	HRESULT SetUp_Components();
	//HRESULT SetUp_ShaderResource();
	

public:
	static CWeapon_Axe* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END