#pragma once

#define D3DCOLOR_ABGR(a,r,g,b) \
    ((D3DCOLOR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

#define GET_INSTANCE(CLASSNAME)	[](){											\
	CLASSNAME*	pInstance = CLASSNAME::Get_Instance();							\
	if(nullptr == pInstance) {													\
	char	szMessage[MAX_PATH] = "";											\
	strcpy_s(szMessage, typeid(CLASSNAME).name());								\
	strcat_s(szMessage, "is nullptr");											\
	MessageBoxA(0, szMessage, nullptr, MB_OK);}									\
	else {																		\
	pInstance->AddRef();}														\
	return pInstance;															\
	}();

#define RELEASE_INSTANCE(CLASSNAME)	[](){										\
	CLASSNAME*	pInstance = CLASSNAME::Get_Instance();							\
	if(nullptr == pInstance) {													\
	char	szMessage[MAX_PATH] = "";											\
	strcpy_s(szMessage, typeid(CLASSNAME).name());								\
	strcat_s(szMessage, "is nullptr");											\
	MessageBoxA(0, szMessage, nullptr, MB_OK);}									\
	else {																		\
	pInstance->Release();}														\
	}();

#define	MGR(CLASSNAME)		CLASSNAME::Get_Instance()

#ifdef ENGINE_EXPORTS
#define ENGINE_DLL _declspec(dllexport)
#else
#define ENGINE_DLL _declspec(dllimport)
#endif

#define MSG_BOX(MESSAGE) MessageBox(0, TEXT(MESSAGE), TEXT("System Error"), MB_OK)
#define MSG_BOX_USER(TITLE_MESSAGE, MESSAGE) MessageBox(0, (MESSAGE), (TITLE_MESSAGE), MB_OK)

#define JSONFILEPATH(filename)		pGameInstance->json_Complete_Path(TEXT(filename)).c_str()
#define JSONFILEPATHARG(Arg)		pGameInstance->json_Complete_Path(Arg).c_str()

#define DATAFILEPATH(filename)		pGameInstance->Data_Complete_Path(TEXT(filename)).c_str()
#define DATAFILEPATHARG(Arg)		pGameInstance->Data_Complete_Path(Arg).c_str()

#define BEGIN(NAMESPACE) namespace NAMESPACE { 
#define END }

#define		USEGAMEINSTANCE			CGameInstance* pGameInstance = CGameInstance::Get_Instance()
#define		GAMEINSTANCE			CGameInstance::Get_Instance()

// Device
#define		DEVICE					pGameInstance->Get_Device()
#define		CONTEXT					pGameInstance->Get_Context()

//Debug
#define		COUT_FLOAT3(vector)		cout << vector.x << " / " << vector.y << " / " << vector.z << " / " << endl
#define		COUT_FLOAT4(vector)		cout << vector.x << " / " << vector.y << " / " << vector.z << " / " << vector.w << endl
#define		COUT_VECTOR(vector)		cout << XMVectorGetX(vector) << " / " << XMVectorGetY(vector) << " / " << XMVectorGetZ(vector) << " / " << XMVectorGetW(vector) << endl
#define		COUT_CAM_POS_LOOKPOS(vector1, vector2)		cout <<"Pos: " XMVectorGetX(vector1) << ", " << XMVectorGetY(vector1) << ", " << XMVectorGetZ(vector1) <<\
 " /// Look: " << XMVectorGetX(vector1) + XMVectorGetX(vector2) << ", " << XMVectorGetY(vector1) + XMVectorGetY(vector2) << ", " << XMVectorGetZ(vector1) + XMVectorGetZ(vector2) << endl

#define		FAILEDCHECK(failed)		if (FAILED(failed)) __debugbreak()
#define		NULLCHECK(nullcheck)	if (nullptr == (nullcheck)) __debugbreak()

// Key_Manager
#define		VK_MAX			0xff
#define		KEY_PRESSING(key)		pGameInstance->Key_Pressing(key)
#define		KEY_DOWN(key)			pGameInstance->Key_Down(key)
#define		KEY_UP(key)				pGameInstance->Key_Up(key)
#define		KEY_NONE(key)			pGameInstance->Key_None(key)
#define		KEY_TOGGLE(key)			pGameInstance->Key_Toggle(key)

#define		MAKE_KEY_DOWN(key)		pGameInstance->Make_Key_Down(key)
#define		MAKE_KEY_UP(key)		pGameInstance->Make_Key_Up(key)
#define		MAKE_TOGGLE_ON(key)		pGameInstance->Make_Toggle_On(key)
#define		MAKE_TOGGLE_OFF(key)	pGameInstance->Make_Toggle_Off(key)

