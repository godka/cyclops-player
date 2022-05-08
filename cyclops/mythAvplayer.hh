#pragma once
#include "MythSocket.hh"
#include "mythFFmpeg.hh"
#include "List.h"
#include "SDL.h"
class mythAvplayer
{
public:
	static mythAvplayer *CreateNew(void* phwnd,char* ip,int cameraid);
	static mythAvplayer *CreateNew(char* ip,int cameraid);
	mythAvplayer(char* ip,int port,int cameraid,void* phwnd);
	int startDecode(int width,int height);
	~mythAvplayer(void);
protected:
	char* connectip;// args[1]
	int connectid;//atoi(args[2])
	int connectport;
	int initSDL(int screenwidth,int screenheight);
	static int playyuv_static(void* mythav);
	static void disPlaySDL_static(void *mythavplayer,char** data,int* dataline,int width,int height);
	int disPlaySDL(char** data,int* dataline,int width,int height);
	int playyuv(void);
	bool flag;
protected:
	struct list_head ListPlay;
	void* hwnd;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event event;
	SDL_Texture* texture;
	AVFrame *picture;
	SDL_Rect s_rect;
	SDL_Rect d_rect;
	int m_width,m_height;

protected:
	SDL_Thread *playthread;
	MythSocket *msocket;
	mythFFmpeg *ffmpeg;
	//SDL_Thread *socketthread;
};

