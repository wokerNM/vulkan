Window is created, no vulkan for now just GLFW, it's used for key presses, drawing, and more so the app needs to have access to it.

std::thread render_thread is a thread for rendering, it's done as a seperate thread for multithreading.
