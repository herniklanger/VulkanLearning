#pragma once

#include "pipeline.h"
#include "device.h"
#include "GaneObject.h"

#include <memory>
#include <vector>

namespace VulkanTest
{
	class SimpleRenderSystem
	{
	public:

		SimpleRenderSystem(Device &device, VkRenderPass renderPass);
		~SimpleRenderSystem();


		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;
		void RenderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject> &gameObjects);

	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);

		Device &device;

		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayour;
	};

}