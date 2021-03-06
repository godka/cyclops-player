#pragma once
#include "mythvirtualdecoder.hh"
#include "myrtspClient.h"
#include "SDL.h"
class mythLive555Decoder :
	public mythVirtualDecoder
{
public:
	static int decodethreadstatic(void* data);
	int decodethread();
	static mythLive555Decoder* CreateNew(char* rtsplink,char* username,char* password);
	~mythLive555Decoder(void);
	static void callbackdatastatic(void *myth,unsigned char* data,unsigned int length);
	void callbackdata(unsigned char* data,unsigned int length);
	void start();
	void stop();
protected:
	mythLive555Decoder(char* rtsplink,char* username,char* password);
	UsageEnvironment *env;
	char *m_rtsplink,*m_username,*m_password;
	RTSPClient *rtspClient;
	SDL_Thread* startthread;
	char flag;
};

