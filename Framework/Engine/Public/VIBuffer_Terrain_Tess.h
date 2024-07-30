#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain_Tess final : public CVIBuffer
{
private:
	CVIBuffer_Terrain_Tess();
	CVIBuffer_Terrain_Tess(const CVIBuffer_Terrain_Tess& rhs);
	virtual ~CVIBuffer_Terrain_Tess() = default;

public:
	virtual _uint3 Get_Indices(_uint iIndex) const {
		return _uint3(((FACEINDICES32*)m_pIndices)[iIndex]._1,
			((FACEINDICES32*)m_pIndices)[iIndex]._2,
			((FACEINDICES32*)m_pIndices)[iIndex]._3);
	}

	virtual HRESULT Initialize_Prototype(const _tchar * pHeightMapFilePath);
	virtual HRESULT Initialize(void* pArg);

private:
	_ulong					m_iNumVerticesX, m_iNumVerticesZ;
	FACEINDICES32*	m_pFaceIndices = nullptr;


public:
	static CVIBuffer_Terrain_Tess* Create(const _tchar * pHeightMapFilePath);
	virtual CComponent* Clone(void* pArg);
	virtual void Free();
};

END