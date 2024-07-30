#pragma once

#include "Base.h"

BEGIN(Engine)
class CGameObject;
class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	CComponent();
	CComponent(const CComponent& Prototype);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Render() { return S_OK; }


public:
	CGameObject*		Get_Owner() { return m_pOwner; }
	void				Set_Owner(CGameObject* pObject) {m_pOwner = pObject; }
	void				Set_ModelFileName(string ModelName) { m_strModelFileName = ModelName; }
	string				Get_ModelFileName(void) { return m_strModelFileName; }
	void				Set_PrototypeID(ID_UINT _iPrototypeID) { m_iPrototypeID = _iPrototypeID; }
	ID_UINT				Get_PrototypeID(void) { return m_iPrototypeID; }

protected:
	//ComPtr<ID3D11Device>				m_pDevice = nullptr;
	//ComPtr<ID3D11DeviceContext>		m_pContext = nullptr;
	_bool						m_isCloned = false;
	
	CGameObject*				m_pOwner = nullptr;

	// Model Component 일 경우. 파일 이름 가지고 있도록 만듬.
	string				m_strModelFileName;
	ID_UINT				m_iPrototypeID;

public:
	virtual CComponent* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
};

END