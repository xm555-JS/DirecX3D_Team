#pragma once

namespace Engine
{

	class CTag_Finder
	{
	private:
		const TCHAR*	m_pStr;

	public:
		CTag_Finder(const TCHAR* pStr)
			: m_pStr(pStr)
		{

		}

	public:
		template<typename T>
		bool	operator()(T& Pair)
		{
			if (!lstrcmp(m_pStr, Pair.first))
				return true;

			return false;
		}
	};

	class CID_Finder
	{
	private:
		unsigned int	m_iID;

	public:
		CID_Finder(unsigned int iID)
			: m_iID(iID)
		{

		}

	public:
		template<typename T>
		bool	operator()(T& Pair)
		{
			if (m_iID == Pair.first)
				return true;

			return false;
		}
	};


	static _float3 Get_MousePos(HWND _hWnd)
	{
		POINT pt{};

		GetCursorPos(&pt);
		ScreenToClient(_hWnd, &pt);

		return _float3(float(pt.x), float(pt.y), 0.f);
	}


	static std::wstring s2ws(const std::string& s)
	{
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	}


	static _bool IsFileExist(char *szFileFullPath)
	{
		return (_access(szFileFullPath, 0) != -1) ? true : false;
	}


	static std::wstring towstring(const std::string str)
	{
		std::wstring Dest; 
		Dest.assign(str.begin(), str.end());
		return Dest;
	}

	static std::string tostring(const std::wstring str)
	{
		std::string Dest;
		Dest.assign(str.begin(), str.end());
		return Dest;
	}

	typedef std::vector<std::string> stringvec;

	static void StringvecClear(stringvec& vec)
	{
		vec.clear();
		vec.swap(stringvec());
	}

	struct path_leaf_string
	{
		std::string operator()(const std::filesystem::directory_entry& entry) const
		{
			//return entry.path().leaf().string();
			return entry.path().filename().string();
		}
	};

	static void read_directory(const std::string& name, stringvec& v)
	{
		std::filesystem::path p(name);
		std::filesystem::directory_iterator start(p);
		std::filesystem::directory_iterator end;
		std::transform(start, end, std::back_inserter(v), path_leaf_string());
	}

}

