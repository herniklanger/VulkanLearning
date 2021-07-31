#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace VulkanTest {

	class Window{

	public:
		Window(int width, int height, std::string name);
		~Window();

		Window(const Window&) = delete;
		Window &operator=(const Window&) = delete;

		bool shouldClose();

		VkExtent2D getExtent() { return { static_cast<uint32_t>(width),static_cast<uint32_t>(height) }; }
		bool wasWindowReized() { return framebufferReized; }
		void restedWindowResizedFlag() { framebufferReized = false; };

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:

		static void frameburfferResizeCallback(GLFWwindow* window, int width, int height);
		void initWindow();

		int width;
		int height;
		bool framebufferReized = false;

		std::string windowName;
		GLFWwindow* window;
	};
}