#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_vulkan.h>
#include "src/gl.c"
#include <string>
static SDL_Window *window = NULL; 
static SDL_GLContext context = NULL;

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

    
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}
 

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
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
	
	float r = .5f+ SDL_sinf(now/10)*.5f;
	float g = .5f+ SDL_sinf(now/5)*.5f;
	float b = .5f+ SDL_sinf(now/15)*.5f;

	
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */ 

}


