#pragma once
#include <random>

namespace Engine
{
	template<typename T>
	void Safe_Delete(T& Temp)
	{
		if (Temp)
		{
			delete Temp;
			Temp = nullptr;
		}
	}

	template<typename T>
	unsigned long Safe_Release(T& Temp)
	{
		unsigned long dwRefCnt = 0;

		if (Temp)
		{
			dwRefCnt = Temp->Release();

			if(0 == dwRefCnt)
				Temp = nullptr;
		}

		return dwRefCnt;
	}

	template<typename T>
	unsigned long Safe_AddRef(T& Temp)
	{
		unsigned long dwRefCnt = 0;

		if (Temp)	
			dwRefCnt = Temp->AddRef();		

		return dwRefCnt;
	}


	template<typename T>
	void Safe_Delete_Array(T& Temp)
	{
		if (Temp)
		{
			delete[] Temp;
			Temp = nullptr;
		}
	}


	static int Random(int _a, int _b)
	{
		// �õ尪�� ��� ���� random_device ����.
		std::random_device rd;

		// random_device �� ���� ���� ���� ������ �ʱ�ȭ �Ѵ�.
		std::mt19937 gen(rd());

		// a ���� b ���� �յ��ϰ� ��Ÿ���� �������� �����ϱ� ���� �յ� ���� ����.
		std::uniform_int_distribution<int> dis(min(_a, _b), max(_a, _b));

		return dis(gen);
	}

	static float fRandom(float _a, float _b)
	{
		// �õ尪�� ��� ���� random_device ����.
		std::random_device rd;

		// random_device �� ���� ���� ���� ������ �ʱ�ȭ �Ѵ�.
		std::mt19937 gen(rd());

		// a ���� b ���� �յ��ϰ� ��Ÿ���� �������� �����ϱ� ���� �յ� ���� ����.
		std::uniform_real_distribution<float> dis(min(_a, _b), max(_a, _b));

		return dis(gen);
	}

	// abga
	static _uint ConvertColor(_float4 vColor)
	{
		//Convert color components to value between 0 and 255.
		_uint r = (_uint)(255.f * vColor.x);
		_uint g = (_uint)(255.f * vColor.y);
		_uint b = (_uint)(255.f * vColor.z);
		_uint a = (_uint)(255.f * vColor.w);

		//_uint r = 255;		//b?
		//_uint b = 0;		//r?
		//_uint g = 0;		//g
		//_uint a = 255;

		//Combine the color components in a single value of the form 0xAaBbGgRr
		//return 0xFF000000 | r | (b << 16) | (g << 8);
		return 0x00000000 | (a << 24) | (b << 16) | (g << 8) | r;
	}
}
