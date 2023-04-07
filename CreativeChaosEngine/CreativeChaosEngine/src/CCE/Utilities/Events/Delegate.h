#pragma once
#include <functional>
#include "../../Analysis/Debug.h"

namespace CCE::Events
{
	class Delegate
	{
	public:

		Delegate()
		{}

		~Delegate()
		{
			bind = NULL;
			func = NULL;
			args = NULL;
		}
		
		void Invoke()
		{
			DASSERT(bind != NULL,
				"The reference object or the function pointer is null!");
			
		}

		void Bind(intptr_t _bind)
		{
			bind = _bind;
		}

		void Bind(intptr_t _bind, va_list _args)
		{
			bind = _bind;
			args = _args;
		}

	private:

		intptr_t bind = NULL;
		intptr_t func = NULL;
		va_list args = NULL;
	};
}
