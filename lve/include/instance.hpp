#pragma once
#define VULKAN_HPP_NO_EXCEPTIONS
#include <deque>
#include <functional>
#include <vulkan/vulkan.hpp>

vk::Instance make_instance(const char *applicationName,
						   std::deque<std::function<void()>> &deletionQueue);
