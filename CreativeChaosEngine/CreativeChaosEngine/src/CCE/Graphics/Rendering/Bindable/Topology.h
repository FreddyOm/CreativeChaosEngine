#pragma once
#include "IBindable.h"
#include "../../../core.h"

namespace CCE::Graphics
{
	struct Topology : public IBindable
	{
	public:
		Topology(D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		~Topology() = default;
		// Geerbt über IBindable
		void DynamicBind() override;
		void StaticBind() override;

	private:
		D3D11_PRIMITIVE_TOPOLOGY pTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	};
}
