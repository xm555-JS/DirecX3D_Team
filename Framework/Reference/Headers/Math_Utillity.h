#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CMath_Utillity final : public CBase
{
public:
	CMath_Utillity();
	~CMath_Utillity();

public:
	static _matrix		Rotate_MatrixFixedY(_matrix Matrix);
	static _float3		Get_PitchYawRollFromRotationMatrix(_matrix Matrix);

	static _bool		isUpperThanPlane(_fvector vPt0, _fvector vPt1, _fvector vPt2, _fvector vTargetPos, _float* pHightOut = nullptr);
	//static void	VectorFromPoint(_vector vPoint0, _vector vPoint1);
//	static HRESULT		WorldToScreen(D3DXVECTOR3* pos, D3DXVECTOR3* out);
//	static HRESULT		ScreenToWorld(D3DXVECTOR3* pos, D3DXVECTOR3* out);
//
//	static HRESULT		Compute_RayInWorldSpace(RAY* _Out, _float _fLength);
//	static RAY			Get_MouseRayInWorldSpace();
//	static _bool		Picking_VIBuffer(class CVIBuffer* pVIBuffer, class CTransform* pTransform, RAY _Ray, _float3* pOut);
//	static _bool		Picking_Mesh
//	(
//		LPD3DXBASEMESH pMesh,
//		CTransform* pTransform,
//		RAY _Ray,
//		_float3* pOut,
//		DWORD* pFaceIndex = nullptr,
//		FLOAT* pU = nullptr, 
//		FLOAT* pV = nullptr,
//		LPD3DXBUFFER* ppAllHits = nullptr, 
//		DWORD* pCountOfHits = nullptr
//	);
//
//public:
//	static _float4x4 Get_Rotation_Matrix(const _float4x4& _Mat);
//	static _float4x4 Get_Scale_Matrix(const _float4x4& _Mat);
//	static _float4x4 Get_Position_Matrix(const _float4x4& _Mat);
//
//	static _float4x4 Rotate_Matrix(const _float4x4& _Mat, const _float3& vAxis, const _float& fRadian);
//	static _float4x4 Set_Scale_Matrix(const _float4x4& _Mat, const _float3& _vScale);
//
//public:
//	static _float3 Rotate_Vec3(const _float3& _Rotation, const _float3& _Vec3);
//	static _float Get_Distance(const _float3& _LeftVector, const _float3& _RightVector);
//
//public:
//	template<typename T>
//	static T Get_Random_Value_In_Vector(vector<T> _Vector)
//	{
//		if (_Vector.empty())
//			return T();
//
//		_uint _index = rand() % _Vector.size();
//
//		return _Vector[_index];
//	}
//
//public:
//	static _float fLerp(_float LeftSide, _float RightSide, _float fRatio);
//	//static _float fSlerp(_float LeftSide, _float RightSide, _float fRatio);
//	static _float3 vecLerp(_float3 LeftSide, _float3 RightSide, _float fRatio);
//	static _float3 Slerp(_float3 LeftSide, _float3 RightSide, _float fRatio);
//	static DWORD FtoDw(float f);
//	static _float Get_RandomFloat(_float LowBound, _float HighBound);
};

END