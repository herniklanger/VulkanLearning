#include "SimpleRenderSystem.h"


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

	SimpleRenderSystem::SimpleRenderSystem(Device& device, VkRenderPass renderPass) : device{ device }
	{
		createPipelineLayout();
		createPipeline(renderPass);
	}
	SimpleRenderSystem::~SimpleRenderSystem() {
		vkDestroyPipelineLayout(device.device(), pipelineLayour, nullptr);
	}

	void SimpleRenderSystem::createPipelineLayout()
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

	void SimpleRenderSystem::createPipeline(VkRenderPass renderPass)
	{
		assert(pipelineLayour != nullptr && "Cannot create pipline befre pipline layout");

		PipelineConfigInfo pipelineConfig{};
		Pipeline::defaultPiplineConfigInfo(
			pipelineConfig);
		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayour;
		pipeline = std::make_unique<Pipeline>(
			device,
			"src/Shaders/simpleShader.vert.spv",
			"src/Shaders/simpleShader.frag.spv",
			pipelineConfig);
	}


	void SimpleRenderSystem::RenderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>&gameObjects)
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
}