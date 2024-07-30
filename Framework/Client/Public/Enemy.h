#pragma once

#include "Client_Defines.h"
#include "Unit.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)
class CState_Handler;
class CWeapon;

class CEnemy abstract : public CUnit
{
public:
	CEnemy();
	CEnemy(const CEnemy& rhs);
	virtual ~CEnemy() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(_float fTimeDelta);

public:
	virtual void Set_StartPos(_matrix StartPos) { XMStoreFloat4(&m_vStartPos, StartPos.r[3]); }
	virtual void Set_StartPos(_vector vStartPos) {  XMStoreFloat4(&m_vStartPos, vStartPos); }
	virtual void Set_PreHp(_float fHp) { m_fPreHp = fHp; }
	void Set_Weapon(CWeapon* _pWeapon) { m_pWeapon = _pWeapon; }
	void Set_IsAttacked(_bool bAttacked) { m_bAttacked = bAttacked; }
	void Set_IsDissolve(_bool bAttacked) { m_bDissolve = bAttacked; }

	virtual _vector Get_StartPos() { return XMLoadFloat4(&m_vStartPos); }
	virtual _float Get_PreHp() { return m_fPreHp; }
	CWeapon* Get_Weapon() { return m_pWeapon; }
	_bool Get_IsAttacked() { return m_bAttacked; }


public:
	virtual void	Damaged(ELEMENTAL_PROPERTIE eElement, _float fPower);

protected:
	virtual void	On_Dead();

protected:
	DECL_ONCE(m_bCreate_PlaceMark);

protected:
	CWeapon* m_pWeapon = nullptr;

protected:
	_float4		m_vStartPos = _float4(0.f,0.f,0.f,1.f);
	_bool		m_bAttacked = false;
	_float		m_fTimeAttacked = 0.f;
	_bool		m_bUseRimLight = false;
	_float4		m_vRimLightColor = _float4(1.f, 1.f, 1.f, 1.f);
	_bool		m_bBossEnemy = false;

protected:
	CTexture*	m_pDissolveTextureCom = nullptr;
	_uint		m_iDissolveTextureNoise = 0;
	_uint		m_iDissolveTextureColor = 0;
	_bool		m_bDissolve = false;
	_float4		m_vDissolveColor = _float4(1.f, 1.f, 1.f, 1.f);
	_float		m_fDissolveStrength = 0.f;
	_float		m_fTimeAccDissolve = 0.f;
	_float		m_fTimeDurationDissolve = 0.f;

protected:
	list<CGameObject*> m_PlaceMarklist;

protected:
	void	Create_UI_Place_Mark(_float fOffsetY = 0.f);
	void	Pooling_UI_PlaceMark();

protected:
	virtual HRESULT SetUp_ShaderResource();

protected:
	void		SetUp_NonAttacked(_float fTimeDelta);
	void		Push_EachOther(CCollider* pTargetCol, CCollider* pVolumeCol, CTransform* pTargetTransform, CNavigation* pTargetNavigation);

public:
	virtual void Free() override;
};

END