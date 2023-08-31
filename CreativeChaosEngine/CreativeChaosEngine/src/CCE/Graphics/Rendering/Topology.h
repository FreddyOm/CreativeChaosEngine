#pragma once
#include "IBindable.h"
#include "../../Core.h"
#include "D3D11.h"

namespace CCE::Graphics
{
	struct CCE_API Topology : public IBindable
	{
	public:
		Topology(D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		~Topology() = default;
		// Geerbt über IBindable
		void Bind() override;

	private:
		D3D11_PRIMITIVE_TOPOLOGY pTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	};
}
