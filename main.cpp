/*
 * Copyright (C) 2009-2018 Josh A. Beam
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <iostream>
#include <cmath>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "Rasterizer.h"

using namespace std;

static const float M_PI_F = (float)M_PI;
static const int WINDOW_WIDTH = 320;
static const int WINDOW_HEIGHT = 240;

static bool g_Running = true;

static void
HandleKeyEvent(const SDL_Event &event)
{
	switch(event.key.keysym.sym) {
		default:
			break;
		case SDLK_ESCAPE:
			g_Running = false;
			break;
	}
}

static void
HandleEvent(const SDL_Event &event)
{
	switch(event.type) {
		default:
			break;
		case SDL_QUIT:
			g_Running = false;
			break;
		case SDL_KEYDOWN:
			HandleKeyEvent(event);
			break;
	}
}

static void
HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		HandleEvent(event);
	}
}

int
main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cerr << "SDL_Init failed" << endl;
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("Triangle Rasterization Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if (!window) {
		cerr << "SDL_CreateWindow failed" << endl;
		SDL_Quit();
		return 1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		cerr << "SDL_CreateRenderer failed" << endl;
		SDL_Quit();
		return 1;
	}

	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!texture) {
		cerr << "SDL_CreateTexture failed" << endl;
		SDL_Quit();
		return 1;
	}

	Rasterizer rast;
	float r = 0.0f;
	unsigned int lastTicks = SDL_GetTicks();

	// loop until we're done running the program
	while (g_Running) {
		HandleEvents();

		void *pixels;
		int pitch;
		if (SDL_LockTexture(texture, NULL, &pixels, &pitch) != 0) {
			cerr << "SDL_LockTexture failed" << endl;
			break;
		}

		rast.SetFrameBuffer((uint32_t *)pixels, WINDOW_WIDTH, WINDOW_HEIGHT);
		rast.Clear();

		// calculate coordinates for triangle
		const float size = 110.0f;
		float x1 = (float)(WINDOW_WIDTH / 2) + cosf(r - M_PI_F / 6.0f) * size;
		float y1 = (float)(WINDOW_HEIGHT / 2) + sinf(r - M_PI_F / 6.0f) * size;
		float x2 = (float)(WINDOW_WIDTH / 2) + cosf(r + M_PI_F / 2.0f) * size;
		float y2 = (float)(WINDOW_HEIGHT / 2) + sinf(r + M_PI_F / 2.0f) * size;
		float x3 = (float)(WINDOW_WIDTH / 2) + cosf(r + M_PI_F + M_PI_F / 6.0f) * size;
		float y3 = (float)(WINDOW_HEIGHT / 2) + sinf(r + M_PI_F + M_PI_F / 6.0f) * size;

		// colors for each point of the triangle
		Color color1(1.0f, 0.0f, 0.0f);
		Color color2(0.0f, 1.0f, 0.0f);
		Color color3(0.0f, 0.0f, 1.0f);

		// render triangle
		rast.DrawTriangle(color1, x1, y1, color2, x2, y2, color3, x3, y3);

		SDL_UnlockTexture(texture);

		if (SDL_RenderClear(renderer) != 0) {
			cerr << "SDL_RenderClear failed" << endl;
			break;
		}

		if (SDL_RenderCopy(renderer, texture, NULL, NULL) != 0) {
			cerr << "SDL_RenderCopy failed" << endl;
			break;
		}

		SDL_RenderPresent(renderer);

		// calculate the number of seconds that
		// have passed since the last update
		unsigned int ticks = SDL_GetTicks();
		unsigned int ticksDiff = ticks - lastTicks;
		if (ticksDiff == 0) {
			continue;
		}
		float time = ticksDiff / 1000.0f;
		lastTicks = ticks;

		// update rotation
		r += M_PI_F / 2.0f * time;

		// display frames per second
		unsigned int fps = 1000 / ticksDiff;
		cout << "Frames per second: " << fps << "     \r";
	}

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
