#include <iostream>
#include "util_init.h"

#define APP_SHORT_NAME "vulkansamples_instance"

int main()
{
//    swap_chain_buffer _swap_chain_buffer = {};
//    layer_properties _layer_properties = {};

    struct sample_info info = {};
    init_global_layer_properties(info);
    /* VULKAN_KEY_START */

    // initialize the VkApplicationInfo structure
    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext = NULL;
    app_info.pApplicationName = APP_SHORT_NAME;
    app_info.applicationVersion = 1;
    app_info.pEngineName = APP_SHORT_NAME;
    app_info.engineVersion = 1;
    app_info.apiVersion = VK_API_VERSION_1_0;

    // initialize the VkInstanceCreateInfo structure
    VkInstanceCreateInfo inst_info = {};
    inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    inst_info.pNext = NULL;
    inst_info.flags = 0;
    inst_info.pApplicationInfo = &app_info;
    inst_info.enabledExtensionCount = 0;
    inst_info.ppEnabledExtensionNames = NULL;
    inst_info.enabledLayerCount = 0;
    inst_info.ppEnabledLayerNames = NULL;

    VkInstance inst;
    VkResult res;

    res = vkCreateInstance(&inst_info, NULL, &inst);
    if (res == VK_ERROR_INCOMPATIBLE_DRIVER) {
        std::cout << "cannot find a compatible Vulkan ICD\n";
        exit(-1);
    } else if (res) {
        std::cout << "unknown error\n";
        exit(-1);
    } else if (res == VK_SUCCESS) {
        std::cout << "Creating vulkan instance was SUCCESSFUL!\n" << std::endl;
        std::cout << "GPU deviceName is: " << info.gpu_props.deviceName << std::endl;
    }

    vkDestroyInstance(inst, NULL);

    /* VULKAN_KEY_END */

    std::cout << "VULKAN API EXAMPLE ENDS !\n" << std::endl;
    return 0;
}
