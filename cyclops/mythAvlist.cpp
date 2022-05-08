/********************************************************************
  Created by MythKAst
  ©2013 MythKAst Some rights reserved.


  You can build it with vc2010,gcc.
  Anybody who gets this source code is able to modify or rebuild it anyway,
  but please keep this section when you want to spread a new version.
  It's strongly not recommended to change the original copyright. Adding new version
  information, however, is Allowed. Thanks.
  For the latest version, please be sure to check my website:
  Http://code.google.com/mythkast


  你可以用vc2010,gcc编译这些代码
  任何得到此代码的人都可以修改或者重新编译这段代码，但是请保留这段文字。
  请不要修改原始版权，但是可以添加新的版本信息。
  最新版本请留意：Http://code.google.com/mythkast

 MythKAst(asdic182@sina.com), in 2013 June.
*********************************************************************/
#include "mythAvlist.hh"
#include <stdio.h>
#ifdef ANDROID
#include <android/log.h>
#define LOG_TAG "org.app.sdl"

#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)w
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#else
#define LOGE printf
#endif
#define AVFRAMECOUNT 25
#define AVTOTALBUFFERCOUNT 5 * 1024 * 1024//5M
mythAvlist::mythAvlist(void)
{
	listcount = 0;
	this->mutex = SDL_CreateMutex();
	this->totalbuffer = (unsigned char*)SDL_malloc(AVTOTALBUFFERCOUNT);
	this->totalptr = 0;
	InitalList();
}
int mythAvlist::InitalList(void){
	//inital list
	ListPacket = (PacketQueue**)SDL_malloc(AVFRAMECOUNT * sizeof(PacketQueue));
	for(int i = 0;i < AVFRAMECOUNT;i++){
		ListPacket[i] = (PacketQueue*)SDL_malloc(sizeof(PacketQueue));
		ListPacket[i]->YY = NULL;
		ListPacket[i]->UU = NULL;
		ListPacket[i]->VV = NULL;
		ListPacket[i]->h264Packet = NULL;
	}
	listwrite = 0;
	listread = 0;
	return 0;
}
mythAvlist::mythAvlist(int BufferSize)
{
	listcount = 0;
	mutex = SDL_CreateMutex();
	totalbuffer = (unsigned char*)SDL_malloc(BufferSize * 1024 * 1024);
	totalptr = 0;
	InitalList();
}
mythAvlist *mythAvlist::CreateNew(int BufferSize){
	if(BufferSize == -1)
		return new mythAvlist();
	else
		return new mythAvlist(BufferSize);
}
mythAvlist::~mythAvlist(void)
{
	free();
}
PacketQueue *mythAvlist::get(int freePacket){
	SDL_LockMutex(this->mutex);
	PacketQueue *tmp;
	if (this->listwrite - this->listread == 1 || 
		this->listwrite - this->listread == 0 ||
		(this->listwrite == 0 && this->listread == AVFRAMECOUNT - 1))
		tmp = NULL;
	else{
		tmp = this->ListPacket[this->listread];
		if(tmp->YY == NULL && tmp->h264Packet == NULL){
			tmp = NULL;
		}else{
			if(freePacket == 0){
				if(listwrite - listread > 5){
					LOGE("skip frames");
					LOGE("read = %d,write = %d,minus = %d\n",listread,listwrite,listwrite - listread);
					listread += 4;
				}else
					listread++;
			}
		}
	}
	listread %= AVFRAMECOUNT;
	//if(listread >= AVFRAMECOUNT)listread = 0;
	SDL_UnlockMutex(this->mutex);
	/*
	PacketQueue *tmp;
	SDL_LockMutex(this->mutex);
	if(header.next == header.prev){
		tmp = NULL;
	}else{
		tmp = list_entry(header.next,struct PacketQueue,list);
		if(freePacket == 0)
			list_del(&tmp->list);
		listcount--;
	}
	SDL_UnlockMutex(this->mutex);
	*/
	return tmp;
}
int mythAvlist::putcore(unsigned char* data,unsigned int datasize,unsigned char** dst){
	if(totalptr + datasize >= AVTOTALBUFFERCOUNT)totalptr = 0;
	*dst = totalbuffer + totalptr;
	SDL_memcpy(totalbuffer + totalptr,data,datasize);
	totalptr += datasize;
	//printf("totalptr = %d\n",totalptr);
	return 0;
}
int mythAvlist::put(unsigned char** dataline,unsigned int *datasize,unsigned int width,unsigned int height){
	SDL_LockMutex(this->mutex);
	if(listwrite >= AVFRAMECOUNT)listwrite = 0;
	PacketQueue *tmp = this->ListPacket[listwrite];
	tmp->h264Packet = NULL;
	tmp->width = width;
	tmp->height = height;

	this->putcore(dataline[0],datasize[0] * height,&tmp->YY);
	tmp->Ydatasize = datasize[0];
	
	this->putcore(dataline[1],datasize[1] * height / 2,&tmp->UU);
	tmp->Udatasize = datasize[1];
	
	this->putcore(dataline[2],datasize[2] * height / 2,&tmp->VV);
	tmp->Vdatasize = datasize[2];

	listwrite++;
	//LOGE("YUVlistcount=%d\n",listwrite);
	SDL_UnlockMutex(this->mutex);
	return 0;
}
int mythAvlist::release(PacketQueue *pack)
{
	/*
	if(pack->YY != NULL)SDL_free(pack->YY);
	if(pack->UU != NULL)SDL_free(pack->UU);
	if(pack->VV != NULL)SDL_free(pack->VV);
	if(pack->h264Packet != NULL)SDL_free(pack->h264Packet);
	*/
	/*
	if(pack!= NULL)
		SDL_free(pack);
		*/
	return 0;
}
int mythAvlist::put(unsigned char* data,unsigned int length){	
	
	SDL_LockMutex(this->mutex);
	if(listwrite >= AVFRAMECOUNT)listwrite = 0;
	PacketQueue *tmp = this->ListPacket[listwrite];

	tmp->YY = NULL;
	tmp->UU = NULL;
	tmp->VV = NULL;


	tmp->h264PacketLength = length;
	putcore(data,length,&tmp->h264Packet);
	listwrite++;
	//LOGE("H264listcount=%d\n",listwrite);
	SDL_UnlockMutex(this->mutex);
	return 0;
}
int mythAvlist::free(){
	SDL_DestroyMutex(mutex);
	return 0;
}
