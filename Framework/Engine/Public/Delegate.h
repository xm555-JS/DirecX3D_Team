#pragma once

// https://kukuta.tistory.com/207

// 같은 파라미터를 공유하는 함수들을 리스트에 넣고 호출 할 수 있다
// 마치 Delegate 자료형처럼 Delegate 변수를 만들어서 사용한다.
// Delegate<>	delegate;
// 이벤트 매니저가 Delegate 객체 가지고 적절한 타이밍에 호출해주면 좋을 듯
// 대표적인 예로 tick 함수 등을 등록해두는 방식이 있다.
// 일정 시간이나 특정 이벤트가 발생할때까지, 발생 하면 Delegate에 등록(Push_back)해두는 식으로 하면 될듯

template <class... ARGS>
class FDelegate
{
public:
	typedef typename std::list<std::function<void(ARGS...)>>::iterator iterator;

	void operator () (const ARGS... args)
	{
		for (auto& func : functions)
		{
			func(args...);
		}
	}

	FDelegate& operator += (std::function<void(ARGS...)> const& func)
	{
		functions.push_back(func);
		return *this;
	}

	FDelegate& operator -= (std::function<void(ARGS...)> const& func)
	{
		void(* const* func_ptr)(ARGS...) = func.template target<void(*)(ARGS...)>();
		const std::size_t func_hash = func.target_type().hash_code();

		if (nullptr == func_ptr)
		{
			for (auto iter = functions.begin(); iter != functions.end(); iter++)
			{
				if (func_hash == (*iter).target_type().hash_code())
				{
					functions.erase(iter);
					return *this;
				}
			}
		}

		else
		{
			for (auto iter = functions.begin(); iter != functions.end(); iter++)
			{
				void(* const* delegate_ptr)(ARGS...) = (*iter).template target<void(*)(ARGS...)>();
				if (nullptr != delegate_ptr && *func_ptr == *delegate_ptr)
				{
					functions.erase(iter);
					return *this;
				}
			}
		}

		return *this;
	}

	iterator begin()
	{
		return functions.begin();
	}

	iterator end()
	{
		return functions.end();
	}

	void Clear()
	{
		functions.clear();
	}


private:
	std::list<std::function<void(ARGS...)>> functions;
};