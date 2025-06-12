#include "graphics_api_vk.h"

#define VK_NO_PROTOTYPES
#include <volk.h>
#include "GLFW/glfw3.h"

#include "spritz/window.h"
#include "window_internal.h"

#include <stdint.h>
// #include <vulkan/vulkan_core.h>

#include <stdlib.h>
#include <string.h>

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef clamp
#define clamp(a, lo, hi) ((a) < (lo) ? lo : ((a) > (hi) ? (hi) : (a)))
#endif

typedef struct {
    bool hasGraphicsQueue;
    bool hasPresentQueue;
    bool hasTransferQueue;

    uint32_t graphicsQueueFamilyIndex;
    uint32_t transferQueueFamilyIndex;
    uint32_t presentQueueFamilyIndex;
} SpritzVKQueueProperties_t;

typedef struct {
    VkQueue graphicsQueue;
    VkQueue transferQueue;
    VkQueue presentQueue;
} SpritzVKQueues_t;

typedef struct {
    VkPhysicalDevice physicalDevice;
    VkDevice device;

    SpritzVKQueueProperties_t queueProperties;
    SpritzVKQueues_t queues;
} SpritzVKDeviceData_t;

typedef struct {
    VkSurfaceKHR surface;

    VkSwapchainKHR swapchain;

    VkImage* images;
    VkImageView* imageViews;
    uint32_t imageCount;

    VkFormat imageFormat;

    SpritzWindow_t window;
} SpritzVKPresentData_t;

typedef struct {
    VkInstance instance;

    bool validationEnabled;
    VkDebugUtilsMessengerEXT debugMessenger;

    SpritzVKDeviceData_t deviceData;

    SpritzVKPresentData_t presentData;
} SpritzVKInternal_t;

#define UDATA_INIT SpritzVKInternal_t* iData = uData
#define RET_ASRT(x, fmt, ...)                                                  \
    if (!(x)) {                                                                \
        printf(fmt, __VA_ARGS__);                                              \
        return false;                                                          \
    }

#define MAX_EXTENSIONS 16
typedef struct {
    char* extensions[MAX_EXTENSIONS];
    uint32_t count;
} ExtensionsInfo_t;

