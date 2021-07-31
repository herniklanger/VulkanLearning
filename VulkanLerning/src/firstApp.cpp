#include "firstApp.h"


// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


#include <array>
#include <cassert>
#include <stdexcept>

namespace VulkanTest
{
	struct SimpelPushConstantData {
		glm::mat2 transform{ 1.f };
		glm::vec2 offeset;
		alignas(16) glm::vec3 color;
	};

	FirstApp::FirstApp() {
		loadGameObjects();
		createPipelineLayout();
		recreateSwapChain();
		createCommandBuffers();
	}
	FirstApp::~FirstApp() {
		vkDestroyPipelineLayout(device.device(), pipelineLayour, nullptr);
	}

	void FirstApp::run() 
	{
		while(!Window.shouldClose())
		{
			glfwPollEvents();
			drawFrame();

		}
		vkDeviceWaitIdle(device.device());
	}

	void FirstApp::loadGameObjects()
	{
		std::vector<Model::Vertex> vertices
		{ 
			{{0.0f, -0.5f},{1.0f,0.0f,0.0f}},
			{{0.5f, 0.5f},{0.0f,1.0f,0.0f}},
			{{-0.5f, 0.5f},{0.0f,0.0f,1.0f}}
		};
		auto model = std::make_shared<Model>(device, vertices);

		auto triangle = GameObject::CreateGameObject();
		triangle.model = model;
		triangle.color = { .1f,.8f, .1f };
		triangle.Transform2d.translation.x = .2f;
		triangle.Transform2d.scale = { 2.f, .5f };
		triangle.Transform2d.roatation = .25f * glm::two_pi<float>();


		gameObjects.push_back(std::move(triangle));
	}

	void FirstApp::createPipelineLayout()
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimpelPushConstantData);


		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

		if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayour) != VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout!");
		}
	}

	void FirstApp::createPipeline()
	{
		assert(swapChain != nullptr && "Cannot create pipline befre swap chain");
		assert(pipelineLayour != nullptr && "Cannot create pipline befre pipline layout");

		PipelineConfigInfo pipelineConfig{};
		Pipeline::defaultPiplineConfigInfo(
			pipelineConfig);
		pipelineConfig.renderPass = swapChain->getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayour;
		pipeline = std::make_unique<Pipeline>(
			device,
			"src/Shaders/simpleShader.vert.spv",
			"src/Shaders/simpleShader.frag.spv",
			pipelineConfig);
	}

	void FirstApp::recreateSwapChain()
	{
		auto extent = Window.getExtent();
		while (extent.width == 0 || extent.height == 0)
		{
			extent = Window.getExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(device.device());

		if (swapChain == nullptr) 
		{
			swapChain = std::make_unique<SwapChain>(device, extent);
		}
		else {
			swapChain = std::make_unique<SwapChain>(device, extent,std::move(swapChain));
			if (swapChain->imageCount() != commandBuffers.size())
			{
				freeCommandBuffer();
				createCommandBuffers();
			}
		}
		createPipeline();

	}

	void FirstApp::createCommandBuffers()
	{
		commandBuffers.resize(swapChain->imageCount());
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = device.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) 
		{
			throw std::runtime_error("failed to allocate command buffers");
		}
		
	}

	void FirstApp::freeCommandBuffer()
	{
		vkFreeCommandBuffers(device.device(), device.getCommandPool(),
			static_cast<float>(commandBuffers.size()), commandBuffers.data());
		commandBuffers.clear();
	}

	void FirstApp::recordCommandBuffre(int imageIndex) 
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {

			throw std::runtime_error("failed to begin recoard command buffer");
		}
		VkRenderPassBeginInfo  renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = swapChain->getRenderPass();
		renderPassInfo.framebuffer = swapChain->getFrameBuffer(imageIndex);

		renderPassInfo.renderArea.offset = { 0,0 };
		renderPassInfo.renderArea.extent = swapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(swapChain->getSwapChainExtent().width);
		viewport.height= static_cast<float>(swapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{ {0,0}, swapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffers[imageIndex],0,1, &viewport);
		vkCmdSetScissor(commandBuffers[imageIndex],0,1, &scissor);

		renderGameObjects(commandBuffers[imageIndex]);


		vkCmdEndRenderPass(commandBuffers[imageIndex]);
		if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to recoard command buffer");
		}
		
	}

	void FirstApp::renderGameObjects(VkCommandBuffer commandBuffer)
	{
		pipeline->bind(commandBuffer);

		for (auto& obj : gameObjects) {
			obj.Transform2d.roatation = glm::mod(obj.Transform2d.roatation + 0.1f, glm::two_pi<float>());

			SimpelPushConstantData push{};
			push.offeset = obj.Transform2d.translation;
			push.color = obj.color;
			push.transform = obj.Transform2d.mat2();
			vkCmdPushConstants(commandBuffer, pipelineLayour, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimpelPushConstantData), &push);
			obj.model->bind(commandBuffer);
			obj.model->draw(commandBuffer);
		}
	}

	void FirstApp::drawFrame()
	{
		uint32_t imageIndex;
		auto result = swapChain->acquireNextImage(&imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return;
		}
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("faild to acquire swap chain image!");
		}

		recordCommandBuffre(imageIndex);
		result = swapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || Window.wasWindowReized()) 
		{
			Window.restedWindowResizedFlag();
			recreateSwapChain();
			return;
		}
		if (result != VK_SUCCESS) {
			throw std::runtime_error("faild to present swap chain image!");
		}
	}

}