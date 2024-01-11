#pragma once
#include <set>

namespace CCE::ECS::Systems
{
	struct Entity;
	class ECSSystem
	{
	private:
		struct cmp {
			bool operator() (int a, int b) const {
				return a < b;
			}
		};

	public:
		/// <summary>
		/// Each system has its own copy of all entites present in the system.
		/// </summary>
		std::set<long long> mEntities;
	};
}