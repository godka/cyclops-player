#include "mythAvplayer.hh"

typedef struct DisplayList{
    uint8_t* data[3];
    int linesize[3];
    struct list_head list;
}DisplayList;

mythAvplayer::mythAvplayer(char* ip,int port,int cameraid,void* phwnd)
{
	this->hwnd = phwnd;
	this->connectip = ip;
	this->connectport = port;
	this->connectid = cameraid;
	ListPlay.next = &ListPlay;
	ListPlay.prev = &ListPlay;
	//LIST_HEAD(ListPlay);
	flag = false;
}
int mythAvplayer::disPlaySDL(char** data,int* dataline,int width,int height)
{
	int act_height = 0;
	if(texture == NULL){
		texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING,width,height);
		s_rect.x = 0;
		s_rect.y = 0;
		s_rect.w = width;
		s_rect.h = height;
	}
	DisplayList *tmpdisplay = (DisplayList*)SDL_malloc(sizeof(DisplayList));
	for(int i = 0;i < 3;i++){
		act_height = i==0?height:height>>1;
		tmpdisplay->data[i] = (uint8_t*)SDL_malloc(dataline[i] * act_height);
		tmpdisplay->linesize[i] = dataline[i];
		SDL_memcpy(tmpdisplay->data[i],data[i],dataline[i] * act_height);
	}
	list_add_tail(&tmpdisplay->list,&ListPlay);
	return 0;
}
void mythAvplayer::disPlaySDL_static(void *mythavplayer,char** data,int* dataline,int width,int height)
{
	mythAvplayer* player = (mythAvplayer*)mythavplayer;
	player->disPlaySDL(data,dataline,width,height);
	return;
}

int mythAvplayer::initSDL(int screenwidth,int screenheight){
  /* Init SDL */
	if( SDL_Init(SDL_INIT_EVERYTHING)  == -1)
	{
		return 1;
	}
	//display in mfc?
	if(this->hwnd == NULL){
		window = SDL_CreateWindow("MythKast",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			screenwidth,screenheight,0);
	}else{
		window = SDL_CreateWindowFrom(this->hwnd);
	}
	if(window == NULL)return 1;
	renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_PRESENTVSYNC);
	if(renderer == NULL)return 1;
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	texture = NULL;//SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING,screenwidth,screenheight);

	this->playthread = SDL_CreateThread(mythAvplayer::playyuv_static,"play",this);
	if(this->playthread == NULL)return 1;
	return 0;
}

int mythAvplayer::playyuv_static(void* mythav)
{
	mythAvplayer* avplayer = (mythAvplayer*)mythav;
	return avplayer->playyuv();
}


int mythAvplayer::playyuv(void)
{

#define BUFF_COUNT 1024*1024
	char* buf = new char[BUFF_COUNT];
	msocket = MythSocket::CreatedNew(connectip,5834);
	ffmpeg = mythFFmpeg::CreateNew((void*)this);
	char tmpsendstr[100];
	sprintf(tmpsendstr,"GET /CameraID=%d&Type=zyh264 HTTP/1.0\r\n\r\n",connectid);
	msocket->socket_SendStr(tmpsendstr,-2);
	SDL_Delay(100);
	while(!flag){
		int rc = msocket->socket_ReceiveDataLn2(buf,BUFF_COUNT,"Content_Length: ");
		if (rc > 0) {
			int got;
			AVPacket pkt;
			int tmplength = 0;
			tmplength = rc;
			ffmpeg->ProcessFrame((unsigned char*)buf,tmplength,disPlaySDL_static);
		}else{
			msocket->socket_CloseSocket();
			SDL_Delay(1000);
			msocket = MythSocket::CreatedNew(connectip,5834);
			msocket->socket_SendStr(tmpsendstr,-2);
		}
	}
	free(buf);
	return 0;
}

int mythAvplayer::startDecode(int width,int height)
{
	//check
	if(this->connectip == NULL ||
		this->connectport == NULL ||
		this->connectid == NULL)return 1;
	//start
	m_width = width > 0?width:640;
	m_height = height > 0?height:480;
	if(this->initSDL(m_width,m_height)!=0)return 1;

	while(!flag){
        while(SDL_PollEvent(&event))
        {
			if(event.window.event == SDL_WINDOWEVENT_RESIZED){
				SDL_SetWindowSize(window, event.window.data1, event.window.data2);
			}
			switch(event.type){
				case SDL_QUIT:
					flag = true;
					SDL_Delay(100);
					while(ListPlay.next != ListPlay.prev){
						struct DisplayList *tmp = list_entry(ListPlay.next,struct DisplayList,list);
						list_del(&tmp->list);
						free(tmp);
					}
					ffmpeg->Cleanup();
					msocket->socket_CloseSocket();
					SDL_DestroyTexture(texture);
					SDL_DestroyRenderer(renderer);
					SDL_DestroyWindow(window);
					SDL_Quit();
					return 0;
					break;
				default:
					break;
			}
		}
		if(ListPlay.next == ListPlay.prev){
			//SDL_PollEvent(NULL);
			SDL_Delay(1);
			continue;
		}

		//SDL_LockYUVOverlay(ps_bmp);
		struct DisplayList *tmp = list_entry(ListPlay.next,struct DisplayList,list);
        //memset(tmp->data[0],127,tmp->linesize[0]*height);
        //memset(tmp->data[1],127,tmp->linesize[1]*height);
        //memset(tmp->data[2],127,tmp->linesize[2]*height);
		SDL_UpdateYUVTexture(texture, NULL,
			tmp->data[0], tmp->linesize[0],
			tmp->data[1], tmp->linesize[1],
			tmp->data[2], tmp->linesize[2]);

		SDL_RenderCopy(renderer, texture, &s_rect, NULL);
		SDL_RenderPresent(renderer);

		for(int i = 0;i < 3;i++)
			SDL_free(tmp->data[i]);
		list_del(&tmp->list);
		SDL_free(tmp);
	}
	return 0;
}
mythAvplayer* mythAvplayer::CreateNew(char* ip,int cameraid)
{
	return new mythAvplayer(ip,5834,cameraid,NULL);
}
mythAvplayer* mythAvplayer::CreateNew(void* phwnd,char* ip,int cameraid)
{
	return new mythAvplayer(ip,5834,cameraid,phwnd);
}
mythAvplayer::~mythAvplayer(void)
{
	flag = true;
	SDL_Delay(100);
	while(ListPlay.next != ListPlay.prev){
		struct DisplayList *tmp = list_entry(ListPlay.next,struct DisplayList,list);
		list_del(&tmp->list);
		free(tmp);
	}
	ffmpeg->Cleanup();
	msocket->socket_CloseSocket();
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
