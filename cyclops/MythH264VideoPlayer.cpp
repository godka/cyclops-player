#include "MythH264VideoPlayer.hh"
//#include "freeimage.h"
#include <time.h>

void MythH264VideoPlayer::disPlaySDL(char** data,int* dataline,int width,int height)
{
	myList->put((unsigned char**)data,(unsigned *)dataline,(unsigned)width,(unsigned)height);
	/*
	if(texture == NULL)
		texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING,width,height);
	
	SDL_UpdateYUVTexture(texture, NULL,
		(Uint8*)data[0], dataline[0],
		(Uint8*)data[1], dataline[1],
		(Uint8*)data[2], dataline[2]);
	
	//SDL_RendererFlip flip = SDL_FLIP_NONE;

	//SDL_RenderCopyEx(renderer, texture, NULL, NULL, 90, NULL, flip);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	if(Text_texture)SDL_RenderCopy(renderer, Text_texture, NULL, &Text_rect);
	if(capturebool){
		char tmpstr[1024];
		time_t t;struct tm *newtime;
		time(&t);newtime=localtime(&t);
		sprintf(tmpstr,"%04d-%02d-%02d-%02d-%02d-%02d.png",
		newtime->tm_year + 1900,newtime->tm_mon + 1,newtime->tm_mday,
		newtime->tm_hour,newtime->tm_min,newtime->tm_sec);
		capturebool = false;
		Uint8* tmpdata = (Uint8*)SDL_malloc(width * height * 4);
		ffmpeg->yuv2RGB(width,height,(const void**)data,dataline,(void**)&tmpdata);
		FIBITMAP* bitmap = FreeImage_ConvertFromRawBits(tmpdata,width,height,width * 4,32,0,0,0);
		FreeImage_FlipVertical(bitmap);
		FreeImage_Save(FIF_PNG, bitmap, tmpstr, PNG_DEFAULT);
		FreeImage_Unload(bitmap);
		SDL_free(tmpdata);
	}
	*/
	return;
}
void MythH264VideoPlayer::disPlaySDLstatic(void* hwnd,char** data,int* dataline,int width,int height){
	MythH264VideoPlayer* mythtmp = (MythH264VideoPlayer*)hwnd;
	mythtmp->disPlaySDL(data,dataline,width,height);
}
MythH264VideoPlayer::MythH264VideoPlayer(mythVirtualDecoder *decoder,void* hwnd,int width,int height)
	:MythVirtualVideoPlayer(decoder)
{
	Uint32 flags = NULL;
	myList = mythAvlist::CreateNew(5); //5M
	m_alpha = 0;
	capturebool = false;
	MythHwnd = hwnd;
	if (width == 0 || height == 0){
		// Declare display mode structure to be filled in.
		SDL_DisplayMode current;
		// Get current display mode of all displays.
		for (int i = 0; i < SDL_GetNumVideoDisplays(); ++i){

			int should_be_zero = SDL_GetCurrentDisplayMode(i, &current);

			if (should_be_zero != 0)
				// In case of error...
				SDL_Log("Could not get display mode for video display #%d: %s", i, SDL_GetError());

			else{
				MythWidth = current.w;
				MythHeight = current.h;
				flags = SDL_WINDOW_FULLSCREEN;
				break;
			}
		}
	}
	else{
		MythWidth = width;
		MythHeight = height;
	}
	texture = NULL;
	if(MythHwnd== NULL){
		SDL_Init(SDL_INIT_EVERYTHING);
		window = SDL_CreateWindow("MythKast",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			MythWidth, MythHeight, SDL_WINDOW_RESIZABLE);
	}
	else{
		SDL_InitSubSystem(SDL_INIT_VIDEO);	
		window =  SDL_CreateWindowFrom(MythHwnd);
	}
	if(!window)
		SDL_SetError("SDL Create Window Failed!");
	renderer = SDL_CreateRenderer(window,-1, SDL_RENDERER_PRESENTVSYNC);
//	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
//	SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
	if(!renderer)
		SDL_SetError("SDL Create render Failed!");
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");	
}

