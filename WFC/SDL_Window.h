#pragma once
#include <SDL.h>
#include <vector>

#include"WFC.h"

class SDL_Window
{
public:

	SDL_Window();
	~SDL_Window();

	void BeginRender();
	void EndRender();
	bool Events();

	inline SDL_Window* getWindow() { return window; }
	inline SDL_Renderer* getRenderer() { return renderer; }

private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
};

