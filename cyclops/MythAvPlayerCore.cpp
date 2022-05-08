#include "MythAvPlayerCore.h"
#include "mythStreamDecoder.hh"
#include "mythLive555Decoder.hh"
#include "MythH264VideoPlayer.hh"
wchar_t* char2wchar(const char *str);
HBAPI void* MYTHAPI Myth_ZiyaDecoder_Create(char* ip, int port, int cameraid){
	//char tmp[256] = { 0 };
	//sprintf(tmp, "%s:%d,%d", ip, port, cameraid);
	//SDL_ShowSimpleMessageBox(NULL, "HI", tmp, NULL);
	mythVirtualDecoder* Decoder = mythStreamDecoder::CreateNew(ip, port, cameraid);
	return (void*)Decoder;
}
HBAPI void* MYTHAPI Myth_Live555Decoder_Create(char* rtsp, char* username, char* password){
	mythVirtualDecoder* Decoder = mythLive555Decoder::CreateNew(rtsp,username,password);
	return (void*)Decoder;
}
HBAPI int MYTHAPI Myth_VideoPlayer_Capture(void* videoplayer){
	if(videoplayer){
		return ((MythVirtualVideoPlayer*)videoplayer)->capture();
	}
	return 0;
}
HBAPI int MYTHAPI Myth_VideoPlayer_SetText(void* videoplayer, char* text, int x, int y){
	if(videoplayer){
		return ((MythVirtualVideoPlayer*)videoplayer)->Create_Text(char2wchar(text),x,y);
	}
	return 0;
}
HBAPI int MYTHAPI Myth_VideoPlayer_SetAlpha(void* videoplayer,int alpha){
	if (videoplayer){
		return ((MythVirtualVideoPlayer*) videoplayer)->SetAlpha(alpha);
	}
	return 0;
}
HBAPI void* MYTHAPI Myth_H264VideoPlayer_Create(void* decoder, void* handle){
	if(decoder){
		MythH264VideoPlayer* player = MythH264VideoPlayer::CreateNew((mythVirtualDecoder*)decoder,handle);
		return (void*)player;
	}else
		return NULL;
}
HBAPI int MYTHAPI Myth_H264VideoPlayer_Start(void* videoplayer, int isthread){
	if(videoplayer){
		bool boolthread = isthread == 0?true:false;
		((MythH264VideoPlayer*)videoplayer)->start(boolthread);
		return 0;
	}else
		return 1;
}
HBAPI int MYTHAPI Myth_H264VideoPlayer_Stop(void* videoplayer){
	if(videoplayer){
		((MythH264VideoPlayer*)videoplayer)->stop();
		return 0;
	}
		return 1;
}
wchar_t* char2wchar(const char *str)
{
	wchar_t* buffer = NULL;
	if(str){
		size_t nu = strlen(str);
		size_t n =(size_t)MultiByteToWideChar(CP_ACP,0,(const char *)str,int(nu),NULL,0);
		buffer = new wchar_t[n+1];
		memset(buffer,0,sizeof(wchar_t)*(n+1));
		//if(n>=len) n=len-1;
		MultiByteToWideChar(CP_ACP,0,(const char *)str,int(nu),buffer,int(n));    
	}
	return buffer;
	//delete []buffer;
}