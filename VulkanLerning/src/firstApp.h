#pragma once

#include "window.h"
#include "device.h"
#include "Model.h"
#include "GaneObject.h"
#include "Renderer.h"

#include <memory>
#include <vector>

namespace VulkanTest 
{
	class FirstApp
	{
	public:

		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		FirstApp();
		~FirstApp();


		FirstApp(const FirstApp&) = delete;
		FirstApp& operator=(const FirstApp&) = delete;

		void run();
	private:
		void loadGameObjects();

		Window window{ WIDTH, HEIGHT, "Hallow Vulkan!" };
		Device device{ window };
		Renderer renderer{ window, device };

		std::vector<GameObject> gameObjects;
	};

}