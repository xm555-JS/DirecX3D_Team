#pragma once

#include "Transform.h"

BEGIN(Engine)
class CLayer;

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	CGameObject();	
	CGameObject(const CGameObject& Prototype);
	virtual ~CGameObject() = default;

public:
	class CComponent* Get_Component(ID_UINT iComponentID);


	_float		Get_CamDistance() { return m_fCamDistance; }
	CTransform* Get_Transform() { return m_pTransformCom; }
	_uint		Get_PrototypeID() { return m_iPrototypeID.iID; }
	_uint		Get_LayerID() { return m_iLayerID.iID; }
	ID_UINT		Get_Layer() { return m_tLayerID; }
	_uint		Get_RenderLayer() { return m_iRenderLayer; }
	_vector		Get_Pos() { return XMLoadFloat4x4(m_pWorldMatrix).r[3]; }
	_uint		Get_CurTickGroup(void) { return m_iTickGroup; }
	_uint		Get_CurLateTickGroup(void) { return m_iLateTickGroup; }
	_uint		Get_CurLevel(void) { return m_iCurLevel; }
	_float4x4*	Get_WorldMatrix(void) { return m_pWorldMatrix; }
	_bool		Get_RenderState() { return m_bIsRender; }
	_float		Get_TimeRate() { return m_fTimeRate; }


	void	Set_CurLevel(_uint iLevel) { m_iCurLevel = iLevel; }
	void	Set_Parent(CGameObject* pObject) 
	{ 
		m_pParent = pObject; 
		m_pTransformCom->Set_PerantTransform(pObject->Get_Transform());
	}
	void	Set_PrototypeID(ID_UINT iPrototype) { m_iPrototypeID = iPrototype; }
	void	Set_PrototypeID(_uint iPrototype) { m_iPrototypeID.iID = iPrototype; }
	void	Set_LayerID(ID_UINT tLayerID) { m_tLayerID = tLayerID; }
	void	Set_LayerID(_uint iLayerID) { m_iLayerID.iID = iLayerID; }
	void    Set_RenderLayer(_uint iRenderLayer) { m_iRenderLayer = iRenderLayer; }
	void	Set_Pos(_fvector vPos)
	{
		_float4		vData;
		XMStoreFloat4(&vData, vPos);
		memcpy(&(*m_pWorldMatrix).m[3], &vData, sizeof(_float4));
			(*m_pWorldMatrix)._44 = 1.f;
	}
	void	Set_CurTickGroup(_uint iTickGroup) { m_iTickGroup = iTickGroup; }
	void	Set_CurLateTickGroup(_uint iLateTickGroup) { m_iLateTickGroup = iLateTickGroup; }
	void	Set_RenderState(_bool bState) { m_bIsRender = bState; }
	void	Set_TiemRate(_float fTimeRate) { m_fTimeRate = fTimeRate; }


	void	Setup_ComponentDead();
	void	Setup_ComponentDisable();
	void	Setup_ComponentAble();

public:
	virtual HRESULT Initialize_Prototype()						{ return S_OK; }
	virtual HRESULT Initialize(void* pArg = nullptr);
	virtual HRESULT Initialize_Pooling(void* pArg = nullptr)	{ return S_OK; }
	virtual void Tick(_float fTimeDelta)						{}
	virtual void LateTick(_float fTimeDelta)					{}
	virtual HRESULT Render()									{ return S_OK; }
	virtual HRESULT Render(void* pArg)							{ return S_OK; }
	HRESULT Compute_CamDistance();


public:
	HRESULT	SetUp_PoolingState(_uint iLevelIndex, _uint tLayerID, _uint tPrototypeID);


protected:
	CTransform*		m_pTransformCom = nullptr;
	_float4x4*		m_pWorldMatrix = nullptr;
	_float4x4*		m_pCombinedTransformationMatrix = nullptr;


protected:
	_float						m_fCamDistance = 0.f;

protected:
	CGameObject*				m_pParent = nullptr;
	ID_UINT						m_tLayerID;

protected:
	_uint						m_iRenderLayer = 1000;

protected:
	map<_uint, class CComponent*>			m_Components;
	typedef map<_uint, class CComponent*>	COMPONENTS;

	static ID_UINT		m_tTransformID;
	_uint				m_iCurLevel;

protected:
	_uint				m_iPassIdx = 0;
	ID_UINT				m_iPrototypeID;
	ID_UINT				m_iLayerID;

protected:
	_uint				m_iTickGroup = 0;
	_uint				m_iLateTickGroup = 0;
	_float				m_fFrustumRange = 1.f;

protected:
	_bool				m_bIsRender = false;

protected:
	_float				m_fTimeRate = 1.f;

protected:
	HRESULT Add_Component(_uint iLevelIndex, ID_UINT iPrototypeID, ID_UINT iComponentID, class CComponent** ppOut, void* pArg = nullptr);
	HRESULT Remove_Component(ID_UINT iComponentID);
	HRESULT Change_Component(_uint iLevelIndex, ID_UINT iPrototypeID, ID_UINT iComponentID, class CComponent** ppOut, void* pArg = nullptr);
	_bool	IsInFrustum(void);

private:
	class CComponent* Find_Components(ID_UINT iComponentID);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END