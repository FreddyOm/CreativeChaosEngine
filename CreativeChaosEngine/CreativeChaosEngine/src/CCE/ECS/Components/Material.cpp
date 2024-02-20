#include "Material.h"
#include "../../Analysis/Debug.h"
#include "../Manager/ProfilingManager.h"

namespace CCE::ECS::Components
{
	/// <summary>
	/// Binding once initially
	/// </summary>
	void Material::StaticBind()
	{

	}

	/// <summary>
	/// Binding every frame
	/// </summary>
	void Material::DynamicBind()
	{
		PROFILE_FUNCTION;
		for (auto& bind : materialBindPtrs)
		{
			bind->DynamicBind();
		}
	}

	/// <summary>
	/// Adds a bindable to the material binds.
	/// </summary>
	/// <param name="bind"></param>
	void Material::AddBind(std::shared_ptr<CCE::Graphics::IBindable> bind) noexcept
	{
		PROFILE_FUNCTION;
		materialBindPtrs.push_back(std::move(bind));
	}
}
