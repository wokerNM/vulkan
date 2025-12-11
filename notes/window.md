Header:
```cpp
    #pragma once 
        prevents multiple inclusions

    #define GLFW_INCLUDE_VULKAN 
        makes glfw include vulkan automatically

    #include "lve_window.hpp"
    ```

CPP:
```cpp
    namespace lve {
    LveWindow::LveWindow(int w, int h, std::string name)
         : width{w},
           height{h},
           windowName{name} {
         initWindow();
    }
      // constructor for window class

    LveWindow::~LveWindow() {
         glfwDestroyWindow(window);
         glfwTerminate();
    }
         // Deconstructor for window class

    void LveWindow::initWindow() {
      glfwInit();
      glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
      glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
      window =
          glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
     }
    } // namespace lve 
```
