#include "lve_pipeline.hpp"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace lve {
std::vector<char> LvePipeline::readFile(const std::string &filePath) {
    std::ifstream file{filePath, std::ios::ate | std::ios::binary};

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath);
    }

    size_t fileSize = static_cast<std::size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

void LvePipeline::createGraphicsPipeline(const std::string &vertFilePath,
                                         const std::string &fragFilePath) {
    auto vertCode = readFile(vertFilePath);
    auto fragCode = readFile(fragFilePath);

    std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
    std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
}

LvePipeline::LvePipeline(const std::string &vertFilePath,
                         const std::string &fragFilePath) {
    createGraphicsPipeline(vertFilePath, fragFilePath);
}

} // namespace lve
