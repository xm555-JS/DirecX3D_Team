#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CVIBuffer;
class CTexture;
class CPhyXCollider;
class CCollider;
END

BEGIN(Client)
class CCursor;
class CAnimObject final : public CGameObject
{
public:
	CAnimObject();
	CAnimObject(const CAnimObject& rhs);
	virtual ~CAnimObject() = default;

public:
	void Set_Animation(_uint iNum) { m_iAnimNum = iNum; }
	void Set_AnimPlay(_bool bPlay) { m_bAnimPlay = bPlay; }
	void Set_AnimAccTime(_float fAccTime) { m_fAnimAcc = fAccTime; }

	void Set_AnimGroup(ANIM_GROUP eGroup) { m_eAnimGroup = eGroup; }
	void Set_WeaponType(WEAPON_TYPE eWeapon) { m_eWeaponType = eWeapon; }

	void Set_PassIndex(_uint iNum) { m_iPassIndex = iNum; }

	void Set_BoneMatrix(_matrix BoneMatrix) 
	{
		XMStoreFloat4x4(&m_BoneMatrix, BoneMatrix);
	}

	_float Get_AnimAccTime() { return m_fAnimAcc; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

	void	Input_Key(_float fTimeDelta);

private:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CModel*				m_pModelCom = nullptr;
	CCollider*			m_pColCom = nullptr;

	_uint				m_iNumMeshContainers = 0;

	_uint				m_iTest = 0;
	_uint				m_iPassIndex = 0;

private:
	_uint				m_iAnimNum = 0;
	_uint				m_iAnimModelID = 0;
	_bool				m_bAnimPlay = false;
	_float				m_fAnimAcc = 1.f;

	_float4x4			m_BoneMatrix;

	ANIM_GROUP			m_eAnimGroup = ANIM_END;
	WEAPON_TYPE			m_eWeaponType = WEAPON_END;

public:
	static CAnimObject* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

