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

class CMobility_Glider final : public CWeapon
{
public:
	typedef struct tagWeaponDesc
	{
		const char*		pBoneName;
	}WEAPONDESC;
public:
	CMobility_Glider();
	CMobility_Glider(const CMobility_Glider& rhs);
	virtual ~CMobility_Glider() = default;

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
	static CMobility_Glider* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END