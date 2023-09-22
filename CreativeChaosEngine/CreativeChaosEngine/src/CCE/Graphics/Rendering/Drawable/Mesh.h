#pragma once
#include "IDrawable.h"
#include "../../../Manager/ProfilingManager.h"
#include "../../../Core.h"
#include "../Vertex.h"
#include "../Transform.h"
#include <memory>

namespace CCE::Graphics
{
	template<typename C>
	class ConstantBuffer;
	template<typename C>
	class VSConstantBuffer;

	struct CCE_API Mesh : public IDrawable
	{
	public:
		Mesh();
		~Mesh()
		{
			UNREGISTER_LEAK_DETECT;
		}

		// From IDrawable
		void Draw();

		// TODO: Somehow implement a GUID for identification
		
	private:
		void CreateConstBufs();

	public:
		Transform transform = {};

	private:
		DirectX::XMMATRIX modelMatrix;
		std::shared_ptr<VSConstantBuffer<DirectX::XMMATRIX>> pMeshConstBuf = nullptr;
	};
}
