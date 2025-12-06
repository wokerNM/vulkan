#include "lve_pipeline.hpp"
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

namespace lve {

LvePipeline::LvePipeline(LveDevice &device, const std::string &vertFilePath,
                         const std::string &fragFilePath,
                         const PipelineConfigInfo &configInfo)
    : lveDevice{device} {
    createGraphicsPipeline(vertFilePath, fragFilePath, configInfo);
}

LvePipeline::~LvePipeline() {
    vkDestroyShaderModule(lveDevice.device(), vertShaderModule, nullptr);
    vkDestroyShaderModule(lveDevice.device(), fragShaderModule, nullptr);
    vkDestroyPipeline(lveDevice.device(), graphicsPipeline, nullptr);
}

std::vector<char> LvePipeline::readFile(const std::string &filepath) {
    std::ifstream file(filepath, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
}

void LvePipeline::createGraphicsPipeline(const std::string &vertFilePath,
                                         const std::string &fragFilePath,
                                         const PipelineConfigInfo &configInfo) {
    assert(configInfo.pipelineLayout != VK_NULL_HANDLE &&
           "Cannot create graphics pipeline:: no pipelineLayout provided in "
           "configInfo");

    assert(configInfo.renderPass != VK_NULL_HANDLE &&
           "Cannot create graphics pipeline:: no renderPass provided in "
           "configInfo");

    auto vertCode = readFile(vertFilePath);
    auto fragCode = readFile(fragFilePath);

    createShaderModule(vertCode, &vertShaderModule);
    createShaderModule(fragCode, &fragShaderModule);

    VkPipelineShaderStageCreateInfo shaderStages[2];
    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].module = vertShaderModule;
    shaderStages[0].pName = "main";
    shaderStages[0].flags = 0;
    shaderStages[0].pNext = nullptr;
    shaderStages[0].pSpecializationInfo = nullptr;

    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].module = fragShaderModule;
    shaderStages[1].pName = "main";
    shaderStages[1].flags = 0;
    shaderStages[1].pNext = nullptr;
    shaderStages[1].pSpecializationInfo = nullptr;

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    // Describes how you interperet vertex buffer data that is the initial input
    // into the pipeline.
    vertexInputInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr;
    vertexInputInfo.pVertexBindingDescriptions = nullptr;

    VkPipelineViewportStateCreateInfo viewportInfo{};
    viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportInfo.viewportCount = 1;
    viewportInfo.pViewports = &configInfo.viewport;
    viewportInfo.scissorCount = 1;
    viewportInfo.pScissors = &configInfo.scissor;

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    // How many programmable stages the pipeline will use
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
    pipelineInfo.pViewportState = &viewportInfo;
    pipelineInfo.pRasterizationState = &configInfo.rasterizationInfo;
    pipelineInfo.pMultisampleState = &configInfo.multisampleInfo;

    pipelineInfo.pColorBlendState = &configInfo.colorBlendInfo;
    pipelineInfo.pDepthStencilState = &configInfo.depthStencilInfo;
    pipelineInfo.pDynamicState = nullptr;

    pipelineInfo.layout = configInfo.pipelineLayout;
    pipelineInfo.renderPass = configInfo.renderPass;
    pipelineInfo.subpass = configInfo.subpass;

    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;
    // Used for optimization because it can be better for a GPU to make a new
    // pipeline derived from an existing one.

    if (vkCreateGraphicsPipelines(lveDevice.device(), VK_NULL_HANDLE, 1,
                                  &pipelineInfo, nullptr,
                                  &graphicsPipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline");
    }
}

void LvePipeline::createShaderModule(const std::vector<char> &code,
                                     VkShaderModule *shaderModule) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

