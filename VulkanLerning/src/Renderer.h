#pragma once

#include "window.h"
#include "device.h"
#include "swapChain.h"

#include <memory>
#include <vector>
#include <cassert>

namespace VulkanTest
{
	class Renderer
	{
	public:


		Renderer(Window& window, Device& device);
		~Renderer();


		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		int GetFrameIndex() const {
			assert(isFrameStarted && "Cannot get frame index when  not in progress");
			return currentFrameIndex;
		}

		VkCommandBuffer BeginFrame();
		void EndFrame();
		void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void EndSwapChainRenderPass(VkCommandBuffer commandBuffer);

		VkRenderPass getSwapChainRenderPass() const { return swapChain->getRenderPass(); }
		bool IsFrameInProgress() const { return isFrameStarted; }

		VkCommandBuffer getCurrentCommandBuffer() const {
			assert(isFrameStarted && "Connot get command buffer when frame not in progress");
			return commandBuffers[currentFrameIndex];
		}

	private:
		void createCommandBuffers();
		void freeCommandBuffer();
		void drawFrame();
		void recreateSwapChain();

		Window &window;
		Device &device;
		std::unique_ptr<SwapChain> swapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex;
		int currentFrameIndex;
		bool isFrameStarted;
	};

}