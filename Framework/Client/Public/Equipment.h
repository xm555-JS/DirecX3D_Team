#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
END

BEGIN(Client)
class CEquipment final : public CGameObject
{
public:
	CEquipment();
	CEquipment(const CEquipment& rhs);
	virtual ~CEquipment() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();


private:
	class CShader*		m_pShaderCom = nullptr;
	class CModel*		m_pModelCom = nullptr;
	class CCollider*	m_pCol_Sphere = nullptr;

	_uint				m_iNumMeshContainers = 0;

	_bool				m_bEquiped_Weapon = false;
	_bool				m_bOwnWeapon = true;
	_bool				m_bIsUsing = false;
	WEAPON_TYPE			m_eWeapon_Type = WEAPON_END;

private:
	void Check_OwnWeapon();
	void Check_EquipedWeapon();
	void Select_Weapon();
	void SetUp_UsingState();

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

public:
	static CEquipment* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END