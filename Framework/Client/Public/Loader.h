#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CLoader : public CBase
{
protected:
	CLoader();
	virtual ~CLoader() = default;

public:
	CRITICAL_SECTION Get_CS() const {
		return m_CriticalSection;
	}

	LEVEL Get_NextLevelID() const {
		return m_eNextLevel;
	}

	_bool is_Finished() const {
		return m_isFinished;
	}

	const _tchar* Get_LoadingText() const {
		return m_szLoadingText;
	}

	CLoader*	Get_CurLoader() { return m_pCurLoader; }

public:
	HRESULT Initialize(LEVEL eNextLevel);

	HRESULT	Loading();

protected:
	_tchar					m_szLoadingText[MAX_PATH] = TEXT("");
	_bool					m_isFinished = false;
	CRITICAL_SECTION		m_CriticalSection;

	
protected:
	HRESULT	Create_CriticalSection();

	virtual HRESULT Loading_Object() { return S_OK; };
	virtual HRESULT Loading_Texture() { return S_OK; };
	virtual HRESULT Loading_Model() { return S_OK; };
	virtual HRESULT Loading_Shader() { return S_OK; };

private:
	LEVEL					m_eNextLevel = LEVEL_END;

private:
	HANDLE					m_hThread;

private:
	CLoader*				m_pCurLoader = nullptr;
	
public:
	static CLoader* Create(LEVEL eNextLevel);
	virtual void Free() override;
};

END