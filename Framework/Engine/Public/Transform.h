#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	typedef struct tagTransformDesc
	{
	//	_float			fSpeedPerSec;
	//	_float			fRotationPerSec;
		ID_UINT			tTransformID;
	}TRANSFORMDESC;

public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_TRANSLATION, STATE_END };

public:
	CTransform();
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	_vector				Get_State(STATE eState) { return XMLoadFloat4x4(&m_WorldMatrix).r[eState]; }
	_matrix				Get_WorldMatrix() { return XMLoadFloat4x4(&m_WorldMatrix); }
	_float4x4*			Get_WorldMatrix_ptr() { return &m_WorldMatrix; }
	_float4x4*			Get_CombinedTransformationMatrix_ptr() { return &m_CombinedTransformationMatrix; }
	TRANSFORMDESC*		Get_TransformDesc() { return &m_TransformDesc; }
	_float3				Get_RotateRate() { return m_vRotateRate; }
	_vector				Get_Quaternion();
	_float3				Get_Scaled();
	_float				Get_JumpYDiff() { return m_fJumpYDiff; }
	_float				Get_Gravity() { return m_fGravity; }
	_bool				Get_YFixState() { return m_bYFixed; }
	_bool				Get_Skate() { return m_bIsInWater_Skate; }

	// 이상함 뭔가 안되는듯
	_matrix Get_RotationMatrix();

	CTransform* Get_PerantTransform() { return m_pParentTransform; }

	void	Set_WorldMatrix(_matrix WorldMatrix) { XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix); }

	void Set_State(STATE eState, _fvector vState)
	{
		_float4		vData;
		XMStoreFloat4(&vData, vState);
		memcpy(&m_WorldMatrix.m[eState], &vData, sizeof(_float4));
		if (STATE_TRANSLATION == eState)
			m_WorldMatrix._44 = 1.f;
	}

	void	Set_Offset(_vector vOffset);

public:
	void	Set_TransformDesc(const TRANSFORMDESC* pTransformDesc) { m_TransformDesc = *pTransformDesc; }
	void	Set_RotateRate(_fvector vRotateRate) { XMStoreFloat3(&m_vRotateRate, vRotateRate); }
	void	Set_Scaled(_float3 vScale);
	void	Set_PerantTransform(CTransform* pTransform) { m_pParentTransform = pTransform; }
	void	Set_RootBoneMatrix(_float4x4 RootBonerMatrix, class CNavigation* pNaviCom = nullptr);
	void	Set_RootBoneMatrix_Rot(_float4x4 RootBonerMatrix, class CNavigation* pNaviCom = nullptr);
	void	SetBilBoard();
	void	SetBilBoard_CamLook();
	void	Set_NavigationCom(class CNavigation* pNavigation) { m_pNavigation = pNavigation; }
	void	Set_AirState(_bool bState) { m_bIsInAir = bState; }
	void	Set_WaterState(_bool bState) { m_bIsInWater = bState; }
	void	Set_Gravity(_float fGravity) { m_fGravity = fGravity; }
	void	Set_YFixState(_bool bState) { m_bYFixed = bState; }
	void	Set_RotationMatrix(_matrix RotateMatrix);
	void	Set_Skate(_bool bState) { m_bIsInWater_Skate = bState; }


	void Reset_RootBoneDiff() {
		XMStoreFloat3(&m_vPreRootPos, XMVectorSet(0.f, 0.f, 0.f, 0.f));
		m_bRootBoneReset = true;
	}

	void Reset_RootBoneDiff_Mat() {
		XMStoreFloat4x4(&m_vPreRootMatrix, XMMatrixIdentity());
		m_bRootBoneReset = true;
	}


	void Reset_FrontAccTime() { m_fFrontAccTime = 0.f; }
	void Reset_StopAccTime() { m_fStopAccTime = 0.f; }
	void Reset_StraightAccTime() { m_fStraightAccTime = 0.f; }

	_bool	Is_InAir() { return m_bIsInAir; }
	_bool	Is_InWater() { return m_bIsInWater; }
	_bool	Is_InSkate() { return m_bIsInWater_Skate; }
	_bool	Is_InGlide() { return m_bIsInAir_Glide; }

public:
	HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual	void Tick(_float fTimeDelta);


public:
	HRESULT Set_ShaderResource(class CShader* pShader, const char* pConstantName);

