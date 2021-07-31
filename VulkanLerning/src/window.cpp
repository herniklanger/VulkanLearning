#include "window.h"
#include <stdexcept>

namespace VulkanTest {
	Window::Window(int width, int height, std::string name)
		:width{ width }, height{ height }, windowName{ name}
	{
		initWindow();
	}
	Window::~Window() 
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}
	void Window::frameburfferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto resizedWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		resizedWindow->framebufferReized = true;
		resizedWindow->width = width;
		resizedWindow->height = height;

	}
	void Window::initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, frameburfferResizeCallback);
	}

	bool Window::shouldClose()
	{
		return glfwWindowShouldClose(window);
	}
	void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create window serface");
		}
	}
}