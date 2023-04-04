#include "graphics_api_vk.h"

#include "GLFW/glfw3.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <string.h>

typedef struct {
    VkInstance instance;

    bool validationEnabled;
    VkDebugUtilsMessengerEXT debugMessenger;
} SpritzVKInternal_t;

#define UDATA_INIT SpritzVKInternal_t* iData = uData
#define RET_ASRT(x, fmt, ...)                                                  \
    if (!x) {                                                                  \
        printf(fmt, __VA_ARGS__);                                              \
        return false;                                                          \
    }

#define MAX_EXTENSIONS 16
typedef struct {
    char* extensions[MAX_EXTENSIONS];
    uint32_t count;
} ExtensionsInfo_t;

static ExtensionsInfo_t
svkGetRequiredExtensions(const SpritzVKInternal_t* iData) {
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
    printf("begin here\n");
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

    VkLayerProperties* layerProperties = alloca(sizeof(VkLayerProperties));
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

    printf("down here 1\n");

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

    ExtensionsInfo_t properties = svkGetRequiredExtensions(iData);

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
    printf("here\n");

    printf("nExtensions: %d\n", instanceCreateInfo.enabledExtensionCount);
    printf("enabled extensions:\n");
    for (int i = 0; i < instanceCreateInfo.enabledExtensionCount; i++) {
        printf("\t%s\n", instanceCreateInfo.ppEnabledExtensionNames[i]);
    }
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

    printf("right here\n");
    RET_ASRT(svkLoadInstance(&iData->instance, iData), "%s",
             "spritz: failed to create vulkan instance!\n")
    volkLoadInstance(iData->instance);
    if (iData->validationEnabled) {
        printf("down here0\n");
        RET_ASRT(svkCheckValidationLayerSupport(iData->instance), "%s",
                 "spritz: validation layers requested, but the required don't "
                 "exist!\n")
        printf("down here1\b");
        RET_ASRT(svkSetupDebugMessenger(&iData->debugMessenger, iData), "%s",
                 "spritz: failed to create debug utils messenger!\n");
    }

    return true;
}

bool spritzGraphicsAPIVKInit(void* data, SpritzGraphicsAPIInitInfo_t initInfo) {
    return true;
}

SpritzGraphicsAPIInternal_t spritzGraphicsAPIVKLoad() {
    SpritzGraphicsAPIInternal_t api = {};

    api.internalData = malloc(sizeof(SpritzVKInternal_t));

    api.PFN_preInit = spritzGraphicsAPIVKPreWindowSystemInit;
    api.PFN_init = spritzGraphicsAPIVKInit;

    return api;
}
