#pragma once
#include "Client_Defines.h"
#include "Effect_Base.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CModel;
END

BEGIN(Client)
class CEffect_Mesh final : public CEffect_Base
{
private:
	CEffect_Mesh();
	CEffect_Mesh(const CEffect_Mesh& rhs);
	virtual ~CEffect_Mesh() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Pooling(void* pArg = nullptr);
	void Initialize_Loop(void);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Set_PositioningMode(_matrix matTransform, _float fTimeSpeed);
	void Set_FollowingMode(_matrix matTransform, _float fTimeSpeed, CGameObject* pOwner, CEffect_Manager::FOLLOW_MODE eFollowMode, string strBoneName);

private:
	_uint							m_iNumMeshContainers = 0;
	_float							m_fTimeSpeed         = 1.f;
	CGameObject*					m_pOwner             = nullptr;
	_bool							m_bIsHaveOwner       = false;
	CModel*							m_pOwnerModelCom     = nullptr;
	string							m_strOwnerBoneName   = "";
	CEffect_Manager::FOLLOW_MODE	m_eFollowMode        = CEffect_Manager::FOLLOW_MODE_END;

private:
	_float4							m_vOffsetScale = {1.f, 1.f, 1.f, 0.f};

private:
	CShader*	m_pShaderCom            = nullptr;
	CModel*		m_pModelCom             = nullptr;
	CTexture*	m_pTextureCom_Mask      = nullptr;
	CTexture*	m_pTextureCom_Noise     = nullptr;
	_uint		m_iShaderPass           = 0;

	_float			m_fTimeAcc             = 0.f;
	_float			m_fTimePointOfDuration = 0.f; // For. Middle Easing


private:
	MESHEFFECT_DESC	m_MeshEffectDesc;

	_float2			m_vUVSpeedPos_Mask  = { 0.f, 0.f }; //유브이 위치(더해지는 양)
	_float2			m_vUVSpeedPos_Noise = { 0.f, 0.f }; //유브이 위치(더해지는 양)

	_float			m_fSpriteTimeAcc    = 0;
	_uint			m_fUVScaleXCur_Mask = 0;
	_uint			m_fUVScaleYCur_Mask = 0;

	_float4			m_vStartDiffuseColorRGBA   = { 1.f, 1.f, 1.f, 1.f };
	_float4			m_vCurrentDiffuseColorRGBA = { 1.f, 1.f, 1.f, 1.f };

	//_float4			m_vStartGlowColorRGBA   = { 1.f, 1.f, 1.f, 1.f };
	_float4			m_vCurrentGlowColorRGBA = { 1.f, 1.f, 1.f, 1.f };

	_float3			m_vStartSpeedXYZ   = { 1.f, 1.f, 1.f };
	_float3			m_vCurrentSpeedXYZ = { 1.f, 1.f, 1.f };

	_float3			m_vStartScaleRUL   = { 1.f, 1.f, 1.f };
	_float3			m_vCurrentScaleRUL = { 1.f, 1.f, 1.f };

	_float3			m_vStartRotateRUL        = { 1.f, 1.f, 1.f };
	_float3			m_vCurrentRotateSpeedRUL = { 1.f, 1.f, 1.f };
	_float3			m_fRotateAccelation      = { 1.f, 1.f, 1.f };

	_float	m_fRotationAcc = 0.f;

	_float4x4 m_OffSetMatrix;

	// Shader
	_float	m_fCurrentDistortionStrength = 0.f;
	_float4	m_vCurrentRimLightColorRGBA = { 1.f, 1.f, 1.f, 1.f };

private:
	void UVSpeed_Sprite(_float fTimeDelta);
	void Moving(_float fTimeDelta);
	void Scaling_Rotating(_float fTimeDelta);
	void Easing_DiffuseColor(void);
	void Easing_GlowColor(void);
	void Easing_Shader(void);


private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

	HRESULT	SetUp_MeshDesc(MESHEFFECT_DESC Desc);

public:
	static CEffect_Mesh* Create();
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};
END