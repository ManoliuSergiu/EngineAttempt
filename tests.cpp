#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>
#include "src/gl.c"
#include <string>
static SDL_Window *window = NULL; 
static SDL_Renderer *renderer = NULL;
static SDL_GLContext context = NULL;
static uint32_t id;

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

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    window = SDL_CreateWindow("My window",
         640, 480, 
         SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if (!window) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    context = SDL_GL_CreateContext(window); 
	SDL_GL_MakeCurrent(window,context);
	if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress)) {
    // Handle error: Could not load OpenGL functions
	}
	glViewport(0,0,640,480);
	


	std::string basepath=SDL_GetBasePath();

	std::string fullPath = basepath + "shaders/shader";
	GLuint vshader = glCreateShader(GL_VERTEX_SHADER); 
	
	uint64_t len;
	void* buff=SDL_LoadFile((fullPath+".vert").c_str(),&len);
	std::string source=(static_cast<const char*>(buff));
	const char *c_str_pointer=(source.c_str());
	glShaderSource(vshader,1,&c_str_pointer,NULL);
	glCompileShader(vshader);


	int success;
    char infoLog[512];
    glGetShaderiv(vshader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vshader, 512, NULL, infoLog);
        SDL_Log("Couldn't create window/renderer: %s", infoLog);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"ERROR",infoLog,window);
    }



	GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER); 
	uint64_t len2;
	void* buff2=SDL_LoadFile((fullPath+".frag").c_str(),&len2);
	std::string source2=(static_cast<const char*>(buff2));
	const char *c_str_pointer2=(source2.c_str());
	glShaderSource(fshader,1,&c_str_pointer2,NULL);
	glCompileShader(fshader);

    glGetShaderiv(fshader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fshader, 512, NULL, infoLog);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"ERROR",infoLog,window);
        SDL_Log("Couldn't create window/renderer: %s", infoLog);
    }

	SDL_free(buff);
	SDL_free(buff2);
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vshader);
	glAttachShader(shaderProgram, fshader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"ERROR",infoLog,window);
    }

	glDeleteShader(vshader);
	glDeleteShader(fshader);  

	glGenVertexArrays(1, &VAO);
  		
	glGenBuffers(1, &VBO);  
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



    glBindVertexArray(0); 
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
	
		glViewport(0,0,x,y);
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
	glClearColor(r,g,b,.9f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 


	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES,0,3);
	SDL_GL_SwapWindow(window);
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */ 
	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}