#define		KEY_LOCK(key)			pGameInstance->Key_Lock(key)
#define		KEY_UNLOCK(key)			pGameInstance->Key_Unlock(key)
#define		ALL_KEY_LOCK			pGameInstance->All_Key_Lock()
#define		ALL_KEY_UNLOCK			pGameInstance->All_Key_Unlock()

// Input_Manager
#define		MOUSE_PRESSING(key)		pGameInstance->Get_DIMouseKeyState(key)

// EventMessege
#define		DELETE_EVENTMESSEGE(EventType)					\
for (auto iter = m_Events.begin(); iter != m_Events.end();)	\
{															\
	EVENTDESC	tEvent = *iter;								\
	if (EventType == tEvent.eEventType)						\
		iter = m_Events.erase(iter);						\
	else													\
		++iter;												\
}


#define		READ_EVENTMESSEGE(EventType, Excute)			\
for (auto iter = m_Events.begin(); iter != m_Events.end();)	\
{															\
	EVENTDESC	tEvent = *iter;								\
	if (EventType == tEvent.eEventType)						\
	{														\
		Excute;												\
		iter = m_Events.erase(iter);						\
	}														\
	else													\
		++iter;												\
}

// 일정 시간동안 계속 울림
#define DECL_TIMER(fkeyName) float fkeyName = 0.f
#define RESET_TIMER(fkeyName) fkeyName = 0.f
#define IMPL_TIMER(fkeyName, Function, fTime, fTimeDelta)	\
if(fkeyName < fTime)										\
{															\
	Function;												\
	fkeyName += fTimeDelta;									\
}


// 일정 시간이 끝나면 계속 울림
#define DECL_TIMER_END(fkeyName) float fkeyName = 0.f
#define RESET_TIMER_END(fkeyName) fkeyName = 0.f
#define IMPL_TIMER_END(fkeyName, Function, fTime, fTimeDelta)	\
fkeyName += fTimeDelta;											\
if(fkeyName > fTime)											\
{																\
	Function;													\
}

// 주어진 기간 동안 반복적으로 울림
#define DECL_ALARM(fkeyName) float fkeyName = 0.f
#define RESET_ALARM(fkeyName) fkeyName = 0.f
#define IMPL_ALARM(fkeyName, Function, fTime, fTimeDelta)	\
fkeyName += fTimeDelta;										\
if(fkeyName > fTime)										\
{															\
	Function;												\
	fkeyName = 0.f;											\
}

//// 주어진 기간이 끝나면 한번만 울림
//#define DECL_TIMER_ONCE(fkeyName) float fkeyName = 0.f
//#define RESET_TIMER_ONCE(fkeyName) fkeyName = 0.f
//#define IMPL_TIMER_ONCE(fkeyName, Function, fTime, fTimeDelta)	\
//fkeyName += fTimeDelta;										\
//if(0 > (fTime - fkeyName) * fkeyName)							\
//{																\
//	Function;													\
//	fkeyName *= -1.f;											\
//}

// 딱 한번만 울림
#define DECL_ONCE(bkeyName)	bool bkeyName = true
#define RESET_ONCE(bkeyName) bkeyName = true
#define IMPL_ONCE(bkeyName, Function)					\
if(bkeyName)											\
{														\
	Function;											\
	bkeyName = false;									\
}

#define NO_COPY(ClassName)								\
ClassName(const ClassName&) = delete;					\
ClassName& operator=(const ClassName&) = delete;

#define DECLARE_SINGLETON(ClassName)					\
		NO_COPY(ClassName)								\
public :												\
	static ClassName* Get_Instance();					\
	static unsigned long Destroy_Instance();			\
private:												\
	static ClassName* m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)					\
ClassName* ClassName::m_pInstance = nullptr;			\
ClassName* ClassName::Get_Instance()					\
{														\
	if (nullptr == m_pInstance)							\
		m_pInstance = new ClassName;					\
	return m_pInstance;									\
}														\
unsigned long ClassName::Destroy_Instance()				\
{														\
	return Safe_Release(m_pInstance);					\
}


#define		_WATER
#define		_SSR				// Water Reflaction
#define		_SHADOW
#define		_OUTLINE_UNIT
#define		_HDR				// High Quality Render 
#define		_TESSELLATION		// Terrain Detail Render
