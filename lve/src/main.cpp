#include "app.hpp"
#include "glfw_backend.hpp"
#include "logger.hpp"
#include "renderer.hpp"
#include <atomic>
#include <thread>

void spawn_render_thread(GLFWwindow *window, std::atomic<bool> *done) {
	Engine *engine = new Engine(window);

	while (!*done) {
		// Do work repeatedly!
	}

	delete engine;
}

int main() {
	Logger *logger = Logger::get_logger();
	logger->set_mode(true);

	int width = 800, height = 600;
	GLFWwindow *window = build_window(width, height, "ID Tech 12");

	std::atomic<bool> done = false;
	std::thread render_thread(spawn_render_thread, window, &done);
	App *app = new App(window);

	done = true;
	render_thread.join();
	glfwTerminate();
	return 0;
}
