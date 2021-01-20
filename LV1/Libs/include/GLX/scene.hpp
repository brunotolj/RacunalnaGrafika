#pragma once
#include <GLX/object.hpp>
#include <vector>

namespace glx
{
	class Scene
	{
		Scene(Scene const&) = delete;
		Scene& operator=(Scene const&) = delete;

		std::vector<Object*> objects;
	public:
		Scene();
		~Scene();
	};
}