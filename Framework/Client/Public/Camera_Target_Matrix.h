#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Target_Matrix final : public CCamera
{
public:
	CCamera_Target_Matrix();
	CCamera_Target_Matrix(const CCamera_Target_Matrix& rhs);
	virtual ~CCamera_Target_Matrix() = default;

public:
	//_float3		Get_Offset() { return m_vOffset; }
	void		Set_TargetObject(CGameObject* pTarget, CTransform * pTargetTransform = nullptr);
	void		Set_Far(_float fFar) { m_CameraDesc.fFar = fFar; }
	//void		Set_Offset(_vector vOffset) { XMStoreFloat3(&m_vOffset, vOffset); }

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick_Camera(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_float			m_fMoveSpeed = 0.f;
	_float			m_fRotateSpeed = 0.f;
	CGameObject*	m_pTarget = nullptr;
	CTransform*		m_pTargetTransform = nullptr;
	//_float3			m_vOffset = {0.f, 0.f, 0.f};


public:
	static CCamera_Target_Matrix* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END