#pragma once

#include "Client_Defines.h"
#include "Effect_Base.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect_Instance;
class CModel;
END

BEGIN(Client)

class CEffect_Rect final : public CEffect_Base
{
public:
	CEffect_Rect();
	CEffect_Rect(const CEffect_Rect& rhs);
	virtual ~CEffect_Rect() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Pooling(void* pArg = nullptr);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Set_PositioningMode(_matrix matTransform, _float fTimeSpeed);
	void Set_FollowingMode(_matrix matTransform, _float fTimeSpeed, CGameObject* pOwner, CEffect_Manager::FOLLOW_MODE eFollowMode, string strBoneName);

private:
	CShader* m_pShaderCom = nullptr;
	CTexture* m_pTextureCom = nullptr;
	CVIBuffer_Rect_Instance* m_pVIBufferCom = nullptr;

	PARTICLE_DESC m_ParticleDesc;

	_float							m_fTimeSpeed = 1.f;
	CGameObject*					m_pOwner = nullptr;
	_bool							m_bIsHaveOwner = false;
	CModel*							m_pOwnerModelCom = nullptr;
	string							m_strOwnerBoneName = "";
	CEffect_Manager::FOLLOW_MODE	m_eFollowMode = CEffect_Manager::FOLLOW_MODE_END;

	_float	m_fTimeAcc = 0.f;
	_float	m_fTimePointOfDuration = 0.f; // For. Middle Easing

	_float4 m_vStartDiffuseColorRGBA = { 1.f, 1.f, 1.f, 1.f };
	_float4 m_vCurrentDiffuseColorRGBA = { 1.f, 1.f, 1.f, 1.f };

	_float4 m_vStartGlowColorRGBA = { 1.f, 1.f, 1.f, 1.f };
	_float4 m_vCurrentGlowColorRGBA = { 1.f, 1.f, 1.f, 1.f };

	_float4x4 m_OffsetMatrix;


private:
	void Easing_DiffuseColor(void);
	void Easing_GlowColor(void);

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

public:
	static CEffect_Rect* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END