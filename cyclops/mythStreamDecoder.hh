#pragma once
#include "mythvirtualdecoder.hh"
#include "SDL.h"
#include "MythSocket.hh"
class mythStreamDecoder :
	public mythVirtualDecoder
{
public:
	static mythStreamDecoder* CreateNew(char* ip, int port, int CameraID);
	static mythStreamDecoder* CreateNew(char* ip, int CameraID);
	void start();
	void stop();
	unsigned int GetTimeCount();
protected:
	int decodethread();
	static int decodethreadstatic(void* data);
	mythStreamDecoder(char* ip, int port, int CameraID);
	~mythStreamDecoder(void);
	MythSocket *msocket;
	SDL_Thread* startthread;
	char* m_ip;
	int m_port;
	int m_cameraid;
};

