#include <iostream>
#include "util_init.h"

#include <cassert>
#include <cstdlib>

#define APP_SHORT_NAME "vulkansamples_instance"

int main()
{
    //01. Init

    /* VULKAN_KEY_START */
    struct sample_info info = {};
    init_global_layer_properties(info);

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
    inst_info.enabledExtensionCount = info.instance_extension_names.size();
    inst_info.ppEnabledExtensionNames = info.instance_extension_names.data();
    inst_info.enabledLayerCount = info.instance_layer_names.size();
    inst_info.ppEnabledLayerNames = info.instance_layer_names.size() ? info.instance_layer_names.data() : NULL;

    //    VkInstance vkInstance;
    VkResult res;

    res = vkCreateInstance(&inst_info, NULL, &info.inst);
    if (res == VK_ERROR_INCOMPATIBLE_DRIVER) {
        std::cout << "cannot find a compatible Vulkan ICD\n";
        exit(-1);
    } else if (res) {
        std::cout << "unknown error\n";
        exit(-1);
    } else if (res == VK_SUCCESS) {
        std::printf("Creating vulkan instance: SUCCESS!\n");
        //        info.inst = vkInstance;
    }

    //02. enumerate physical devices

    uint32_t gpu_count = 10;
    res = vkEnumeratePhysicalDevices(info.inst, &gpu_count, NULL); //after this operation gpu_count will be actual count of vulkan-gpu
    info.gpus.resize(gpu_count);
    res = vkEnumeratePhysicalDevices(info.inst, &gpu_count, info.gpus.data());
    if (res == VK_SUCCESS) {
        std::printf("Enumerating physical devices: SUCCESS!\n");
        std::printf("Amount of GPUs with VULKAN API support is:\t%ld\n", info.gpus.size());
    }

    vkGetPhysicalDeviceMemoryProperties(info.gpus[0], &info.memory_properties);
    vkGetPhysicalDeviceProperties(info.gpus[0], &info.gpu_props);
    printf("Device model is:\t%s\n", info.gpu_props.deviceName);

    /* query device extensions for enabled layers */
    for (auto& layer_props : info.instance_layer_properties) {
        init_device_extension_properties(info, layer_props);
    }

    //03. create and destroy a Vulkan physical device
    VkDeviceQueueCreateInfo queue_info_VK_QUEUE_GRAPHICS_BIT = {};

    vkGetPhysicalDeviceQueueFamilyProperties(info.gpus[0], &info.queue_family_count, NULL);
    assert(info.queue_family_count >= 1);
    info.queue_props.resize(info.queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(info.gpus[0], &info.queue_family_count, info.queue_props.data());
    assert(info.queue_family_count >= 1);

    //    bool found_VK_QUEUE_GRAPHICS_BIT = false;
    //    for (unsigned int i = 0; i < info.queue_family_count; i++) {
    //        if (info.queue_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
    //            queue_info.queueFamilyIndex = i;
    //            found_VK_QUEUE_GRAPHICS_BIT = true;
    //            break;
    //        }
    //    }
    bool found_VK_QUEUE_GRAPHICS_BIT = findQueueByName(info, queue_info_VK_QUEUE_GRAPHICS_BIT,VK_QUEUE_GRAPHICS_BIT);
    assert(found_VK_QUEUE_GRAPHICS_BIT);
    assert(info.queue_family_count >= 1);

    //only example to find another queue family with given flag
    VkDeviceQueueCreateInfo queue_info_VK_QUEUE_COMPUTE_BIT = {};
    bool found_VK_QUEUE_COMPUTE_BIT = findQueueByName(info, queue_info_VK_QUEUE_COMPUTE_BIT,VK_QUEUE_COMPUTE_BIT);
    assert(found_VK_QUEUE_COMPUTE_BIT);

    float queue_priorities[1] = {0.0};
    queue_info_VK_QUEUE_GRAPHICS_BIT.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_info_VK_QUEUE_GRAPHICS_BIT.pNext = NULL;
    queue_info_VK_QUEUE_GRAPHICS_BIT.queueCount = 1;
    queue_info_VK_QUEUE_GRAPHICS_BIT.pQueuePriorities = queue_priorities;

    VkDeviceCreateInfo device_info = {};
    device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_info.pNext = NULL;
    device_info.queueCreateInfoCount = 1;
    device_info.pQueueCreateInfos = &queue_info_VK_QUEUE_GRAPHICS_BIT;
    device_info.enabledExtensionCount = 0;
    device_info.ppEnabledExtensionNames = NULL;
    device_info.enabledLayerCount = 0;
    device_info.ppEnabledLayerNames = NULL;
    device_info.pEnabledFeatures = NULL;

    res = vkCreateDevice(info.gpus[0], &device_info, NULL, &info.device);
    if (res == VK_SUCCESS) {
        std::printf("Create Vulkan logical device: SUCCESS!\n");
    }

    // 04. Create a Command Buffer

    /* Create a command pool to allocate our command buffer from */
    VkCommandPoolCreateInfo cmd_pool_info = {};
    cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmd_pool_info.pNext = NULL;
    cmd_pool_info.queueFamilyIndex = info.graphics_queue_family_index;
    cmd_pool_info.flags = 0;

    res = vkCreateCommandPool(info.device, &cmd_pool_info, NULL, &info.cmd_pool);
    assert(res == VK_SUCCESS);

    /* Create the command buffer from the command pool */
    VkCommandBufferAllocateInfo cmd = {};
    cmd.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmd.pNext = NULL;
    cmd.commandPool = info.cmd_pool;
    cmd.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmd.commandBufferCount = 1;

    res = vkAllocateCommandBuffers(info.device, &cmd, &info.cmd);
    assert(res == VK_SUCCESS);

    /* VULKAN_KEY_END */
    printf("Destroying device ...\n");
    VkCommandBuffer cmd_bufs[1] = {info.cmd};
    vkFreeCommandBuffers(info.device, info.cmd_pool, 1, cmd_bufs);
    vkDestroyCommandPool(info.device, info.cmd_pool, NULL);
    vkDeviceWaitIdle(info.device);
    vkDestroyDevice(info.device, NULL);
    vkDestroyInstance(info.inst, NULL);
    printf("VULKAN API EXAMPLE ENDS !\n");
    return 0;
}