MythH264VideoPlayer* MythH264VideoPlayer::CreateNew(mythVirtualDecoder *decoder,void* hwnd,int width,int height){
	return new MythH264VideoPlayer(decoder,hwnd,width,height);
}

int MythH264VideoPlayer::capture(){
	
	char tmpstr[1024];
	time_t t;struct tm *newtime;
	time(&t);newtime=localtime(&t);
	sprintf(tmpstr,"%04d-%02d-%02d-%02d-%02d-%02d.bmp",
	newtime->tm_year + 1900,newtime->tm_mon + 1,newtime->tm_mday,
	newtime->tm_hour,newtime->tm_min,newtime->tm_sec);
	capturebool = false;
	PacketQueue *pkt = NULL;
	do{
		pkt = myList->get(1);
		SDL_Delay(20);
	}while(pkt == NULL);
	if(pkt){
		Uint8* tmpdata = (Uint8*)SDL_malloc(pkt->width * pkt->height * 4);
		ffmpeg->yuv2RGB(pkt->width,
			pkt->height,pkt->YY,pkt->UU,pkt->VV,
			pkt->Ydatasize,pkt->Udatasize,pkt->Vdatasize,(void**)&tmpdata);
#if 0
		FIBITMAP* bitmap = FreeImage_ConvertFromRawBits(tmpdata,pkt->width,pkt->height,pkt->width * 4,32,0,0,0);
		FreeImage_FlipVertical(bitmap);
		FreeImage_Save(FIF_PNG, bitmap, tmpstr, PNG_DEFAULT);
		FreeImage_Unload(bitmap);
#else
		SDL_Surface* tmpsurface = SDL_CreateRGBSurfaceFrom(tmpdata,pkt->width,pkt->height,32,pkt->width * 4,0xff0000,0xff00,0xff,0);
		SDL_SaveBMP(tmpsurface,tmpstr);
		SDL_FreeSurface(tmpsurface);
#endif
		SDL_free(tmpdata);
	}
/*	
	char tmpstr[1024];
	time_t t;struct tm *newtime;
	time(&t);newtime=localtime(&t);
	sprintf(tmpstr,"%04d-%02d-%02d-%02d-%02d-%02d.png",
	newtime->tm_year + 1900,newtime->tm_mon + 1,newtime->tm_mday,
	newtime->tm_hour,newtime->tm_min,newtime->tm_sec);
	//hello i wanna using freeimage
	SDL_Rect rect = {0,0,0,0};
	if(texture == NULL)
		return 1;
	//get pixels from render.
	SDL_QueryTexture(texture,NULL,NULL,&rect.w,&rect.h);
	BYTE* SDL_pixel = (BYTE*)SDL_malloc(rect.w * rect.h * 4);
	SDL_RenderReadPixels(renderer,&rect,NULL,SDL_pixel,rect.w * 4);
#if 0
	SDL_Surface* tmpsurface = SDL_CreateRGBSurfaceFrom(SDL_pixel,rect.w,rect.h,32,rect.w * 4,0xff0000,0xff00,0xff,0);
	SDL_SaveBMP(tmpsurface,tmpstr);
	SDL_FreeSurface(tmpsurface);
#else
	FIBITMAP* bitmap = FreeImage_ConvertFromRawBits(SDL_pixel,rect.w,rect.h,rect.w * 4,32,8,8,8);
	FreeImage_FlipVertical(bitmap);
	FreeImage_Save(FIF_PNG, bitmap, tmpstr, PNG_DEFAULT);
	FreeImage_Unload(bitmap);
#endif
*/
	return 0;
}
void MythH264VideoPlayer::start(bool hasthread){
	this->dethread = SDL_CreateThread(playthreadstatic,"decode",this);
	if(hasthread){
		startthread = SDL_CreateThread(decodethreadstatic,"decode",this);
	}else{
		startthread = NULL;
		this->decodethread();
	}
}
void MythH264VideoPlayer::stop(){
	this->flag = 1;
	if(!startthread)
		SDL_WaitThread(startthread,NULL);
	return;
}
int MythH264VideoPlayer::playthreadstatic(void* data){
	MythH264VideoPlayer* m_decoder = (MythH264VideoPlayer*)data;
	return m_decoder->playthread();
}
int MythH264VideoPlayer::decodethreadstatic(void* data){
	MythH264VideoPlayer* m_decoder = (MythH264VideoPlayer*)data;
	return m_decoder->decodethread();
}
int MythH264VideoPlayer::playthread()
{
	ffmpeg = mythFFmpeg::CreateNew(this);
	if(virtualdecoder == NULL)
		return 1;
	else
		virtualdecoder->start();
	while(flag == 0){
		SDL_PollEvent(&event);
		PacketQueue *pkt = virtualdecoder->get();
		if(pkt!=NULL){
			ffmpeg->ProcessFrame(pkt->h264Packet,pkt->h264PacketLength,disPlaySDLstatic);
			virtualdecoder->release(pkt);
		}else{
			//SDL_RenderCopy(renderer, Text_texture, NULL, &Text_rect);
			SDL_Delay(1);
		}
	}
	virtualdecoder->stop();
	return 0;
}
//#define SDL_DEPTH 50
int MythH264VideoPlayer::decodethread(){
	
	while(flag == 0){
		while(SDL_PollEvent(&event)){
			if (event.key.keysym.sym == SDLK_ESCAPE){
				ffmpeg->Cleanup();
				SDL_DestroyTexture(texture);
				//this->capture();
                return 0;
            }
			
		}
		PacketQueue *pkt = myList->get();
		if(pkt!=NULL){
			if (texture == NULL){
				texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, pkt->width, pkt->height);
				//SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_MOD);
				//SDL_SetTextureColorMod(texture, 99, 99, 99);
				//SDL_SetTextureAlphaMod(texture, 255);
			}
#if 1
			if (m_alpha){
				for (int i = 0; i < pkt->height; i++)
					for (int j = 0; j < pkt->Ydatasize; j++)
						pkt->YY[j + i*pkt->Ydatasize] = pkt->YY[j + i*pkt->Ydatasize] > 255 - m_alpha ? 255 : pkt->YY[j + i*pkt->Ydatasize] += m_alpha;
			}
#else
			//what the fuck?
#endif		
			if(SDL_UpdateYUVTexture(texture, NULL,
				(Uint8*)pkt->YY, pkt->Ydatasize,
				(Uint8*)pkt->UU, pkt->Udatasize,
				(Uint8*) pkt->VV, pkt->Vdatasize) < 0){
				printf("error!\n");
			}

			if (SDL_RenderCopy(renderer, texture, NULL, NULL) < 0){
				printf("error!!\n");
			}
			int speed = virtualdecoder->getSpeed();
			if (speed >= 0){
				char tmp[255] = { 0 };
				sprintf(tmp, "%dk/s", speed);
				wchar_t* tmpw = getWchar_t(tmp);
				Create_Text(tmpw, 15, 15);
				SDL_RenderCopy(renderer, Text_texture, NULL, &Text_rect);
				free(tmpw);
			}
			//SDL_SetTextureBlendMode(texture,SDL_BLENDMODE_BLEND);
			myList->release(pkt);
		}
		else{
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			SDL_RenderCopy(renderer, Text_texture, NULL, &Text_rect);
			SDL_Delay(1);
		}
		SDL_RenderPresent(renderer);
	}
	ffmpeg->Cleanup();
	SDL_DestroyTexture(texture);
	//SDL_DestroyRenderer(renderer);
	//SDL_DestroyWindow(window);
	//SDL_Quit();
	return 0;
}
MythH264VideoPlayer::~MythH264VideoPlayer(void)
{
}

int MythH264VideoPlayer::SetAlpha(int alpha)
{
	m_alpha = alpha;
	return m_alpha;
}
