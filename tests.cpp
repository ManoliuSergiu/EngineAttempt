#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_vulkan.h>
#include "src/volk.h"
#include <string>



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
    SDL_SetAppMetadata("My first window","0.0","com.test.test1");

    window=SDL_CreateWindow("ManNG",800,600,SDL_WINDOW_VULKAN);

	if (volkInitialize() != VK_SUCCESS) {
        SDL_Log("Critical Error: Vulkan loader not found in the system.");
        return SDL_APP_FAILURE;
    }

    // 2. Query SDL for required Extensions
    // SDL knows if you need VK_KHR_win32_surface or VK_EXT_metal_surface
    Uint32 extensionCount = 0;
    const char* const* sdlExtensions = SDL_Vulkan_GetInstanceExtensions(&extensionCount);

    if (sdlExtensions == nullptr) {
        SDL_Log("Error: SDL could not fetch Vulkan extensions: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // 3. Create the Vulkan Instance
    // This is the "Root" of your Vulkan application
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "My Vulkan Engine";
    appInfo.apiVersion = VK_API_VERSION_1_2; // Safe target for 2026 (Supports Mac/MoltenVK)

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    
    // Pass the extensions SDL gave us to Vulkan
    createInfo.enabledExtensionCount = extensionCount;
    createInfo.ppEnabledExtensionNames = sdlExtensions;
    
    // (Optional) Enable Validation Layers here for debugging
    createInfo.enabledLayerCount = 0; 

    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS) {
        SDL_Log("Failed to create Vulkan Instance! Error code: %d", result);
        return SDL_APP_FAILURE;
    }

    // 4. Load Instance Functions via Volk
    volkLoadInstance(instance);

    // 5. Create the Surface
    // SDL handles the complex platform-specific logic here automatically
    if (!SDL_Vulkan_CreateSurface(window, instance, nullptr, &surface)) {
        SDL_Log("Failed to create Vulkan Surface: %s", SDL_GetError());
        return SDL_APP_FAILURE;
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

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{


	//BGCOLOR
    const double now = ((double)SDL_GetTicks()) / 1000.0;  /* convert from milliseconds to seconds. */
    /* choose the color for the frame we will draw. The sine wave trick makes it fade between colors smoothly. */
	


	
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */ 

}


