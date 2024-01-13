#pragma once
#include <vector>
#include <memory>
#include "../../Graphics/Rendering/Bindable/BindableCommon.h"

namespace CCE::ECS::Components
{
	struct Material
	{
	public:

		void StaticBind();
		void DynamicBind();

		void AddBind(std::shared_ptr<CCE::Graphics::IBindable> bind) noexcept;
		
	private:
		std::vector<std::shared_ptr<CCE::Graphics::IBindable>> materialBindPtrs = {};
	};
}
