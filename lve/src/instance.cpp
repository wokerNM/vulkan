#include "instance.hpp"
#include "logger.hpp"
#include "vulkan/vulkan.hpp"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <vulkan/vulkan_core.h>

vk::Instance make_instance(const char *applicationName,
						   std::deque<std::function<void()>> &deletionQueue) {
	Logger *logger = Logger::get_logger();

	logger->print("Making an instance");

	uint32_t version = vk::enumerateInstanceVersion().value;

	logger->report_version_number(version);

	version &= ~(0xFFFU);

	vk::ApplicationInfo appInfo =
		vk::ApplicationInfo(applicationName, version, NULL, version, version);

	uint32_t glfwExtenstionCount = 0;

	const char **glfwExtenstions =
		glfwGetRequiredInstanceExtensions(&glfwExtenstionCount);

	logger->print("Extensions to be requested:");
	logger->print_list(glfwExtenstions, glfwExtenstionCount);

	vk::InstanceCreateInfo createInfo =
		vk::InstanceCreateInfo(vk::InstanceCreateFlags(), &appInfo, 0, nullptr,
							   glfwExtenstionCount, glfwExtenstions);

	vk::ResultValue<vk::Instance> instanceAttempt =
		vk::createInstance(createInfo);
	if (instanceAttempt.result != vk::Result::eSuccess) {
		logger->print("Failed to create instance!");
		return nullptr;
	}

	vk::Instance instance = instanceAttempt.value;
	VkInstance handle = instance;

	deletionQueue.push_back([logger, handle]() {
		vkDestroyInstance(handle, nullptr);
		logger->print("Deleted Instance!");
	});

	return instance;
}
