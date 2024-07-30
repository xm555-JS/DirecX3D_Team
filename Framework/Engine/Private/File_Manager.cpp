#include "..\Public\File_Manager.h"

IMPLEMENT_SINGLETON(CFile_Manager)

#define	JSONFOLDER_PATH	L"../Bin/Resources/Json/"
#define	DATAFOLDER_PATH	L"../Bin/Resources/Data/"


CFile_Manager::CFile_Manager()
{
}




HRESULT CFile_Manager::Open_Save_Handle(HANDLE * hFile, LPCWSTR FilePath)
{
	// 파일 개방 함수
	*hFile = CreateFile(FilePath, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX("Fail to save File");
		__debugbreak();
	}

	return S_OK;

	//DWORD		dwByte = 0;
	//for (auto& iter : vecTemp)
	//{
	//	WriteFile(hFile, &(iter), DataSize, &dwByte, nullptr);
	//}

	//CloseHandle(hFile);
}


HRESULT CFile_Manager::Open_Load_Handle(HANDLE * hFile, LPCWSTR FilePath)
{
	//L"../Data/Tile.dat"
	// 파일 개방 함수
	*hFile = CreateFile(FilePath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == *hFile)
	{
		MSG_BOX("Fail to load File");
		__debugbreak();
	}

	return S_OK;

	//DWORD			dwByte = 0;
	//void*			pTemp;
	//while (true)
	//{
	//	ReadFile(hFile, &pTemp, DataSize, &dwByte, nullptr);
	//	if (0 == dwByte)
	//		break;
	//	m_vecData.push_back(pTemp);
	//}

	//for (auto& elem : m_vecData)
	//{
	//	vecTemp.push_back(elem);
	//}


	//CloseHandle(hFile);
}


HRESULT CFile_Manager::Delete_File(LPCWSTR FilePath)
{
	_int	iFile = DeleteFile(FilePath);

	if (0 > iFile)
	{
		MSG_BOX("Fail to Delete File");
		__debugbreak();
	}

	return S_OK;
}


wstring CFile_Manager::Data_Complete_Path(const wchar_t * _strFileName)
{
	wstring strFullPath = DATAFOLDER_PATH;
	strFullPath += _strFileName;
	strFullPath += L".dat";

	return strFullPath;
}


HRESULT CFile_Manager::Save_Json(const wchar_t * _strPath, json _json)
{
	ofstream fout;

	fout.open(_strPath);

	if (fout.is_open())
	{
		fout << _json << endl;
	}
	else
	{
		//LPWSTR	szMessage = L"Fail to Load Json / Path : ";
		//lstrcpyW(szMessage, _strPath);
		//MessageBox(0, szMessage, TEXT("System Error"), MB_OK);
		MSG_BOX("Fail to Save Json");
		__debugbreak();
	}

	fout.close();

	return S_OK;
}


HRESULT CFile_Manager::Load_Json(const wchar_t * _strPath, json * pOut)
{
	json JsonTemp;

	ifstream fin;

	fin.open(_strPath);

	if (fin.is_open())
	{
		fin >> (JsonTemp);
	}
	else
	{
		//char	szMessage[MAX_PATH] = "Fail to Load Json / Path : ";
		//char	szPath[MAX_PATH];
		//wcstombs_s(MAX_PATH, szPath, _strPath);
		//strcpy_s(szMessage, szPath);
		//MessageBoxA(0, szMessage, nullptr, MB_OK);
		MSG_BOX("Fail to Load Json");
		__debugbreak();
	}

	fin.close();
	*pOut = JsonTemp;

	return S_OK;
}

wstring CFile_Manager::json_Complete_Path(const wchar_t * _strFileName)
{
	wstring strFullPath = JSONFOLDER_PATH;
	strFullPath += _strFileName;
	strFullPath += L".json";

	return strFullPath;
}

void CFile_Manager::Free()
{
}
