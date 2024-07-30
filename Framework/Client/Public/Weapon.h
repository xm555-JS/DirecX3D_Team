#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CHierarchyNode;
class CCollider;
END

BEGIN(Client)

class CWeapon abstract : public CGameObject
{
protected:
	CWeapon();
	CWeapon(const CWeapon& rhs);
	virtual ~CWeapon() = default;

public:
	void	Setup_WeaponState(CGameObject* pTargetObject, string strTargetBoneName);
	void	Set_IsDissolve(_bool bAttacked) { m_bDissolve = bAttacked; }
	void	Set_IsUsing(_bool bUsing) { m_bUsing = bUsing; }
	void	Set_IsGlow(_bool bGlow) { m_bGlow = bGlow; }

	_bool	Get_Dissolved() { return (m_fTimeAccDissolve >= m_fTimeDurationDissolve); }
	_bool	Get_IsUsing() { return m_bUsing; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render(void* pArg);

protected:
	CShader*				m_pShaderCom = nullptr;	
	CRenderer*				m_pRendererCom = nullptr;		
	CModel*					m_pModelCom = nullptr;

	_float4x4				m_WorldMatrix;

protected:
	class CHierarchyNode*	m_pBonePtr = nullptr;
	CTransform*				m_pParentTransformCom = nullptr;

	CGameObject*			m_pTarget = nullptr;
	string					m_strTargetBoneName;

	_bool					m_bUsing = false;

protected:
	CTexture*				m_pDissolveTextureCom = nullptr;
	_uint					m_iDissolveTextureNoise = 0;
	_uint					m_iDissolveTextureColor = 0;
	_bool					m_bDissolve = false;
	_float4					m_vDissolveColor = _float4(1.f, 1.f, 1.f, 1.f);
	_float					m_fDissolveStrength = 0.f;
	_float					m_fTimeAccDissolve = 0.f;
	_float					m_fTimeDurationDissolve = 0.f;

	_bool					m_bGlow = false;
	_float4					m_vGlowColor = _float4(1.f, 1.f, 1.f, 1.f);

protected:
	_uint					m_iNumMeshContainers = 0;

protected:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();
	

public:
	virtual void Free() override;
};

END