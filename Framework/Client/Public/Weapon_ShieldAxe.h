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

class CWeapon_ShieldAxe final : public CWeapon
{
public:
	typedef struct tagWeaponDesc
	{
		const char*		pBoneName;
	}WEAPONDESC;
public:
	CWeapon_ShieldAxe();
	CWeapon_ShieldAxe(const CWeapon_ShieldAxe& rhs);
	virtual ~CWeapon_ShieldAxe() = default;

public:
	void	Set_Weapon(_uint iModelIdx) { m_pModelCom = m_pModel[iModelIdx]; }
	void	Set_ChangeWeapon();

	_uint	Get_WeaponType();

	void	Reset_Weapon();

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	//virtual void Tick(_float fTimeDelta);
	//virtual void LateTick(_float fTimeDelta);
	//virtual HRESULT Render();


private:
	CCollider* m_pCol_Sphere = nullptr;
	CModel* m_pModel[2] = { nullptr, };

private:
	HRESULT SetUp_Components();
	//HRESULT SetUp_ShaderResource();
	

public:
	static CWeapon_ShieldAxe* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END