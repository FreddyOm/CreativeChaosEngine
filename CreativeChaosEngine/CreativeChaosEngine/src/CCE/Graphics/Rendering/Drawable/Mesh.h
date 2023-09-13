#pragma once
#include "IDrawable.h"
//#include "../Bindable/IBindable.h"
#include "../../../Manager/ProfilingManager.h"
#include "../../../Core.h"
#include "../Vertex.h"
#include "../Transform.h"

namespace CCE::Graphics
{
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
		ComPtr<ID3D11Buffer> pPerObjectConstBuf = nullptr;
	};
}
