#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Trail final : public CVIBuffer
{
public:
	typedef struct tagTrailDesc
	{
		_uint		iNumTrail = 1; // 사각형 버퍼의 갯수.
		_float4		vLocalSwordLow = { 0.f, 0.f, 0.f, 0.f };
		_float4		vLocalSwordHigh = { 0.f, 0.f, 0.f, 0.f };
		_float4x4	matOwnerWorld;
		_float		fCreateTrailTimer = 0.01f;
		

		tagTrailDesc(_uint _iNumTrail = 1, _float4 _vLocalSwordLow = { 0.f, 0.f, 0.f, 0.f }, _float4 _vLocalSwordHigh = { 0.f, 0.f, 0.f, 0.f }, _matrix _matOwnerWorld = XMMatrixIdentity(), _float _fCreateTrailTimer = 0.01f)
			: iNumTrail(_iNumTrail)
			, vLocalSwordLow(_vLocalSwordLow)
			, vLocalSwordHigh(_vLocalSwordHigh)
			, fCreateTrailTimer(_fCreateTrailTimer)
		{
			XMStoreFloat4x4(&matOwnerWorld, _matOwnerWorld);
		}
	}TRAIL_DESC;

protected:
	CVIBuffer_Trail();
	CVIBuffer_Trail(const CVIBuffer_Trail& rhs);
	virtual ~CVIBuffer_Trail() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	//HRESULT Initialize_Pooling(void* pArg);
	virtual HRESULT Render();

	virtual _uint3 Get_Indices(_uint iIndex) const {
		return _uint3(((FACEINDICES16*)m_pIndices)[iIndex]._1,
			((FACEINDICES16*)m_pIndices)[iIndex]._2,
			((FACEINDICES16*)m_pIndices)[iIndex]._3);
	}

public:
	void Update(_float fTimeDelta, TRAIL_DESC& _stTrailDesc);

private:
	//ID3D11Buffer*	m_pVBInstance = nullptr;
	//_uint			m_iInstanceStride = 0;

	_float			m_fTimeAcc = 0.f;
	TRAIL_DESC		m_stTrailDesc;
	_uint			m_iVtxCount = 0;

private:
	//HRESULT Setting_Index(void);
	//void	SetUp_ParticleDesc(PARTICLE_DESC Desc, VTXINSTANCE * *pOutInstance);

public:
	static CVIBuffer_Trail* Create();
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END