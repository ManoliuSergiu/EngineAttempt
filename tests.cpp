#define SDL_MAIN_USE_CALLBACKS 1
#define VK_NO_PROTOTYPES
#include "src/volk.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_vulkan.h>
#include <string>
#include <vector>
#include <set>
#include <optional>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() { 
        return graphicsFamily.has_value() && presentFamily.has_value(); 
    }
};
const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME

};

bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
    for (const auto& extension : availableExtensions) 
        requiredExtensions.erase(extension.extensionName);
	return requiredExtensions.empty();
}
QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
    QueueFamilyIndices indices;
    
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        
        if (presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}

static SDL_Window *window = nullptr;

VkInstance instance;
VkSurfaceKHR surface;



unsigned int shaderProgram;
unsigned int VBO;
unsigned int VAO;
float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};  
 

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	if (volkInitialize() != VK_SUCCESS) {
        SDL_Log("Critical Error: Vulkan loader not found in the system.");
        return SDL_APP_FAILURE;
    }
    window=SDL_CreateWindow("ManNG",800,600,SDL_WINDOW_VULKAN);
	SDL_Vulkan_LoadLibrary(nullptr);
    Uint32 extensionCount = 0;
    const char* const* sdlExtensions = SDL_Vulkan_GetInstanceExtensions(&extensionCount);

    if (sdlExtensions == nullptr) {
        SDL_Log("Error: SDL could not fetch Vulkan extensions: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
	
    SDL_SetAppMetadata("My first window","0.0","com.test.test1");

// ---------------------------------------------------------
    // STEP A: Prepare the Extension List (Dynamic Check)
    // ---------------------------------------------------------
    
    // 1. Get all extensions supported by the driver (not just what SDL wants)
    uint32_t availableExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions.data());

    // 2. Create a list of extensions to enable (start with SDL's list)
    std::vector<const char*> extensions(sdlExtensions, sdlExtensions + extensionCount);

    // 3. Check if the Portability extension is available (Mac/MoltenVK needs this)
    bool portabilityAvailable = false;
    for (const auto& ext : availableExtensions) {
        if (strcmp(ext.extensionName, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME) == 0) {
            portabilityAvailable = true;
            break;
        }
    }

    // 4. If available, add it to the list
    VkInstanceCreateFlags createFlags = 0;
    if (portabilityAvailable) {
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        // Also add this dependency which is often required by portability
        extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME); 
        
        createFlags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    }

    // ---------------------------------------------------------
    // STEP B: Create the Instance
    // ---------------------------------------------------------

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "My Vulkan Engine";
    
    // !!! CRITICAL FIX FOR ERROR -9 !!!
    // If your driver is slightly old, requesting 1.2 or 1.4 causes immediate failure.
    // Start with 1.1. If this works, you can try bumping it up later.
    appInfo.apiVersion = VK_API_VERSION_1_4; 

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    
    // Use our dynamic list and flags
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    createInfo.flags = createFlags;
    
    createInfo.enabledLayerCount = 0; 

    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);


    volkLoadInstance(instance);

    if (!SDL_Vulkan_CreateSurface(window, instance, nullptr, &surface)) {
        SDL_Log("Failed to create Vulkan Surface: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
	QueueFamilyIndices indices;
	VkPhysicalDevice physicalDevice;

	for(auto& device : devices){
		QueueFamilyIndices currentIndices = findQueueFamilies(device,surface);
		bool extensionsSupported = checkDeviceExtensionSupport(device);
		VkPhysicalDeviceProperties properties;
    	vkGetPhysicalDeviceProperties(device, &properties);
    	bool isDiscrete = properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
		if (currentIndices.isComplete() && extensionsSupported) {
        
			physicalDevice = device;
			indices = currentIndices; 
			
			if (isDiscrete) {
				break; 
			}
		}

	}
	if (physicalDevice == VK_NULL_HANDLE) {
		throw std::runtime_error("failed to find a suitable GPU!");
	}
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}
 

/* This function runs when a new event (mouse input, keypresses, etc.) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }

	if(event->type == SDL_EVENT_WINDOW_RESIZED)
	{
		int x=1,y=2;
		if (!SDL_GetWindowSize(window,&x,&y))
		{
			 SDL_Log("Couldn't resize window/renderer: %s", SDL_GetError());
		}

		
		
	}
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

SDL_AppResult SDL_AppIterate(void *appstate)
{


    const double now = ((double)SDL_GetTicks()) / 1000.0;  /* convert from milliseconds to seconds. */
	


	
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{

}


