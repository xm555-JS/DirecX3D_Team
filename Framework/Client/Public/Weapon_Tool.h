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

class CWeapon_Tool final : public CWeapon
{
public:
	typedef struct tagWeaponDesc
	{
		const char*		pBoneName;
	}WEAPONDESC;
public:
	CWeapon_Tool();
	CWeapon_Tool(const CWeapon_Tool& rhs);
	virtual ~CWeapon_Tool() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	//virtual void Tick(_float fTimeDelta);
	//virtual void LateTick(_float fTimeDelta);
	//virtual HRESULT Render();

private:
	_uint				m_iModelID = 0;

private:
	HRESULT SetUp_Components();
	//HRESULT SetUp_ShaderResource();
	

public:
	static CWeapon_Tool* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END