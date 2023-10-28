#pragma once
#include "../Bindable/IBindable.h"
#include "../../../Manager/ProfilingManager.h"
#include "../../../Utilities/IO/IO.h"
#include "../../../Core.h"
#include "../Vertex.h"
#include "../Transform.h"
#include "IDrawable.h"
#include <memory>

namespace CCE::Graphics
{
	struct CCE_API Mesh : public IDrawable
	{
	public:
		Mesh(File _pixelShader, File _vertexShader);
		~Mesh()
		{
			UNREGISTER_LEAK_DETECT;
		}

		// From IDrawable
		void Draw();

		// TODO: Somehow implement a GUID for identification
		
	public:
		Transform transform = {};

		File pixelShader = {};
		File vertexShader = {};

	private:
		DirectX::XMMATRIX modelMatrix;
		std::shared_ptr<VSConstantBuffer<DirectX::XMMATRIX>> pMeshConstBuf = nullptr;
	};
}
