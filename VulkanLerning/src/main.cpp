#include "firstApp.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() 
{
	VulkanTest::FirstApp app{};

	try {
		app.run();
	}
	catch(const std::exception &e)
	{
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}