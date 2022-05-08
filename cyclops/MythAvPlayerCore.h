#pragma once
//#include <Windows.h>
#define MYTHAPI _stdcall
#ifdef __cplusplus   
	#define HBAPI extern "C" __declspec (dllexport)   
#else   
	#define HBAPI __declspec (dllexport)   
#endif   

HBAPI void* MYTHAPI Myth_ZiyaDecoder_Create(char* ip, int port, int cameraid);
HBAPI void* MYTHAPI Myth_Live555Decoder_Create(char* rtsp, char* username, char* password);
HBAPI void* MYTHAPI Myth_H264VideoPlayer_Create(void* decoder, void* handle);
HBAPI int MYTHAPI Myth_H264VideoPlayer_Start(void* videoplayer, int isthread);
HBAPI int MYTHAPI Myth_H264VideoPlayer_Stop(void* videoplayer);
HBAPI int MYTHAPI Myth_VideoPlayer_SetText(void* decoder, char* text, int x, int y);
HBAPI int MYTHAPI Myth_VideoPlayer_Capture(void* videoplayer); 
HBAPI int MYTHAPI Myth_VideoPlayer_SetAlpha(void* videoplayer, int alpha);