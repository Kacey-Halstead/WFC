#include "SDL_Window.h"
#include "Grid.h"

#include <iostream>
#include <vector>
#include <SDL_image.h>

SDL_Window::SDL_Window()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) //initialise SDL
	{
		std::string error{ "Unable to initialise SDL: ", SDL_GetError() };
		throw std::runtime_error(error);
	}

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		std::string error{ "Error SDL2_image Initialization", IMG_GetError() };
		throw std::runtime_error(error);
	}

	//Create window
	window = SDL_CreateWindow("WFC", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 900, 900, SDL_WINDOW_OPENGL); //name, horizontal pos, vertical pos, width, height, mode
	if (!window) {
		std::string error{ "Error window creation", SDL_GetError() };
		throw std::runtime_error(error);
	}

	//Create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); //window to render to, index of rendering driver (-1 = first available), mode
	if (!renderer) {
		std::string error{ "Failed to create SDL Renderer: ", SDL_GetError() };
		throw std::runtime_error(error);
	}
}

SDL_Window::~SDL_Window()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}

void SDL_Window::BeginRender()
{
	SDL_RenderClear(renderer); //remove anything already rendered
}

void SDL_Window::EndRender()
{
	SDL_RenderPresent(renderer); //moving from back buffer to screen
}

bool SDL_Window::Events()
{
	SDL_Event e;
	if (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			return false;
		}
		if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE && e.window.windowID == SDL_GetWindowID(window))
		{
			return false;
		}
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			WFC::WFCReset();
		}
	}

	return true;
}
