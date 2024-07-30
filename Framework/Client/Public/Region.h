#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CCollider;
END

BEGIN(Client)

class CRegion : public CGameObject
{
public:
	CRegion();
	CRegion(const CRegion& rhs);
	virtual ~CRegion() = default;

public:
	typedef struct RegionDesc
	{
		RegionDesc() {}
		RegionDesc(_float4 _vPos, _float3 _vCol_Scale, REGION_TYPE _eRegionType)
		{
			vPos = _vPos;
			vCol_Scale = _vCol_Scale;
			eRegionType = _eRegionType;
		}

		_float4 vPos = {0.f,0.f,0.f,1.f};
		_float3 vCol_Scale = { 1.f,1.f,1.f };
		REGION_TYPE eRegionType = REGION_END;

	}REGIONDESC;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual void On_EventMessege(EVENTDESC tEvent);

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

private:
	REGIONDESC m_tRegionDesc;
	CCollider* m_pCol_Region = nullptr;

private:
	_bool		m_bStartBGM = false;
	_bool		m_bLeviathan_Apear = false;

public:
	static CRegion* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

