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

class CWeapon_Hammer final : public CWeapon
{
public:
	typedef struct tagWeaponDesc
	{
		const char*		pBoneName;
	}WEAPONDESC;
public:
	CWeapon_Hammer();
	CWeapon_Hammer(const CWeapon_Hammer& rhs);
	virtual ~CWeapon_Hammer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();


private:
	CCollider* m_pCol_Sphere = nullptr;
	_matrix		m_ColMatrix;

private:
	HRESULT SetUp_Components();
	//HRESULT SetUp_ShaderResource();
	

public:
	static CWeapon_Hammer* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END