    if (vkCreateShaderModule(lveDevice.device(), &createInfo, nullptr,
                             shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create shader module");
    }
}

PipelineConfigInfo LvePipeline::defaultPipelineConfigInfo(uint32_t width,
                                                          uint32_t height) {
    PipelineConfigInfo configInfo{};

    configInfo.inputAssemblyInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    configInfo.inputAssemblyInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    // Tell it to use vertecies to make triangle(s)
    configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

    configInfo.viewport.x = 0.0f;
    configInfo.viewport.y = 0.0f;
    configInfo.viewport.width = static_cast<float>(width);
    configInfo.viewport.height = static_cast<float>(height);
    configInfo.viewport.minDepth = 0.0f;
    configInfo.viewport.maxDepth = 1.0f;
    // 0,0 represents the top left pixel because the viewport uses pixel
    // instead of numbers (the triangles use numbers from -1 to 1). This means
    // that if the viewport is cut in half, the whole thing is still rendered,
    // but it is squished into the top half, because the -1 to 1 represents the
    // percentage of the viewport, not constant numbers. the bottom right is
    // (width, height)

    configInfo.scissor.offset = {0, 0};
    configInfo.scissor.extent = {width, height};
    // scissor is like viewport, but instead of squishing a triangle, it cuts
    // it.

    configInfo.rasterizationInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
    // Forces the z component of gl_Position to be between 0 and 1.
    configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
    // Discards all primitives before rasterization, should only use when you
    // only want to use the first few stages of the graphics pipeline.
    configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
    // When drawing triangles, do you want to draw just the corners, the edges,
    // or the triangle filled in.
    configInfo.rasterizationInfo.lineWidth = 1.0f;
    configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
    // Can optionally discard triangles based on their apparent facing, also
    // known as winding order. Imagine 3 nails in a board, and some string being
    // wrapped around them to make a triangle. The order the string is wound
    // around them in is the winding order. More literally, the order you choose
    // to make the program use the vertecies to draw the triangle is the winding
    // order. To determine which face you are looking at, walk around triangle
    // in the order the vertecies are specified.
    // If it was vertex 0, then 1, then 2, then you would be seeing the
    // clockwise face (If that motion would be clockwise. for example, if vertex
    // 0 is on top, 1 is bottom right, 2 is bottom left, you would be facing the
    // clockwise face. Whereas if 1 and 2 were swapped, you would be facing the
    // counter-clockwise face.)
    // Also, this depends on the POV, because if you flip it around, then it has
    // the opposite winding order.
    // Also, you need to specify which direction (clockwise or
    // counter-clockwise) should be considered the front face,
    // then if you want to call the front or back faces.
    // For now none is the default so the first triangle isn't accidentally
    // called.
    configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
    // depth bias can be used to alter depth values by adding a constant value
    // or by a factor of the fragment slope.
    configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;
    configInfo.rasterizationInfo.depthBiasClamp = 0.0f;
    configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;

    configInfo.multisampleInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    //  Multisampling is related to how the rasterizer handles the edges of
    //  geometry. Without it enabled, a fragment is considered either completely
    //  in or completely out of a triangle based on where the pixel's center is.
    //  This can result in visual artifacts known as aliasing, where the edges
    //  of objects looks jagged. MSAA is a method where multiple samples are
    //  taken along the edges of geometry to better approximate how much of the
    //  fragment is contained by the triangle, then based on this shading the
    //  pixel by a variable amount.
    configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
    configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    configInfo.multisampleInfo.minSampleShading = 1.0f;
    configInfo.multisampleInfo.pSampleMask = nullptr;
    configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;
    configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;

    configInfo.colorBlendAttachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
    configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    configInfo.colorBlendInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
    configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
    configInfo.colorBlendInfo.attachmentCount = 1;
    configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
    configInfo.colorBlendInfo.blendConstants[0] = 0.0f;
    configInfo.colorBlendInfo.blendConstants[1] = 0.0f;
    configInfo.colorBlendInfo.blendConstants[2] = 0.0f;
    configInfo.colorBlendInfo.blendConstants[3] = 0.0f;

    // Color blending controls how colors are combined in the frame buffer.
    // If two triangles overlap, then the fragment shader will return multiple
    // colours for some pixels in the frame buffer
    // Here, you can optionally enable color blending and set the values
    // determining how you mix the current output with the colour value already
    // in the frame buffer if any. Mixing occurs based on something like the
    // following: color.rgb = (srcColorBlendFactor * nerColor.rgb)
    // <colorBlendOp> (dstColorBlendFactor * oldColor.rgb ) color.a (alpha) =
    // (srcAlphaBlendFactor * newColor.a) <colorBlendOp> (dstAlphaBlendFactor *
    // oldColor.a) Color = Color & colorWriteMask

    configInfo.depthStencilInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
    configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
    configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
    configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
    configInfo.depthStencilInfo.minDepthBounds = 0.0f;
    configInfo.depthStencilInfo.maxDepthBounds = 1.0f;
    configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
    configInfo.depthStencilInfo.front = {};
    configInfo.depthStencilInfo.back = {};
    // A depth buffer is an additional attachment to the frame buffer that
    // stores a value for every pixel just like how the color attachment the
    // color. It doesn't keep track of layers, just the depth value to whatever
    // fragment is currently on top for each pixel.
    // For example, let's say you have rendered some mountains and a colour
    // buffer. The depth buffer would be the shape of the mountains. If you were
    // to draw a cloud, what would happen is that for each pixel of the cloud,
    // you'd check it's depth against the current values in the depth buffer.
    // You would only update the color and depth buffers for pixels where the
    // cloud is the closest fragment. In this case, if the cloud has a depth of
    // around 0.8, the sky has one of 1.0, and the mountains have one of 0.5,
    // any of the cloud's fragments behind the mountains are simply discarded.
    // The higher the number for depth, the further away it is from the
    // viewport/POV.

    return configInfo;
}

} // namespace lve
