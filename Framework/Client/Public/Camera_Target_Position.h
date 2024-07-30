#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)
class CModel;
class CTransform;
END

BEGIN(Client)

class CCamera_Target_Position : public CCamera
{
public:
	CCamera_Target_Position();
	CCamera_Target_Position(const CCamera_Target_Position& rhs);
	virtual ~CCamera_Target_Position() = default;

public:
	//_float3		Get_Offset() { return m_vOffset; }
	virtual void	Set_TargetObject(CGameObject* pTarget);
	//void		Set_Offset(_vector vOffset) { XMStoreFloat3(&m_vOffset, vOffset); }

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick_Camera(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void	On_EventMessege(EVENTDESC tEvent);


protected:
	_float			m_fMoveSpeed = 0.f;
	_float			m_fRotateSpeed = 0.f;
	CGameObject*	m_pTarget = nullptr;
	CTransform*		m_pTargetTransform = nullptr;
	CModel*			m_pTargetModel = nullptr;
	//_float3			m_vOffset = {0.f, 0.f, 0.f};


public:
	static CCamera_Target_Position* Create();
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END