static ExtensionsInfo_t
svkGetRequiredInstanceExtensions(const SpritzVKInternal_t* iData) {
    ExtensionsInfo_t info = {};
    uint32_t currentExtensionIndex = 0;

#define SET_EXTENSION(x)                                                       \
    if (currentExtensionIndex >= MAX_EXTENSIONS) {                             \
        printf("spritz: Too many Vulkan extensions!\n");                       \
    } else {                                                                   \
        info.extensions[currentExtensionIndex++] = x;                          \
    }

    uint32_t nGLFWExtensions;
    const char** glfwExtensions =
        glfwGetRequiredInstanceExtensions(&nGLFWExtensions);

    for (uint32_t i = 0; i < nGLFWExtensions; i++) {
        SET_EXTENSION((char*)glfwExtensions[i]);
    }

#ifdef __APPLE__
    // macOS uses MoltenVK which needs the portability extensions
    SET_EXTENSION(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME)
#endif

    if (iData->validationEnabled) {
        SET_EXTENSION(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

#undef SET_EXTENSION

    info.count = currentExtensionIndex;
    return info;
}

static ExtensionsInfo_t
svkGetRequiredDeviceExtensions(const SpritzVKInternal_t* iData) {
    ExtensionsInfo_t info = {};
    uint32_t currentExtensionIndex = 0;

#define SET_EXTENSION(x)                                                       \
    if (currentExtensionIndex >= MAX_EXTENSIONS) {                             \
        printf("spritz: Too many Vulkan extensions!\n");                       \
    } else {                                                                   \
        info.extensions[currentExtensionIndex++] = x;                          \
    }

#ifdef __APPLE__
    // macOS uses MoltenVK which needs the portability extensions
    SET_EXTENSION("VK_KHR_portability_subset");
#endif

    SET_EXTENSION(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

#undef SET_EXTENSION

    info.count = currentExtensionIndex;
    return info;
}
#undef MAX_EXTENSIONS

static VKAPI_ATTR VkBool32 VKAPI_CALL
svkDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                 VkDebugUtilsMessageTypeFlagsEXT messageType,
                 const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                 void* pUserData) {

    printf("spritz vulkan validation: %s\n", pCallbackData->pMessage);

    return VK_FALSE;
}

static VkDebugUtilsMessengerCreateInfoEXT
svkGetDebugUtilsMessengerCreateInfo() {
    VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT; //|
    /*VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;*/
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    createInfo.pfnUserCallback = svkDebugCallback;

    return createInfo;
}

static bool
svkSetupDebugMessenger(VkDebugUtilsMessengerEXT* debugUtilsMessenger,
                       const SpritzVKInternal_t* iData) {
    VkDebugUtilsMessengerCreateInfoEXT createInfo =
        svkGetDebugUtilsMessengerCreateInfo();

    PFN_vkCreateDebugUtilsMessengerEXT createFN =
        (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            iData->instance, "vkCreateDebugUtilsMessengerEXT");

    if (createFN != NULL) {
        if (createFN(iData->instance, &createInfo, NULL, debugUtilsMessenger) !=
            VK_SUCCESS) {
            printf("spritz: failed to create debug messenger!\n");
            return false;
        } else {
            return true;
            printf("success here\n");
        }
    } else {
        printf("spritz: failed to load proc address of "
               "vkCreateDebugUtilsMessengerEXT\n");
        return false;
    }
}

static bool svkDestroyDebugUtilsMessenger(const SpritzVKInternal_t* iData) {
    PFN_vkDestroyDebugUtilsMessengerEXT destroyFN =
        (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            iData->instance, "vkDestroyDebugUtilsMessengerEXT");

    if (destroyFN == NULL) {
        printf("spritz: failed to load vkDestroyDebugUtilsMessengerEXT\n");
        return false;
    }

    destroyFN(iData->instance, iData->debugMessenger, NULL);

    return true;
}

#define SVK_N_VALIDATION_LAYERS 1
static const char* svkValidationLayers[SVK_N_VALIDATION_LAYERS] = {
    "VK_LAYER_KHRONOS_validation"};

static bool svkCheckValidationLayerSupport(VkInstance instance) {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, NULL);

    VkLayerProperties* layerProperties =
        alloca(sizeof(VkLayerProperties) * layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, layerProperties);

    for (int i = 0; i < SVK_N_VALIDATION_LAYERS; i++) {
        bool hasLayer = false;
        for (int j = 0; j < layerCount; j++) {
            VkLayerProperties* properties = layerProperties + j;
            if (strcmp(properties->layerName, svkValidationLayers[i]) == 0) {
                hasLayer = true;
                break;
            }
        }

        if (!hasLayer)
            return false;
    }

    return true;
}

static bool svkLoadInstance(VkInstance* instance,
                            const SpritzVKInternal_t* iData) {
    VkApplicationInfo applicationCreateInfo = {};
    applicationCreateInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationCreateInfo.apiVersion = VK_API_VERSION_1_1;
    applicationCreateInfo.pEngineName = "Spritz";
    applicationCreateInfo.pApplicationName = "Spritz Vulkan App";
    applicationCreateInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationCreateInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);

    uint32_t nGLFWExtensions;
    const char** glfwExtensions =
        glfwGetRequiredInstanceExtensions(&nGLFWExtensions);

    ExtensionsInfo_t properties = svkGetRequiredInstanceExtensions(iData);

    VkInstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &applicationCreateInfo;
    instanceCreateInfo.enabledExtensionCount = properties.count;
    instanceCreateInfo.ppEnabledExtensionNames =
        (const char**)properties.extensions;

#ifdef __APPLE__
    // because we use MoltenVK on MacOS
    instanceCreateInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif
    if (iData->validationEnabled) {
        instanceCreateInfo.enabledLayerCount = SVK_N_VALIDATION_LAYERS;
        instanceCreateInfo.ppEnabledLayerNames = svkValidationLayers;

        VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo =
            svkGetDebugUtilsMessengerCreateInfo();
        instanceCreateInfo.pNext = &debugUtilsMessengerCreateInfo;
    } else {
        instanceCreateInfo.enabledLayerCount = 0;
    }

    if (vkCreateInstance(&instanceCreateInfo, NULL, instance) != VK_SUCCESS) {
        printf("spritz: failed to load vulkan instance!\n");
        return false;
    }

    return true;
}

