#pragma once
#include <vector>
#include <memory>
#include "../../graphics/rendering/bindable/bindable-common.h"

namespace CCE::ECS::Components
{
	struct Material
	{
	public:

		Material() = default;
		~Material() = default;

		void StaticBind();
		void DynamicBind();

		void AddBind(std::shared_ptr<CCE::Graphics::IBindable> bind) noexcept;
		
	public:

		DirectX::XMFLOAT4 BaseColor = { 1, 1, 1, 1};

	private:
		std::vector<std::shared_ptr<CCE::Graphics::IBindable>> materialBindPtrs = {};
	};
}
