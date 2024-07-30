#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect_Instance final : public CVIBuffer
{
public:
	enum STARTPOINT_TYPE { POINT, CUBE, SPHERE };
	enum BILLBOARD_TYPE { NORMAL, BILLBOARD, DIRECTIONAL };
	enum MOVING_TYPE { SPEED, ACCELATION, POSITIONING };

protected:
	CVIBuffer_Rect_Instance();
	CVIBuffer_Rect_Instance(const CVIBuffer_Rect_Instance& rhs);
	virtual ~CVIBuffer_Rect_Instance() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	HRESULT Initialize_Pooling(void* pArg);
	HRESULT Initialize_Loop(void);
	virtual HRESULT Render();
	virtual _uint3 Get_Indices(_uint iIndex) const {
		return _uint3(((FACEINDICES16*)m_pIndices)[iIndex]._1,
			((FACEINDICES16*)m_pIndices)[iIndex]._2,
			((FACEINDICES16*)m_pIndices)[iIndex]._3);
	}

public:
	void Update(_float fTimeDelta);

private:
	ID3D11Buffer* m_pVBInstance = nullptr;
	_uint m_iInstanceStride = 0;
	_float m_fTimeAcc = 0.f;

	PARTICLE_DESC m_ParticleDesc;

	_float3* m_vStartScaleRUL = nullptr;
	//_float3* m_vCurrentScaleRUL = nullptr;
	
	_float3* m_vStartSpeedXYZ = nullptr;
	//_float3* m_vCurrentSpeedXYZ = nullptr;
	
	_float3* m_fEachAccelationXYZ = nullptr;
	_float3 m_fWholeAccelation = { 0.f, 0.f, 0.f };

	_float3* m_vStartRotateRUL = nullptr;
	//_float3* m_vCurrentRotateSpeedRUL = nullptr;
	_float3* m_fRotateAccelation = nullptr;


private:
	HRESULT Setting_Index(void);
	void	SetUp_ParticleDesc(PARTICLE_DESC Desc, VTXINSTANCE** pOutInstance);

public:
	static CVIBuffer_Rect_Instance* Create();
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END