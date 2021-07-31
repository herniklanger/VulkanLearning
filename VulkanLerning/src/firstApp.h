#pragma once

#include "window.h"
#include "pipeline.h"
#include "device.h"
#include "swapChain.h"
#include "Model.h"
#include "GaneObject.h"

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
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void freeCommandBuffer();
		void drawFrame();
		void recreateSwapChain();
		void recordCommandBuffre(int imageIndex);
		void renderGameObjects(VkCommandBuffer commandBuffer);

		Window Window{ WIDTH, HEIGHT, "Hallow Vulkan!" };
		Device device{ Window };
		std::unique_ptr<SwapChain> swapChain;
		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayour;
		std::vector<VkCommandBuffer> commandBuffers;
		std::vector<GameObject> gameObjects;
	};

}