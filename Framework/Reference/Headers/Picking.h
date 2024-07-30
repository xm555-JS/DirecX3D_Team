#pragma once

#include "Base.h"

BEGIN(Engine)
class CVIBuffer;
class CTransform;

class CPicking final : public CBase
{
	DECLARE_SINGLETON(CPicking);
public:
	CPicking();
	virtual ~CPicking() = default;

public:
	HRESULT Initialize(HWND hWnd);
	void Compute_RayInWorldSpace();
	_float3 Get_RayDir(void) { return m_vRayDir; }
	_float3 Get_RayPos(void) { return m_vRayPos; }

	_bool Picking(CVIBuffer* pVIBuffer, CTransform* pTransform, _vector* vOut = nullptr, _float* fHight = nullptr);
	_bool Picking_Terrain(CVIBuffer* pVIBuffer, CTransform* pTransform, _vector* vOut = nullptr, _float* fHight = nullptr, _uint3* pOutIndices = nullptr);
	_bool Picking(CVIBuffer* pVIBuffer, _matrix WorlMatrix, _vector* vOut, _float* fHight);
	_bool Picking(CVIBuffer* pVIBuffer, CTransform* pTransform, _fvector vRayPos, _fvector vRayDir, _vector* vOut = nullptr, _float* pHight = nullptr);
	HRESULT Picking(_float _fHight, _vector * vOut);
	HRESULT Picking(_float _fHight, _fvector vRayPos, _fvector vRayDir, _vector* vOut);

private:
	HWND					m_hWnd;
	ComPtr<ID3D11Device>	m_pDevice = nullptr;

private:
	_float3				m_vRayDir, m_vRayPos;


public:
	virtual void Free() override;
};

END