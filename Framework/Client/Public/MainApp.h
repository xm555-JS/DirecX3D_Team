#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CRenderer;
class CGameInstance;
END

BEGIN(Client)
//class CGameInstance;
class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;
public:
	HRESULT Initialize();	
	void Tick(float fTimeDelta);
	HRESULT Render();

private:
	CGameInstance*			m_pGameInstance = nullptr;
	CRenderer*				m_pRenderer = nullptr;

private:
	_uint					m_iNumRender = 0;
	_tchar					m_szFPS[MAX_PATH] = TEXT("");
	_float					m_fTimeAcc = 0.f;

private:
	HRESULT Open_Level(LEVEL eLevelID);
	HRESULT Ready_Prototype_Component();
	HRESULT Ready_Prototype_Camera();
	HRESULT Ready_Prototype_Loading();

private:
	HRESULT Initialize_Engine();
	void Add_MapModelPrototypeFromOneFolder(string Path, _int eModelID, _matrix TransformMatrix);


public:
	static CMainApp* Create();
	virtual void Free();

};

END