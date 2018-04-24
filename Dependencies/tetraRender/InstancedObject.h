#pragma once
#include "VertexBufferObject.h"
#include "GameObject.h"

namespace tetraRender {
	class InstancedObject : public GameObject
	{
	public:
		InstancedObject();
		~InstancedObject();

	private:
		std::vector<std::shared_ptr<Solid>> objects;
		
	};
}


