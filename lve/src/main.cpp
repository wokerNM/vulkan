#include "first_app.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <unistd.h>

int main() {
    lve::FirstApp app{};

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        std::cout << "Current working directory: " << cwd << std::endl;
    } else {
        perror("getcwd() error");
    }

    try {
        app.run();
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            std::cout << "Current working directory after running: " << cwd
                      << std::endl;
        } else {
            perror("getcwd() error");
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
