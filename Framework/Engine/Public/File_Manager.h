#pragma once
#include "Base.h"

BEGIN(Engine)

class CFile_Manager final : public CBase
{
	DECLARE_SINGLETON(CFile_Manager)

public:
	CFile_Manager();
	virtual ~CFile_Manager() = default;

public:
	HRESULT Open_Save_Handle(HANDLE* hFile, LPCWSTR FilePath);
	HRESULT Open_Load_Handle(HANDLE* hFile, LPCWSTR FilePath);
	HRESULT Delete_File(LPCWSTR FilePath);
	wstring Data_Complete_Path(const wchar_t * _strFileName);



	HRESULT	Save_Json(const wchar_t* _strPath, json _json);
	HRESULT	Load_Json(const wchar_t* _strPath, json* pOut);
	wstring json_Complete_Path(const wchar_t* _strFileName);


private:
	vector<void*>			m_vecData;

public:
	virtual void Free() override;
};

END