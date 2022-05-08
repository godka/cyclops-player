#pragma once
#include <sstream>
#include "mythVirtualDecoder.hh"
extern "C"{
#include "SDL.h"
#include "SDL_ttf.h"
}
using namespace std;
class MythVirtualVideoPlayer
{
public:
	static MythVirtualVideoPlayer* CreateNew(mythVirtualDecoder* virtualdecoder);
	virtual void start();
	virtual void stop();
	virtual void pause();
	virtual void fast_next();
	virtual void slow_next();
	virtual int capture();
	virtual int SetAlpha(int alpha);
#ifdef _WIN32
	//	int Create_Text(wchar_t* text,int x,int y);
	int Create_Text(wchar_t* text, int x, int y);
#else
	//using utf-8
	int Create_Text(const char* text, int x, int y);
#endif
protected:
	int flag;
	mythVirtualDecoder* virtualdecoder;
	MythVirtualVideoPlayer(mythVirtualDecoder* decoder);
	virtual ~MythVirtualVideoPlayer(void);
	SDL_Texture* Text_texture;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Rect Text_rect;
	TTF_Font* font;
	wchar_t * getWchar_t(char* cstr){
		size_t len = strlen(cstr) + 1;
		wchar_t *WStr; size_t converted = 0;
		WStr = (wchar_t*) malloc(len*sizeof(wchar_t));
		mbstowcs_s(&converted, WStr, len, cstr, _TRUNCATE);
		return WStr;
	}
};

