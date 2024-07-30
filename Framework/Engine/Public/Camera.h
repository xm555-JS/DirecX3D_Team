#pragma once

#include "GameObject.h"


BEGIN(Engine)
class CRenderer;


class ENGINE_DLL CCamera : public CGameObject
{
public:
	typedef struct CameraDesc
	{
		/* 
		1. 트랜스폼원형을 클라에서 생성했다. 
		2. 원형객체를 복제하여 생성한 카메라에 트랜스폼 컴포넌트를 엔진에서 추가하고싶다. 
		3. 엔진내에서 클라에서 추가한 트랜스폼 원형의 정보를 던져준다. 
		*/
		_float4		vEye;
		_float4		vAt;
		_float		fFovy;
		_float		fAspect;
		_float		fNear;
		_float		fFar;

		 CTransform::TRANSFORMDESC		TransformDesc;
	}CAMERADESC;
protected:
	CCamera();
	CCamera(const CCamera& Prototype);
	virtual ~CCamera() = default;

public:
	CAMERADESC	Get_CameraDesc() { return m_CameraDesc; }
	_float3		Get_Offset() { return m_vOffset; }
	CRenderer*	Get_Renderer() {return m_pRenderer;}


	void		Set_CameraDesc(CAMERADESC Desc) { m_CameraDesc = Desc; }
	void		Set_Far(_float fFar) { m_CameraDesc.fFar = fFar; }
	void		Set_Offset(_vector vOffset) {
		XMStoreFloat3(&m_vNextOffset, vOffset);
		XMStoreFloat3(&m_vOriOffset, vOffset);
		XMStoreFloat3(&m_vOffset_Ori, vOffset);
		XMStoreFloat3(&m_vOffset, vOffset);
		m_pTransformCom->Set_Offset(XMLoadFloat3(&m_vOffset));
	}
	void		Set_Scale(_float fScale) { 
		XMStoreFloat3(&m_vOffset, XMLoadFloat3(&m_vOffset_Ori) * fScale);
		m_pTransformCom->Set_Offset(XMLoadFloat3(&m_vOffset));
	}


public:
	HRESULT		Add_RenderGroup(RENDERGROUP	eGroup, CGameObject* pGameObject);

	virtual	void		Reset_CamInfo() {};

	_bool		Is_Finish() { return m_bIsFinish; }

public:
	HRESULT Bind_PipeLine();

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) final {};
	virtual void Tick_Camera(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta) {};
	virtual HRESULT Render() { return S_OK; };

protected:
	CAMERADESC						m_CameraDesc;
	_float3			m_vOffset = { 0.f, 0.f, 0.f };
	_float3			m_vOffset_Ori = {0.f, 0.f, 0.f};

	_float3			m_vNextOffset = { 0.f, 0.f, 0.f };
	_float3			m_vOriOffset = { 0.f, 0.f, 0.f };

	_bool			m_bIsFinish = false;

protected:
	CRenderer*		m_pRenderer = nullptr;
	

public:
	static CCamera* Create();
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END