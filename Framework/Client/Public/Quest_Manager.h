#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)
class CQuest;

class CQuest_Manager : public CBase
{
public:
	DECLARE_SINGLETON(CQuest_Manager)

private:
	CQuest_Manager();
	virtual ~CQuest_Manager() = default;

public:
	_uint Get_CurQuset()
	{
		if (m_Questlist.size())
		{
			return *m_Questlist.begin();
		}
		else
		{
			return QUEST_END;
		}
	}
	_bool Get_IsQuest() { return m_bIsQuest; }

public:
	HRESULT		Initialize();

public:
	void	Receive_Quest(_uint iCurLevel);
	void	Achieve_Quest();
	void	Complete_Quest();

public:
	//virtual void	On_EventMessege(EVENTDESC tEvent) override;

private:
	list<QUEST_TYPE> m_Questlist;
	_bool			 m_bIsQuest = false;
	CQuest*			 m_pCurQuest = nullptr;

public:
	virtual void Free();
};

END