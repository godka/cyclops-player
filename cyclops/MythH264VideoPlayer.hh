#pragma once
#include "mythvirtualvideoplayer.hh"
#include "mythFFmpeg.hh"
class MythH264VideoPlayer :
	public MythVirtualVideoPlayer
{
public:
	static void disPlaySDLstatic(void* hwnd,char** data,int* dataline,int width,int height);
	void disPlaySDL(char** data,int* dataline,int width,int height);
	static MythH264VideoPlayer* CreateNew(mythVirtualDecoder *decoder,void* hwnd=NULL,int width=0,int height=0);
	~MythH264VideoPlayer(void);
	void start(bool hasthread = false);
	void stop();
	int SetAlpha(int alpha);
	int capture();
protected:
	void* MythHwnd;
	int MythWidth,MythHeight;

	MythH264VideoPlayer(mythVirtualDecoder *decoder,void* hwnd,int width,int height);
	SDL_Event event;
	SDL_Texture* texture;

	mythFFmpeg *ffmpeg;
	mythAvlist* myList;

	int decodethread();
	int playthread();
	static int decodethreadstatic(void* data);
	static int playthreadstatic(void* data);
	SDL_Thread* startthread;
	SDL_Thread* dethread;
	bool capturebool;
	int m_alpha;
};