static SpritzVKQueueProperties_t
svkGetPhysicalDeviceQueues(VkPhysicalDevice physicalDevice,
                           const SpritzVKInternal_t* iData) {
    SpritzVKQueueProperties_t queueProperties = {};

    uint32_t nPropertiesCount;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &nPropertiesCount,
                                             NULL);

    VkQueueFamilyProperties queueFamilyProperties[nPropertiesCount];
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &nPropertiesCount,
                                             queueFamilyProperties);

    for (uint32_t i = 0; i < nPropertiesCount; i++) {
        if (!queueProperties.hasGraphicsQueue &&
            queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            queueProperties.graphicsQueueFamilyIndex = i;
            queueProperties.hasGraphicsQueue = true;
        }

        if (!queueProperties.hasTransferQueue &&
            queueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
            queueProperties.transferQueueFamilyIndex = i;
            queueProperties.hasTransferQueue = true;
        }

        if (glfwGetPhysicalDevicePresentationSupport(iData->instance,
                                                     physicalDevice, i)) {
            queueProperties.presentQueueFamilyIndex = i;
            queueProperties.hasPresentQueue = true;
        }
    }

    return queueProperties;
}

static bool
svkConfirmPhysicalDeviceExtensionSupport(VkPhysicalDevice device,
                                         const SpritzVKInternal_t* iData) {
    ExtensionsInfo_t requiredExtensions = svkGetRequiredDeviceExtensions(iData);

    uint32_t supportedDeviceExtensionCount;
    vkEnumerateDeviceExtensionProperties(device, NULL,
                                         &supportedDeviceExtensionCount, NULL);

    VkExtensionProperties supportedExtensions[supportedDeviceExtensionCount];
    vkEnumerateDeviceExtensionProperties(
        device, NULL, &supportedDeviceExtensionCount, supportedExtensions);

    for (int i = 0; i < requiredExtensions.count; i++) {
        bool supportsExtension = false;
        const char* extensionName = requiredExtensions.extensions[i];
        for (int j = 0; j < supportedDeviceExtensionCount; j++) {
            if (strcmp(extensionName, supportedExtensions[j].extensionName) ==
                0) {
                supportsExtension = true;
                break;
            }
        }

        if (!supportsExtension) {
            return false;
        }
    }

    return true;
}