public:
	HRESULT Go_Straight(_float fTimeDelta, _float fMoveSpeed, class CNavigation* pNaviCom = nullptr);
	HRESULT Go_Front(_float fTimeDelta, _float fMoveSpeed, class CNavigation* pNaviCom = nullptr);
	HRESULT Go_Backward(_float fTimeDelta, _float fMoveSpeed, class CNavigation* pNaviCom = nullptr);
	HRESULT Go_Back(_float fTimeDelta, _float fMoveSpeed, class CNavigation* pNaviCom = nullptr);
	HRESULT Go_Left(_float fTimeDelta, _float fMoveSpeed, class CNavigation* pNaviCom = nullptr);
	HRESULT Go_Right(_float fTimeDelta, _float fMoveSpeed, class CNavigation* pNaviCom = nullptr);
	HRESULT Go_Up(_float fTimeDelta, _float fMoveSpeed, class CNavigation* pNaviCom = nullptr);
	HRESULT Go_Down(_float fTimeDelta, _float fMoveSpeed, class CNavigation* pNaviCom = nullptr);


	HRESULT Go_AccFront(_float fTimeDelta, _float fMoveSpeed, class CNavigation* pNaviCom = nullptr);
	HRESULT Go_AccFront_Limit(_float fTimeDelta, _float fMoveSpeed, _float fLimitSpeed, class CNavigation* pNaviCom = nullptr, _float* fOut = nullptr);
	HRESULT Go_AccFront_Stop(_float fTimeDelta, _float fFriction, _float fMoveSpeed, class CNavigation* pNaviCom = nullptr, _float* fOut = nullptr);
	HRESULT Go_AccStraight(_float fTimeDelta, _float fMoveSpeed, class CNavigation* pNaviCom = nullptr);
	HRESULT Go_AccStraight_Limit(_float fTimeDelta, _float fMoveSpeed, _float fLimitSpeed, class CNavigation* pNaviCom = nullptr, _float* fOut = nullptr);
	HRESULT Go_AccStraight_Stop(_float fTimeDelta, _float fFriction, _float fMoveSpeed, class CNavigation* pNaviCom = nullptr, _float* fOut = nullptr);
	HRESULT Go_Hight(_float fTimeDelta, _float fMoveSpeed, _float _fHight, class CNavigation* pNaviCom = nullptr);
	HRESULT Go_Drop(_float fTimeDelta, _float fMoveSpeed, _float _fHight, _float _fEndHight, class CNavigation* pNaviCom = nullptr);

	//FrostDragonTest
	HRESULT Go_Around(_float4 vSpot);
	HRESULT OrbitY(_float4 StartPos, _float4 ParentsPos, _float fSpeed);
	HRESULT Chase_Around(_float fTimeDelta, _float fMoveSpeed, _uint iTime, _float4 vSpot);
	void	Easing_LookAt(_fvector vTargetPos , _float fTimeDelta, _float fMoveSpeed, _float fEndTime);

	void	Jump(_float fPower, class CNavigation* pNaviCom, _float fGravity = 15.f);
	void	Glide(_float fPower, class CNavigation* pNaviCom, _float fGravity = 15.f);

	void	Swim(_float fSwimY, class CNavigation* pNaviCom);
	void	Skate(_float fMinY, class CNavigation* pNaviCom);

	void Turn(_fvector vAxis, _float fTimeDelta, _float fMoveSpeed);
	void Turn(_fvector vAxis, _float fRadian);
	void Turn(_matrix RotationMatrix);
	void TurnAngles(_fvector vAngles);
	void Rotation(_fvector vAxis, _float fRadian);
	void RotationAngles(_fvector vAngles);
	void MatrixTurnQuaternion(_fvector vQuaternion);
	void MatrixRotationQuaternion(_fvector vQuaternion);
	void LookAt(_fvector vTargetPos);
	void LookTo(_fvector vTargetLook);
	void YFixedLookAt(_fvector vTargetPos);
	void YFixedLook(_fvector vTargetLook);
	void UpNaviLook(_fvector vTargetLook, class CNavigation* pNaviCom = nullptr);
	void UpTo(_fvector vTargetUp);
	void Chase(_fvector vTargetPos, _float fTimeDelta, _float fMoveSpeed, _float fLimit = 0.f, class CNavigation* pNaviCom = nullptr);
	void MoveTo(_vector vPos, class CNavigation* pNaviCom);


private:
	void	Update_CombinedTransformationMatrix();
	//void	Check_Navigation(class CNavigation* pNavigation);

private:
	_float4x4				m_vPreRootMatrix;



private:
	_float4x4				m_WorldMatrix;
	_float4x4				m_RootBoneMatrix;
	_float3					m_vPreRootPos = { 0.f, 0.f, 0.f };
	_bool					m_bRootBoneReset = false;

	_float4x4				m_CombinedTransformationMatrix;


	TRANSFORMDESC			m_TransformDesc;
	static ID_UINT			m_tTransformID;
	_float3					m_vRotateRate = {0.f, 0.f, 0.f};

	CTransform*				m_pParentTransform = nullptr;
	//Front Acc
	_float					m_fFrontAccTime = 0.f;
	_float					m_fStopAccTime = 0.f;
	_float					m_fStraightAccTime = 0.f;
	_float					m_fNavigationAccTime = 0.f;


private: /* For Jump*/
	class CNavigation*		m_pNavigation = nullptr;
	_float					m_fPower = 0.f;
	_float					m_fGravity = 0.f;
	_float					m_fTimeAcc = 0.f;
	_bool					m_bIsInAir = false;
	_bool					m_bIsInAir_Glide = false;
	_bool					m_bIsInWater = false;
	_bool					m_bIsInWater_Skate = false;
	_float					m_fJumpYDiff = 0.f;
	_bool					m_bYFixed = false;
	_bool					m_bResetForce = false;


private: /* For Jump*/
	void		Move_JumpPos(_float fTimeDelta, _float fPower, class CNavigation* pNaviCom, _float fGravity);


public:
	static CTransform* Create();
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END