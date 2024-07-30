#pragma once

#include "Component.h"
#include "Transform.h"

// Cell 을 생성하여 정보를 가지고 있다.
// 현재 Cell로 부터 이동 가능한지 아닌지를 판단해준다.

BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
public:
	typedef struct tagNavigationDesc
	{
		_int						m_iCurrentIndex = -1;
		class CGameObject*			pOwner = nullptr;

	}NAVIDESC;

private:
	ComPtr<ID3D11Device>							m_pDevice = nullptr;
	ComPtr<ID3D11DeviceContext>						m_pContext = nullptr;

private:
	CNavigation();
	CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pNavigationData);
	virtual HRESULT Initialize(void* pArg);

public:
	_bool					isMove(_fvector vPosition);
	_bool					isUpperThanPlane(CTransform* pTransform, _float* pHightOut = nullptr);
	_fvector				Get_LineSlidePos(_fvector vPosition_Prev, _fvector vPosition_Cur);
	_fvector				Get_Plane(CTransform* pTransform);
	_int					Get_CurCellIdx() { return m_NaviDesc.m_iCurrentIndex; }
	class CCell*			Get_CurCellDesc() { return m_Cells[m_NaviDesc.m_iCurrentIndex]; }
	vector<class CCell*>	Get_Cells() { return m_Cells; }

	void			Set_CurCellDesc(_uint iCurrentIdx) { m_NaviDesc.m_iCurrentIndex = iCurrentIdx; }

	void			SetUp_OnTerrain(CTransform* pTransform);
	void			SetUp_OnTerrain_Anim(CTransform* pTransform, _matrix vStartingPointWorldMatrix);
	void			Find_CurrentIndex(CTransform* pTransform);
	void			Make_LineNormal();

#ifdef _DEBUG
public:
	HRESULT Render();
#endif // _DEBUG



private:	
	vector<class CCell*>			m_Cells;
	typedef vector<class CCell*>	CELLS;

	NAVIDESC						m_NaviDesc;

private:
	HRESULT Make_Neighbor();

	void	Check_SameCell();

#ifdef _DEBUG
private:
	class CShader*					m_pShader = nullptr;
#endif // _DEBUG

public:
	static CNavigation* Create(const _tchar* pNavigationData);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END