static bool svkLoadDeviceAndQueues(SpritzVKDeviceData_t* deviceData,
                                   const SpritzVKInternal_t* iData) {
    uint32_t nPhysicalDevices;
    vkEnumeratePhysicalDevices(iData->instance, &nPhysicalDevices, NULL);

    VkPhysicalDevice* physicalDevices =
        alloca(sizeof(VkPhysicalDevice) * nPhysicalDevices);
    vkEnumeratePhysicalDevices(iData->instance, &nPhysicalDevices,
                               physicalDevices);

    VkPhysicalDevice pickedPhysicalDevice = VK_NULL_HANDLE;
    VkPhysicalDeviceType pickedPhysicalDeviceType;

    for (int i = 0; i < nPhysicalDevices; i++) {
        SpritzVKQueueProperties_t queueProperties =
            svkGetPhysicalDeviceQueues(physicalDevices[i], iData);

        if (!queueProperties.hasGraphicsQueue ||
            !queueProperties.hasTransferQueue ||
            !queueProperties.hasPresentQueue) {
            continue;
        }

        if (!svkConfirmPhysicalDeviceExtensionSupport(physicalDevices[i],
                                                      iData)) {
            continue;
        }

        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physicalDevices[i], &properties);

        if (pickedPhysicalDevice == VK_NULL_HANDLE) {
            pickedPhysicalDevice = physicalDevices[i];
            properties.deviceType = properties.deviceType;
        }

        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            pickedPhysicalDevice = physicalDevices[i];
            pickedPhysicalDeviceType = properties.deviceType;
        }
    }

    if (pickedPhysicalDevice == VK_NULL_HANDLE) {
        printf("spritz: failed to find suitable device!\n");
        return false;
    }

    deviceData->physicalDevice = pickedPhysicalDevice;
    deviceData->queueProperties =
        svkGetPhysicalDeviceQueues(pickedPhysicalDevice, iData);

    float maxQueuePriority = 1.0f;

    VkDeviceQueueCreateInfo graphicsQueueCreateInfo = {};
    graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    graphicsQueueCreateInfo.queueFamilyIndex =
        deviceData->queueProperties.graphicsQueueFamilyIndex;
    graphicsQueueCreateInfo.queueCount = 1;
    graphicsQueueCreateInfo.pQueuePriorities = &maxQueuePriority;

    VkDeviceQueueCreateInfo presentQueueCreateInfo = {};
    presentQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    presentQueueCreateInfo.queueFamilyIndex =
        deviceData->queueProperties.presentQueueFamilyIndex;
    presentQueueCreateInfo.queueCount = 1;
    presentQueueCreateInfo.pQueuePriorities = &maxQueuePriority;

    VkDeviceQueueCreateInfo queueCreateInfos[2] = {graphicsQueueCreateInfo,
                                                   presentQueueCreateInfo};

    uint32_t queueCount =
        deviceData->queueProperties.graphicsQueueFamilyIndex ==
                deviceData->queueProperties.presentQueueFamilyIndex
            ? 1
            : 2;

    VkPhysicalDeviceFeatures deviceFeatures = {};

    ExtensionsInfo_t deviceExtensionInfo =
        svkGetRequiredDeviceExtensions(iData);

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = queueCount;
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos;
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    deviceCreateInfo.ppEnabledLayerNames = svkValidationLayers;
    deviceCreateInfo.enabledLayerCount = SVK_N_VALIDATION_LAYERS;
    deviceCreateInfo.enabledExtensionCount = deviceExtensionInfo.count;
    deviceCreateInfo.ppEnabledExtensionNames =
        (const char**)deviceExtensionInfo.extensions;

    if (vkCreateDevice(iData->deviceData.physicalDevice, &deviceCreateInfo,
                       NULL, &deviceData->device) != VK_SUCCESS) {
        printf("spritz: failed to create Vulkan device!\n");
        return false;
    }

    volkLoadDevice(iData->deviceData.device);

    vkGetDeviceQueue(iData->deviceData.device,
                     iData->deviceData.queueProperties.graphicsQueueFamilyIndex,
                     0, &deviceData->queues.graphicsQueue);

    vkGetDeviceQueue(iData->deviceData.device,
                     iData->deviceData.queueProperties.presentQueueFamilyIndex,
                     0, &deviceData->queues.presentQueue);

    return true;
}

static void svkDestroyDevice(SpritzVKDeviceData_t* deviceData) {
    vkDestroyDevice(deviceData->device, NULL);
}

