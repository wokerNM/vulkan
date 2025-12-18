#pragma once
#include "instance.hpp"
#include "logger.hpp"
#include <GLFW/glfw3.h>
#include <deque>
#include <functional>

/**
 * @brief Vroom vroom.
 *
 */
class Engine {

	public:
		/**
		 * @brief Construct a new Engine object
		 *
		 * @param window main window to render to
		 */
		Engine(GLFWwindow *window);

		/**
		 * @brief Destroy the Engine object
		 *
		 */
		~Engine();

	private:
		/**
		 * @brief static debug logger
		 *
		 */
		Logger *logger{Logger::get_logger()};

		/**
		 * @brief Main window
		 *
		 */
		GLFWwindow *window;

		std::deque<std::function<void()>> deletionQueue;

		vk::Instance instance;
};
