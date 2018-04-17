#pragma once
#include <vector>
#include "Go.h"
class GameObject;

	class Cam2 : public Go
	{
	public:
		Cam2();
		~Cam2();

		std::vector<int> proj;
	};