static bool volkInitialized = false;
bool spritzGraphicsAPIVKPreWindowSystemInit(void* uData) {
    UDATA_INIT;
    if (!volkInitialized) {
        volkInitialize();
        volkInitialized = true;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

#ifdef NDEBUG
    iData->validationEnabled = false;
#else
    iData->validationEnabled = true;
#endif

    RET_ASRT(svkLoadInstance(&iData->instance, iData), "%s",
             "spritz: failed to create vulkan instance!\n")
    volkLoadInstance(iData->instance);
    if (iData->validationEnabled) {
        RET_ASRT(svkCheckValidationLayerSupport(iData->instance), "%s",
                 "spritz: validation layers requested, but the required don't "
                 "exist!\n")
        RET_ASRT(svkSetupDebugMessenger(&iData->debugMessenger, iData), "%s",
                 "spritz: failed to create debug utils messenger!\n");
    }

    return true;
}

static int svkChooseSwapchainImageCount(
    const VkSurfaceCapabilitiesKHR* surfaceCapabilities) {
    uint32_t imageCount = surfaceCapabilities->minImageCount + 1;

    if (surfaceCapabilities->maxImageCount != 0)
        imageCount = min(imageCount, surfaceCapabilities->maxImageCount);

    return imageCount;
}

static VkSurfaceFormatKHR
svkChooseSwapchainSurfaceFormat(const SpritzVKInternal_t* iData) {
    uint32_t nFormats;
    vkGetPhysicalDeviceSurfaceFormatsKHR(iData->deviceData.physicalDevice,
                                         iData->presentData.surface, &nFormats,
                                         NULL);

    VkSurfaceFormatKHR surfaceFormats[nFormats];
    vkGetPhysicalDeviceSurfaceFormatsKHR(iData->deviceData.physicalDevice,
                                         iData->presentData.surface, &nFormats,
                                         surfaceFormats);

    for (uint32_t i = 0; i < nFormats; i++) {
        if (surfaceFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB &&
            surfaceFormats[i].colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR) {
            printf("spritz: Found preferred Vulkan surface format!\n");
            return surfaceFormats[i];
        }
    }

    return surfaceFormats[0];
}

static VkPresentModeKHR
svkChooseSwapchainPresentMode(const SpritzVKInternal_t* iData) {
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(iData->deviceData.physicalDevice,
                                              iData->presentData.surface,
                                              &presentModeCount, NULL);

    VkPresentModeKHR presentModes[presentModeCount];
    vkGetPhysicalDeviceSurfacePresentModesKHR(iData->deviceData.physicalDevice,
                                              iData->presentData.surface,
                                              &presentModeCount, presentModes);

    for (uint32_t i = 0; i < presentModeCount; i++) {
        if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            printf("Found preferred present mode!\n");
            return VK_PRESENT_MODE_MAILBOX_KHR;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

static VkExtent2D
svkChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR* surfaceCapabilities,
                         const SpritzVKInternal_t* iData) {
    if (surfaceCapabilities->currentExtent.width != UINT32_MAX) {
        return surfaceCapabilities->currentExtent;
    }

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(iData->presentData.window->window, &fbWidth,
                           &fbHeight);

    VkExtent2D windowExtent = {fbWidth, fbHeight};

    windowExtent.width =
        clamp(windowExtent.width, surfaceCapabilities->minImageExtent.width,
              surfaceCapabilities->maxImageExtent.width);
    windowExtent.height =
        clamp(windowExtent.height, surfaceCapabilities->minImageExtent.height,
              surfaceCapabilities->maxImageExtent.height);

    return windowExtent;
}

static bool svkInitPresentation(SpritzVKPresentData_t* presentData,
                                const SpritzVKInternal_t* iData) {

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(iData->deviceData.physicalDevice,
                                              iData->presentData.surface,
                                              &surfaceCapabilities);

    VkSurfaceFormatKHR surfaceFormat = svkChooseSwapchainSurfaceFormat(iData);

    bool samePresentAsGraphicsQueue =
        iData->deviceData.queueProperties.graphicsQueueFamilyIndex ==
        iData->deviceData.queueProperties.presentQueueFamilyIndex;

    uint32_t queueFamilyIndexCount = samePresentAsGraphicsQueue ? 1 : 2;
    const uint32_t queueFamilyIndices[] = {
        iData->deviceData.queueProperties.graphicsQueueFamilyIndex,
        iData->deviceData.queueProperties.presentQueueFamilyIndex};

    VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.surface = iData->presentData.surface;
    swapchainCreateInfo.minImageCount =
        svkChooseSwapchainImageCount(&surfaceCapabilities);
    swapchainCreateInfo.imageFormat = surfaceFormat.format;
    swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
    swapchainCreateInfo.imageExtent =
        svkChooseSwapchainExtent(&surfaceCapabilities, iData);
    swapchainCreateInfo.imageArrayLayers = 1;
    swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainCreateInfo.imageSharingMode = samePresentAsGraphicsQueue
                                               ? VK_SHARING_MODE_EXCLUSIVE
                                               : VK_SHARING_MODE_CONCURRENT;
    swapchainCreateInfo.queueFamilyIndexCount = queueFamilyIndexCount;
    swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainCreateInfo.presentMode = svkChooseSwapchainPresentMode(iData);
    swapchainCreateInfo.clipped = VK_TRUE;
    swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(iData->deviceData.device, &swapchainCreateInfo,
                             NULL, &(presentData->swapchain)) != VK_SUCCESS) {
        printf("spritz: failed to create Vulkan swapchain!\n");
        return false;
    }

    presentData->imageFormat = swapchainCreateInfo.imageFormat;

    vkGetSwapchainImagesKHR(iData->deviceData.device, presentData->swapchain,
                            &presentData->imageCount, NULL);
    presentData->images = malloc(sizeof(VkImage) * presentData->imageCount);
    presentData->imageViews =
        malloc(sizeof(VkImageView) * presentData->imageCount);
    vkGetSwapchainImagesKHR(iData->deviceData.device, presentData->swapchain,
                            &presentData->imageCount, presentData->images);

    for (uint32_t i = 0; i < presentData->imageCount; i++) {
        VkImageViewCreateInfo imageViewCreateInfo = {};
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.image = presentData->images[i];
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format = presentData->imageFormat;
        imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.subresourceRange.aspectMask =
            VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;

        vkCreateImageView(iData->deviceData.device, &imageViewCreateInfo, NULL,
                          presentData->imageViews + i);
    }

    return true;
}

static bool svkDestroySwapchain(SpritzVKInternal_t* iData) {
    for (uint32_t i = 0; i < iData->presentData.imageCount; i++) {
        vkDestroyImageView(iData->deviceData.device,
                           iData->presentData.imageViews[i], NULL);
    }

    free(iData->presentData.images);
    free(iData->presentData.imageViews);
    iData->presentData.imageCount = 0;

    vkDestroySwapchainKHR(iData->deviceData.device,
                          iData->presentData.swapchain, NULL);

    return true;
}

bool spritzGraphicsAPIVKInit(void* uData,
                             SpritzGraphicsAPIInitInfo_t initInfo) {
    UDATA_INIT;

    iData->presentData.window = initInfo.window;

    RET_ASRT(glfwCreateWindowSurface(iData->instance, initInfo.window->window,
                                     NULL,
                                     &iData->presentData.surface) == VK_SUCCESS,
             "%s", "spritz: failed to create window surface!\n");

    RET_ASRT(svkLoadDeviceAndQueues(&iData->deviceData, iData), "%s",
             "spritz: failed to load vulkan device and queues!\n");

    RET_ASRT(svkInitPresentation(&iData->presentData, iData), "%s",
             "spritz: failed to initialize presentation!\n");
    return true;
}

bool spritzGraphicsAPIVKShutdown(void* uData) {
    UDATA_INIT;

    svkDestroySwapchain(iData);

    svkDestroyDevice(&iData->deviceData);
    vkDestroySurfaceKHR(iData->instance, iData->presentData.surface, NULL);

    svkDestroyDebugUtilsMessenger(iData);
    vkDestroyInstance(iData->instance, NULL);

    return true;
}

SpritzGraphicsAPIInternal_t spritzGraphicsAPIVKLoad() {
    SpritzGraphicsAPIInternal_t api = {};

    api.internalData = malloc(sizeof(SpritzVKInternal_t));

    api.PFN_preInit = spritzGraphicsAPIVKPreWindowSystemInit;
    api.PFN_init = spritzGraphicsAPIVKInit;

    api.PFN_shutdown = spritzGraphicsAPIVKShutdown;

    return api;
}
