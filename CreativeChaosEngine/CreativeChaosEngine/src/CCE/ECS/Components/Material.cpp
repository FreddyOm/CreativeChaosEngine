#include "Material.h"
#include "../../Analysis/Debug.h"


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
		materialBindPtrs.push_back(std::move(bind));
	}
}
