#include "firstApp.h"

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
	FirstApp::FirstApp() {
		loadGameObjects();
	}
	FirstApp::~FirstApp() {

	}

	void FirstApp::run() 
	{
		SimpleRenderSystem simpleRenderSystem{device, renderer.getSwapChainRenderPass()};
		while(!window.shouldClose())
		{
			glfwPollEvents();
			if (auto commandBuffer = renderer.BeginFrame()) {
				renderer.BeginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.RenderGameObjects(commandBuffer, gameObjects);
				renderer.EndSwapChainRenderPass(commandBuffer);
				renderer.EndFrame();
			}

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
}