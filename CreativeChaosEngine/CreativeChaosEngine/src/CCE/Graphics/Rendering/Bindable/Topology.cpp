#include "Topology.h"

namespace CCE::Graphics
{
	/// <summary>
	/// The constructor of the topology bindable. Defaults the topology type to TRAINGLELIST.
	/// </summary>
	/// <param name="topology">The topology to use.</param>
	Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY topology)
		: pTopology(topology)
	{ }

	/// <summary>
	/// Binds the provided topology to the pipeline.
	/// </summary>
	void Topology::Bind()
	{
		GetContext()->IASetPrimitiveTopology(pTopology);
	}
}
