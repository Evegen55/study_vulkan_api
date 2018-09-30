#ifndef UTIL_INIT_H
#define UTIL_INIT_H

#include <vulkan/vk_layer.h>
#include <util.h>

VkResult init_global_layer_properties(sample_info &info);
VkResult init_device_extension_properties(struct sample_info &info, layer_properties &layer_props);
bool findQueueByName(struct sample_info &info, VkDeviceQueueCreateInfo &queue_info, enum VkQueueFlagBits queue_name);

#endif // UTIL_INIT_H
