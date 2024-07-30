#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain_Custom final : public CVIBuffer
{
protected:
	CVIBuffer_Terrain_Custom();
	CVIBuffer_Terrain_Custom(const CVIBuffer_Terrain_Custom& rhs);
	virtual ~CVIBuffer_Terrain_Custom() = default;

public:
	virtual _uint3 Get_Indices(_uint iIndex) const {
		return _uint3(((FACEINDICES32*)m_pIndices)[iIndex]._1,
			((FACEINDICES32*)m_pIndices)[iIndex]._2,
			((FACEINDICES32*)m_pIndices)[iIndex]._3);
	}

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pHeightMapFilePath);
	virtual HRESULT Initialize(void* pArg) override;

private:
	_ulong					m_iNumVerticesX, m_iNumVerticesZ;

public:
	static CVIBuffer_Terrain_Custom* Create(const _tchar* pHeightMapFilePath);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END