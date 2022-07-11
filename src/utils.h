#pragma once
#include "shitrndr/src/shitrndr.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <cstdint>
#include <string>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_test_font.h>
#include <SDL2/SDL.h>

template<typename T>
inline T lerp(const T& a, const T& b, float d) { return a+(b-a)*d; }

inline float frand() { return float(std::rand()) / float(RAND_MAX); }

#ifdef DEBUG_TEXT
inline static TTF_Font* debug_font;
inline static bool debug_text_init = 0;
inline static int lc = 0;
#endif
inline void printDebugLine(const std::string& l, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255)
{
#ifdef DEBUG_TEXT
	if(debug_text_init && !debug_font) return;
	if(!debug_text_init)
	{
		TTF_Init();
		debug_font = TTF_OpenFont("res/font.ttf", 16);
		if(!debug_font)
		{
			std::cerr << "Utils debug text error: Couldn't load font file 'res/font.ttf'" << std::endl;
			return;
		}
		debug_text_init = 1;
	}
	SDL_Surface* s = TTF_RenderText_Blended(debug_font, l.c_str(), {r,g,b,255});
	SDL_Texture* t = SDL_CreateTextureFromSurface(shitrndr::ren, s);
	int w, h;
	SDL_QueryTexture(t, 0, 0, &w, &h);
	shitrndr::Copy(t, {0,lc*12, w, h});
	lc++;
	SDL_FreeSurface(s);
	SDL_DestroyTexture(t);
#endif
}
inline void clearDebugLines()
{
#ifdef DEBUG_TEXT
	lc = 0;
#endif
}
inline void printDebugFPS(float t)
{
#ifdef DEBUG_TEXT
	static float last = 0, lastf = 0;
	static uint32_t count = 0, lcount = 0;
	count++;
	if(t-last>=1)
	{
		lcount = count;
		count = 0;
		last = t;
	}
	printDebugLine("frames: "+std::to_string(lcount));
	printDebugLine("delta: "+std::to_string(t-lastf));
	lastf = t;
#endif
}
