#pragma once
#include "SDL.h"
#include "List.h"
typedef struct PacketQueue {
    unsigned char* h264Packet;
	unsigned int h264PacketLength;
	//only for mobile
    unsigned char* YY;
    unsigned char* UU;
    unsigned char* VV;
    unsigned int width,height;
    unsigned int Ydatasize,Udatasize,Vdatasize;
    //unsigned int PacketCount;
	struct list_head list;
} PacketQueue;
class mythAvlist
{
public:
	static mythAvlist* CreateNew(int BufferSize = -1);
	~mythAvlist(void);
	PacketQueue *get(int freePacket = 0);
	int put(unsigned char* data,unsigned int length);
	int put(unsigned char** dataline,unsigned int *datasize,unsigned int width,unsigned int height);
	int release(PacketQueue *pack);
	int free();
protected:
	mythAvlist(void);
	mythAvlist(int BufferSize);
	struct list_head header;
    int abort_request;
    SDL_mutex *mutex;
	unsigned char* totalbuffer;
	unsigned int totalptr;
	unsigned int listcount;
	int putcore(unsigned char* data,unsigned int datasize,unsigned char** dst);
};

